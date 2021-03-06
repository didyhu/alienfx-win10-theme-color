#include "stdafx.h"
#include <windows.h>
#include "LFX2.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	// Load alienfx dll.
	LFX2INITIALIZE initFunction;
	LFX2RELEASE releaseFunction;
	LFX2RESET resetFunction;
	LFX2UPDATE updateFunction;
	LFX2LIGHT lightFunction;
	HINSTANCE hLibrary = LoadLibrary(_T(LFX_DLL_NAME));
	if (!hLibrary)
	{
		MessageBox(NULL, L"Failed", L"Failed to load LightFX.dll.", MB_OK);
		return 1;
	}
	initFunction = (LFX2INITIALIZE)GetProcAddress(hLibrary, LFX_DLL_INITIALIZE);
	releaseFunction = (LFX2RELEASE)GetProcAddress(hLibrary, LFX_DLL_RELEASE);
	resetFunction = (LFX2RESET)GetProcAddress(hLibrary, LFX_DLL_RESET);
	updateFunction = (LFX2UPDATE)GetProcAddress(hLibrary, LFX_DLL_UPDATE);
	lightFunction = (LFX2LIGHT)GetProcAddress(hLibrary, LFX_DLL_LIGHT);
	LFX_RESULT result = initFunction();
	if (result != LFX_SUCCESS)
	{
		MessageBox(NULL, L"Failed", L"Failed to load LightFX.dll.", MB_OK);
		return 1;
	}
	result = resetFunction();
	DWORD cv = 0;
	// Watch if theme color changed.
	while (true)
	{
		HKEY keyDWM;
		LONG result = RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\DWM", 0, KEY_READ, &keyDWM);
		DWORD v = 0;
		DWORD dataSize = sizeof(v);
		LONG result2 = RegGetValueW(keyDWM, nullptr, L"ColorizationColor", RRF_RT_REG_DWORD, nullptr, &v, &dataSize);
		if (v == cv)
		{
			Sleep(500);
		}
		else
		{
			cv = v;
			lightFunction(LFX_ALL, v | 0x33000000);
			result = updateFunction();
		}
	}
	//FreeLibrary(hLibrary);
	//return 0;
}
