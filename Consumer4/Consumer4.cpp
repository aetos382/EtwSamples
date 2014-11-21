// Consumer4.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "../Provider4/ProviderManifest4.h"

VOID WINAPI EventRecordCallback(_In_ PEVENT_RECORD eventRecord);

VOID ShowPropertyInfo(
    PEVENT_RECORD eventRecord,
    PTRACE_EVENT_INFO eventInfo,
    ULONG index);

VOID ShowPropertyValue(
    USHORT inputType,
    USHORT outputType,
    LPCVOID data);

int _tmain(int argc, _TCHAR* argv[])
{
    setlocale(LC_ALL, "");

    EVENT_TRACE_LOGFILE logFile = {};
    logFile.LogFileName = L"trace_test4.etl";
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

    if (!IsEqualGUID(header.ProviderId, PROVIDERID_SampleProvider4))
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

        wprintf_s(L"Provider   : %s", providerId);

        if (info->ProviderNameOffset != 0)
        {
            LPCWSTR providerName = reinterpret_cast<LPCWSTR>(reinterpret_cast<LPCBYTE>(info)+info->ProviderNameOffset);
            wprintf_s(L" (%s)", providerName);
        }

        wprintf_s(L"\n");

        if (info->ProviderMessageOffset != 0)
        {
            LPCWSTR providerMessage = reinterpret_cast<LPCWSTR>(reinterpret_cast<LPCBYTE>(info)+info->ProviderMessageOffset);
            wprintf_s(L"             %s\n", providerMessage);
        }

        wprintf_s(L"Channel    : %u", header.EventDescriptor.Channel);

        if (info->ChannelNameOffset != 0)
        {
            LPCWSTR channelName = reinterpret_cast<LPCWSTR>(reinterpret_cast<LPCBYTE>(info)+info->ChannelNameOffset);
            wprintf_s(L" (%s)", channelName);
        }

        wprintf_s(L"\n");

        wprintf_s(L"Event      : %u (Version: %u)\n", header.EventDescriptor.Id, header.EventDescriptor.Version);

        if (info->EventMessageOffset != 0)
        {
            LPCWSTR eventMessage = reinterpret_cast<LPCWSTR>(reinterpret_cast<LPCBYTE>(info)+info->EventMessageOffset);
            wprintf_s(L"             %s\n", eventMessage);
        }

        wprintf_s(L"Level      : %u", header.EventDescriptor.Level);

        if (info->LevelNameOffset != 0)
        {
            LPCWSTR levelName = reinterpret_cast<LPCWSTR>(reinterpret_cast<LPCBYTE>(info)+info->LevelNameOffset);
            wprintf_s(L" (%s)", levelName);
        }

        wprintf_s(L"\n");

        wprintf_s(L"Task       : %u", header.EventDescriptor.Task);

        if (info->TaskNameOffset != 0)
        {
            LPCWSTR taskName = reinterpret_cast<LPCWSTR>(reinterpret_cast<LPCBYTE>(info)+info->TaskNameOffset);
            wprintf_s(L" (%s)", taskName);
        }

        wprintf_s(L"\n");

        wprintf_s(L"Opcode     : %u", header.EventDescriptor.Opcode);

        if (info->OpcodeNameOffset != 0)
        {
            LPCWSTR opcodeName = reinterpret_cast<LPCWSTR>(reinterpret_cast<LPCBYTE>(info)+info->OpcodeNameOffset);
            wprintf_s(L" (%s)", opcodeName);
        }

        wprintf_s(L"\n");

        wprintf_s(L"Keywords   : %#0I64x\n", header.EventDescriptor.Keyword);

        if (info->KeywordsNameOffset != 0)
        {
            LPCWSTR keywordName = reinterpret_cast<LPCWSTR>(reinterpret_cast<LPCBYTE>(info)+info->KeywordsNameOffset);
            while (*keywordName != L'\0')
            {
                wprintf_s(L"             %s\n", keywordName);

                keywordName += wcslen(keywordName) + 1;
            }
        }

        FILETIME time = { header.TimeStamp.LowPart, header.TimeStamp.HighPart };
        FILETIME localTime = {};
        SYSTEMTIME st = {};

        FileTimeToLocalFileTime(&time, &localTime);
        FileTimeToSystemTime(&localTime, &st);

        wprintf_s(L"Time       : %04u-%02u-%02u %02u:%02u:%02u.%07u\n",
            st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,
            header.TimeStamp.QuadPart % 10000000);

        wprintf_s(L"Process    : %u\n", header.ProcessId);
        wprintf_s(L"Thread     : %u\n", header.ThreadId);
        wprintf_s(L"Flags      : %#0hx\n", header.Flags);

        if ((header.Flags & EVENT_HEADER_FLAG_EXTENDED_INFO) != 0)
        {
            _putws(L"             Extended Info");
        }

        if ((header.Flags & EVENT_HEADER_FLAG_PRIVATE_SESSION) != 0)
        {
            _putws(L"             Private Session");
        }

        if ((header.Flags & EVENT_HEADER_FLAG_STRING_ONLY) != 0)
        {
            _putws(L"             String Only");
        }

        if ((header.Flags & EVENT_HEADER_FLAG_TRACE_MESSAGE) != 0)
        {
            _putws(L"             Trace Message\n");
        }

        if ((header.Flags & EVENT_HEADER_FLAG_NO_CPUTIME) != 0)
        {
            _putws(L"             No CPU Time");
        }

        if ((header.Flags & EVENT_HEADER_FLAG_32_BIT_HEADER) != 0)
        {
            _putws(L"             32bit Header");
        }

        if ((header.Flags & EVENT_HEADER_FLAG_64_BIT_HEADER) != 0)
        {
            _putws(L"             64bit Header");
        }

        if ((header.Flags & EVENT_HEADER_FLAG_CLASSIC_HEADER) != 0)
        {
            _putws(L"             Classic Header");
        }

        if ((header.Flags & EVENT_HEADER_FLAG_PROCESSOR_INDEX) != 0)
        {
            _putws(L"             Processor Index");
        }

        /*
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
        */

        _putws(L"Properties :");

        for (ULONG i = 0; i < info->TopLevelPropertyCount; ++i)
        {
            ShowPropertyInfo(eventRecord, info, i);
        }

        _putws(L"");
    }
    __finally
    {
        free(info);
    }
}

VOID ShowPropertyInfo(
    PEVENT_RECORD eventRecord,
    PTRACE_EVENT_INFO eventInfo,
    ULONG index)
{
    static LPCWSTR const inTypes[] = {
        L"NULL",
        L"Unicode String",
        L"Ansi String",
        L"INT8",
        L"UINT8",
        L"INT16",
        L"UINT16",
        L"INT32",
        L"UINT32",
        L"INT64",
        L"UINT64",
        L"FLOAT",
        L"DOUBLE",
        L"BOOLEAN",
        L"BINARY",
        L"GUID",
        L"POINTER",
        L"FILETIME",
        L"SYSTEMTIME",
        L"SID",
        L"HEXINT32",
        L"HEXINT64"
    };

    static LPCWSTR const outTypes[] = {
        L"NULL",
        L"String",
        L"DateTime",
        L"BYTE",
        L"UBYTE",
        L"SHORT",
        L"USHORT",
        L"INT",
        L"UINT",
        L"LONG",
        L"ULONG",
        L"FLOAT",
        L"DOUBLE",
        L"BOOLEAN",
        L"GUID",
        L"HEXBINARY",
        L"HEXINT8",
        L"HEXINT16",
        L"HEXINT32",
        L"HEXINT64",
        L"PID",
        L"TID",
        L"PORT",
        L"IPv4",
        L"IPv6",
        L"Socket Address",
        L"CIM DateTime",
        L"ETWTIME",
        L"XML",
        L"ErrorCode",
        L"Win32Error",
        L"NTStatus",
        L"HRESULT",
        L"Culture neutral datetime"
    };

    EVENT_PROPERTY_INFO * property = &eventInfo->EventPropertyInfoArray[index];

    LPCWSTR propertyName = reinterpret_cast<LPCWSTR>(reinterpret_cast<LPBYTE>(eventInfo) + property->NameOffset);

    PROPERTY_DATA_DESCRIPTOR propDesc = {};
    propDesc.PropertyName = reinterpret_cast<ULONGLONG>(propertyName);
    propDesc.ArrayIndex = ULONG_MAX;

    ULONG propSize = 0;
    TDHSTATUS status = TdhGetPropertySize(eventRecord, 0, NULL, 1, &propDesc, &propSize);
    if (status != ERROR_SUCCESS)
    {
        return;
    }

    LPVOID buffer = malloc(propSize);

    __try
    {
        status = TdhGetProperty(eventRecord, 0, NULL, 1, &propDesc, propSize, static_cast<LPBYTE>(buffer));
        if (status == ERROR_SUCCESS)
        {
            USHORT inType = property->nonStructType.InType;
            USHORT outType = property->nonStructType.OutType;

            wprintf_s(L"\tName        : %s\n", propertyName);
            wprintf_s(L"\tInput Type  : %s\n", inTypes[inType]);
            wprintf_s(L"\tOutput Type : %s\n", outTypes[outType]);

            ShowPropertyValue(inType, outType, buffer);
        }
    }
    __finally
    {
        free(buffer);
    }
}

VOID ShowPropertyValue(
    USHORT inputType,
    USHORT outputType,
    LPCVOID data)
{
    wprintf_s(L"\t\t");

    switch (inputType)
    {
        case TDH_INTYPE_UNICODESTRING:
            wprintf_s(L"%s\n", static_cast<LPCWSTR>(data));
            break;

        case TDH_INTYPE_UINT32:
            wprintf_s(L"%u\n", *(static_cast<UINT32 const *>(data)));
            break;
    }

    _putws(L"");
}
