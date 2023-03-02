// dllmain.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxwin.h>
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE DllIFIUIStyle2018DLL = { NULL, NULL };

GdiplusStartupInput  gdiplusStartupInput;
ULONG_PTR  gdiplusToken = 0;
HINSTANCE gDllIFIUIStyle201Ins = 0;

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("DllIFIUIStyle2018.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(DllIFIUIStyle2018DLL, hInstance))
			return 0;

		gDllIFIUIStyle201Ins = hInstance;
		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(DllIFIUIStyle2018DLL);

		GdiplusStartup(& gdiplusToken, & gdiplusStartupInput, NULL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("DllIFIUIStyle2018.DLL Terminating!\n");

		GdiplusShutdown(gdiplusToken);

		gDllIFIUIStyle201Ins = NULL;

		// Terminate the library before destructors are called
		AfxTermExtensionModule(DllIFIUIStyle2018DLL);		
	}
	return 1;   // ok
}
