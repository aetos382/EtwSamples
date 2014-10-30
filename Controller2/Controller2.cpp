// Controller2.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "../Provider0/ProviderManifest0.h"

int _tmain(int argc, _TCHAR* argv[])
{
	LPCWSTR sessionName = L"EtwSamples.Controller2";

	const DWORD logFileNameOffset = sizeof(EVENT_TRACE_PROPERTIES);
	const DWORD logFileNameLength = MAX_PATH;
	const DWORD loggerNameOffset = sizeof(EVENT_TRACE_PROPERTIES) + (sizeof(WCHAR) * logFileNameLength);
	const DWORD loggerNameLength = 80;

	const DWORD size = sizeof(EVENT_TRACE_PROPERTIES) + (sizeof(WCHAR) * logFileNameLength) + (sizeof(WCHAR) * loggerNameLength);
	BYTE buffer[size] = {};

	EVENT_TRACE_PROPERTIES * prop = reinterpret_cast<EVENT_TRACE_PROPERTIES *>(buffer);

	prop->Wnode.BufferSize = size;
	prop->Wnode.Flags = WNODE_FLAG_TRACED_GUID;
	prop->LogFileMode = EVENT_TRACE_FILE_MODE_SEQUENTIAL | EVENT_TRACE_USE_PAGED_MEMORY | EVENT_TRACE_REAL_TIME_MODE;
	prop->LogFileNameOffset = logFileNameOffset;
	prop->LoggerNameOffset = loggerNameOffset;

	LPWSTR logFileName = reinterpret_cast<LPWSTR>(buffer + logFileNameOffset);
	LPWSTR loggerName = reinterpret_cast<LPWSTR>(buffer + loggerNameOffset);

	wcscpy_s(logFileName, logFileNameLength, L"Controller2.etl");
	wcscpy_s(loggerName, loggerNameLength, sessionName);

	TRACEHANDLE traceHandle = INVALID_PROCESSTRACE_HANDLE;
	ULONG result = StartTrace(&traceHandle, sessionName, prop);

	result = EnableTraceEx(&PROVIDERID_SampleProvider0, NULL, traceHandle, 1, 0, 0, 0, 0, NULL);

	_putws(L"press any key to continue.");
	_getwch();

	result = EnableTraceEx(&PROVIDERID_SampleProvider0, NULL, traceHandle, 0, 0, 0, 0, 0, NULL);

	result = StopTrace(traceHandle, NULL, prop);

	return 0;
}

