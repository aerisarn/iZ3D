/* IZ3D_FILE: $Id$ 
*
* Project : iZ3D Stereo Driver
* Copyright (C) iZ3D Inc. 2002 - 2010
*
* $Author$
* $Revision$
* $Date$
* $LastChangedBy$
* $URL$
*/
#include "stdafx.h"

#define UNHOOK_DELAY 10*1000

__declspec(dllexport) DWORD WINAPI StartDDAServer(void *p)
{
	return (DWORD&)gInfo;
}

static GUID g_magicTag =  { 0x5f9fd868, 0xb2a7, 0xbf42, 0xaa, 0x13, 0xe3, 0x39, 0x34, 0xe1, 0x2a, 0x20  };

__declspec(dllexport) DWORD WINAPI SetSyncRenderCallback(void *p)
{
	static void* i;
	if(!p)
		i = p;
	else 
		return -1;
	return (DWORD)&g_magicTag;
}

__declspec(dllexport) DWORD WINAPI ClearHandles(HANDLE *p, int counter)
{
	for(int i=0; i< counter; i++)
	{
		WaitForSingleObject(p[i], 5500);
		CloseHandle(p[i]);
	}
	return 0;
}

static HANDLE g_HookThread = 0;

DWORD WINAPI UnHookThread(LPVOID lpParameter)
{
	Sleep(UNHOOK_DELAY);
	g_HookThread = 0;
	return 0;										    
}

void __declspec(dllexport) WINAPI StopDDAServer(IDirect3D9 *pOriginalD3D9)
{
	//--- renew hook ---
	UnHookThread((LPVOID)StopDDAServer);
}
