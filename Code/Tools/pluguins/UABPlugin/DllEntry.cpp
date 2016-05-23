//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Contains the Dll Entry stuff
// AUTHOR: 
//***************************************************************************/

#include "maxscrpt/maxscrpt.h"
HINSTANCE hInstance;
// ========================================================
// Grab onto this DLL's instance handle
BOOL WINAPI DllMain(HINSTANCE DLLhinst, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			hInstance = DLLhinst;
			break;
	}
	return TRUE;
}

__declspec (dllexport) void LibInit()
{
	// TODO: Put any code for initializing your plug-in here.
#ifdef _DEBUG
	MessageBox(NULL, _T("My MAXScript plug-in is loaded!!"), _T("Testing!"), MB_OK);
#endif
}
__declspec (dllexport) const TCHAR* LibDescription()
{
	// TODO: Put code in here telling what your plug-in does.
	return _T("My MAXScript plug-in");
}

__declspec (dllexport) ULONG LibVersion()
{
	// Return the version of the Max SDK
	return VERSION_3DSMAX;
}
