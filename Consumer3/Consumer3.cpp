// Consumer3.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "../Provider3/ProviderManifest3.h"

VOID WINAPI EventRecordCallback(_In_ PEVENT_RECORD eventRecord);

int _tmain(int argc, _TCHAR* argv[])
{
    setlocale(LC_ALL, "");

    EVENT_TRACE_LOGFILE logFile = {};
    logFile.LogFileName = L"trace_test3.etl";
    logFile.LoggerName = NULL;
    logFile.ProcessTraceMode =
        PROCESS_TRACE_MODE_EVENT_RECORD;
    logFile.EventRecordCallback = &EventRecordCallback;
    logFile.Context = &logFile;

    TRACEHANDLE hTrace = OpenTrace(&logFile);
    if (hTrace == INVALID_PROCESSTRACE_HANDLE)
    {
        return 1;
    }

    ULONG result = ProcessTrace(&hTrace, 1, NULL, NULL);

    CloseTrace(hTrace);

    return 0;
}

VOID WINAPI EventRecordCallback(_In_ PEVENT_RECORD eventRecord)
{
    EVENT_HEADER & header = eventRecord->EventHeader;

    if (!IsEqualGUID(header.ProviderId, PROVIDERID_SampleProvider3))
    {
        return;
    }

    ULONG size = 0;
    TDHSTATUS status = TdhGetEventInformation(eventRecord, 0, NULL, NULL, &size);

    if (status != ERROR_SUCCESS && status != ERROR_INSUFFICIENT_BUFFER)
    {
        return;
    }

    TRACE_EVENT_INFO * info = static_cast<TRACE_EVENT_INFO *>(malloc(size));

    __try
    {
        status = TdhGetEventInformation(eventRecord, 0, NULL, info, &size);

        if (status != ERROR_SUCCESS)
        {
            return;
        }

        WCHAR providerId[80] = {};
        StringFromGUID2(header.ProviderId, providerId, _countof(providerId));

        wprintf_s(L"Provider : %s", providerId);

        if (info->ProviderNameOffset != 0)
        {
            LPCWSTR providerName = reinterpret_cast<LPCWSTR>(reinterpret_cast<LPCBYTE>(info) + info->ProviderNameOffset);
            wprintf_s(L" (%s)", providerName);
        }

        wprintf_s(L"\n");

        if (info->ProviderMessageOffset != 0)
        {
            LPCWSTR providerMessage = reinterpret_cast<LPCWSTR>(reinterpret_cast<LPCBYTE>(info) + info->ProviderMessageOffset);
            wprintf_s(L"           %s\n", providerMessage);
        }

        wprintf_s(L"Channel  : %u", header.EventDescriptor.Channel);

        if (info->ChannelNameOffset != 0)
        {
            LPCWSTR channelName = reinterpret_cast<LPCWSTR>(reinterpret_cast<LPCBYTE>(info) + info->ChannelNameOffset);
            wprintf_s(L" (%s)", channelName);
        }

        wprintf_s(L"\n");
        
        wprintf_s(L"Event    : %u (Version: %u)\n", header.EventDescriptor.Id, header.EventDescriptor.Version);

        if (info->EventMessageOffset != 0)
        {
            LPCWSTR eventMessage = reinterpret_cast<LPCWSTR>(reinterpret_cast<LPCBYTE>(info) + info->EventMessageOffset);
            wprintf_s(L"           %s\n", eventMessage);
        }
        
        wprintf_s(L"Level    : %u", header.EventDescriptor.Level);

        if (info->LevelNameOffset != 0)
        {
            LPCWSTR levelName = reinterpret_cast<LPCWSTR>(reinterpret_cast<LPCBYTE>(info) + info->LevelNameOffset);
            wprintf_s(L" (%s)", levelName);
        }

        wprintf_s(L"\n");
        
        wprintf_s(L"Task     : %u", header.EventDescriptor.Task);

        if (info->TaskNameOffset != 0)
        {
            LPCWSTR taskName = reinterpret_cast<LPCWSTR>(reinterpret_cast<LPCBYTE>(info) + info->TaskNameOffset);
            wprintf_s(L" (%s)", taskName);
        }

        wprintf_s(L"\n");
        
        wprintf_s(L"Opcode   : %u", header.EventDescriptor.Opcode);

        if (info->OpcodeNameOffset != 0)
        {
            LPCWSTR opcodeName = reinterpret_cast<LPCWSTR>(reinterpret_cast<LPCBYTE>(info) + info->OpcodeNameOffset);
            wprintf_s(L" (%s)", opcodeName);
        }

        wprintf_s(L"\n");
        
        wprintf_s(L"Keywords : %#0I64x\n", header.EventDescriptor.Keyword);

        if (info->KeywordsNameOffset != 0)
        {
            LPCWSTR keywordName = reinterpret_cast<LPCWSTR>(reinterpret_cast<LPCBYTE>(info) + info->KeywordsNameOffset);
            while (*keywordName != L'\0')
            {
                wprintf_s(L"           %s\n", keywordName);

                keywordName += wcslen(keywordName) + 1;
            }
        }

        FILETIME time = { header.TimeStamp.LowPart, header.TimeStamp.HighPart };
        FILETIME localTime = {};
        SYSTEMTIME st = {};

        FileTimeToLocalFileTime(&time, &localTime);
        FileTimeToSystemTime(&localTime, &st);

        wprintf_s(L"Time     : %04u-%02u-%02u %02u:%02u:%02u.%07u\n",
            st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,
            header.TimeStamp.QuadPart % 10000000);

        wprintf_s(L"Process  : %u\n", header.ProcessId);
        wprintf_s(L"Thread   : %u\n", header.ThreadId);
        wprintf_s(L"Flags    : %#0hx\n", header.Flags);

        if ((header.Flags & EVENT_HEADER_FLAG_EXTENDED_INFO) != 0)
        {
            _putws(L"           Extended Info");
        }

        if ((header.Flags & EVENT_HEADER_FLAG_PRIVATE_SESSION) != 0)
        {
            _putws(L"           Private Session");
        }

        if ((header.Flags & EVENT_HEADER_FLAG_STRING_ONLY) != 0)
        {
            _putws(L"           String Only");
        }

        if ((header.Flags & EVENT_HEADER_FLAG_TRACE_MESSAGE) != 0)
        {
            _putws(L"           Trace Message\n");
        }

        if ((header.Flags & EVENT_HEADER_FLAG_NO_CPUTIME) != 0)
        {
            _putws(L"           No CPU Time");
        }

        if ((header.Flags & EVENT_HEADER_FLAG_32_BIT_HEADER) != 0)
        {
            _putws(L"           32bit Header");
        }

        if ((header.Flags & EVENT_HEADER_FLAG_64_BIT_HEADER) != 0)
        {
            _putws(L"           64bit Header");
        }

        if ((header.Flags & EVENT_HEADER_FLAG_CLASSIC_HEADER) != 0)
        {
            _putws(L"           Classic Header");
        }

        if ((header.Flags & EVENT_HEADER_FLAG_PROCESSOR_INDEX) != 0)
        {
            _putws(L"           Processor Index");
        }

        wprintf_s(L"Property : %#0hx\n", header.EventProperty);

        if ((header.EventProperty & EVENT_HEADER_PROPERTY_XML) != 0)
        {
            _putws(L"           XML");
        }

        if ((header.EventProperty & EVENT_HEADER_PROPERTY_FORWARDED_XML) != 0)
        {
            _putws(L"           Forwarded XML");
        }

        if ((header.EventProperty & EVENT_HEADER_PROPERTY_LEGACY_EVENTLOG) != 0)
        {
            _putws(L"           Legacy");
        }

        _putws(L"");
    }
    __finally
    {
        free(info);
    }
}
