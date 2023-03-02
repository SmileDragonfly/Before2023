
// SeverIso8583App.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSeverIso8583AppApp:
// See SeverIso8583App.cpp for the implementation of this class
//

class CSeverIso8583AppApp : public CWinApp
{
public:
	CSeverIso8583AppApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSeverIso8583AppApp theApp;
