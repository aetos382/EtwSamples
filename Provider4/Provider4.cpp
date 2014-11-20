// Provider4.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ProviderManifest4.h"

int _tmain(int argc, _TCHAR* argv[])
{
    REGHANDLE hTrace = NULL;
    ULONG result = EventRegister(&PROVIDERID_SampleProvider4, NULL, NULL, &hTrace);
    if (result != ERROR_SUCCESS)
    {
        return 1;
    }

    EVENT_DATA_DESCRIPTOR descriptor[2] = {};

    WCHAR name[] = { L"Alice" };
    UINT score = 95;

    EventDataDescCreate(&descriptor[0], name, sizeof(name));
    EventDataDescCreate(&descriptor[1], &score, sizeof(score));

    result = EventWrite(hTrace, &EVENTDESC_Hello, _countof(descriptor), descriptor);

    result = EventUnregister(hTrace);

    return 0;
}

