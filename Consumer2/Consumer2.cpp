// Consumer2.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "../Provider0/ProviderManifest0.h"

VOID WINAPI EventRecordCallback(_In_ PEVENT_RECORD eventRecord);

int _tmain(int argc, _TCHAR* argv[])
{
	EVENT_TRACE_LOGFILE logFile = {};
	logFile.LogFileName = L"Controller2.etl";
	logFile.LoggerName = L"EtwSamples.Controller2";
	logFile.ProcessTraceMode =
		PROCESS_TRACE_MODE_EVENT_RECORD | PROCESS_TRACE_MODE_REAL_TIME;
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

	if (!IsEqualGUID(header.ProviderId, PROVIDERID_SampleProvider0))
	{
		return;
	}

	WCHAR providerId[80] = {};
	StringFromGUID2(header.ProviderId, providerId, _countof(providerId));

	wprintf_s(L"Provider : %s\n", providerId);
	wprintf_s(L"Channel  : %u\n", header.EventDescriptor.Channel);
	wprintf_s(L"Event    : %u (Version: %u)\n", header.EventDescriptor.Id, header.EventDescriptor.Version);
	wprintf_s(L"Level    : %u\n", header.EventDescriptor.Level);
	wprintf_s(L"Task     : %u\n", header.EventDescriptor.Task);
	wprintf_s(L"Opcode   : %u\n", header.EventDescriptor.Opcode);
	wprintf_s(L"Keywords : %#0I64x\n", header.EventDescriptor.Keyword);

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
