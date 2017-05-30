// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Windows.h>

int main()
{
	HMODULE hDll = LoadLibrary(L"BLP WIC codec.dll");
	HRESULT(STDAPICALLTYPE *DllRegisterServer)(void) = (HRESULT(STDAPICALLTYPE*)(void))GetProcAddress(hDll, "DllRegisterServer");
    
	DllRegisterServer();
	return 0;
}

