// Provider3.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ProviderManifest3.h"

int _tmain(int argc, _TCHAR* argv[])
{
    REGHANDLE hTrace = NULL;
    ULONG result = EventRegister(&PROVIDERID_SampleProvider3, NULL, NULL, &hTrace);
    if (result != ERROR_SUCCESS)
    {
        return 1;
    }

    result = EventWrite(hTrace, &EVENTDESC_Hello, 0, NULL);
    result = EventWrite(hTrace, &EVENTDESC_Hello2, 0, NULL);

    result = EventUnregister(hTrace);

	return 0;
}

