// dllmain.cpp : DllMain ��ʵ�֡�

#include "stdafx.h"
#include "resource.h"
#include "zShellExt_i.h"
#include "dllmain.h"

CzShellExtModule _AtlModule;
HINSTANCE g_hInstance = NULL;

// DLL ��ڵ�
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	g_hInstance = hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}
