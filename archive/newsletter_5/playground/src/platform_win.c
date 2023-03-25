#ifdef _WIN32

#include <windows.h>
#include <ShlObj.h>

int win_sleep_ms(int ms) {
	HANDLE timer;	
	LARGE_INTEGER li;	
	LONGLONG ns = ms * 10000;

	if(!(timer = CreateWaitableTimer(NULL, TRUE, NULL)))
		return FALSE;

	li.QuadPart = -ns;

	if(!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE)){
		CloseHandle(timer);
		return FALSE;
	}

	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
	
	return TRUE;
}

int win_get_document_path(char *path) {
	wchar_t folder[1024];
	HRESULT hr = SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, 0, folder);
	
	if (SUCCEEDED(hr)) {
		wcstombs(path, folder, 1023);
		return 1;
	}
	else {
		path = 0;
		return 0;
	}
}

#endif
