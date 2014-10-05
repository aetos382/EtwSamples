// Provider.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "ProviderManifest.h"

int _tmain(int argc, _TCHAR* argv[])
{
	REGHANDLE hTrace = NULL;
	ULONG result = EventRegister(&PROVIDERID_SampleProvider1, NULL, NULL, &hTrace);
	if (result != ERROR_SUCCESS)
	{
		return 1;
	}

	result = EventWrite(hTrace, &EVENTDESC_Hello, 0, NULL);

	result = EventUnregister(hTrace);
	return 0;
}
