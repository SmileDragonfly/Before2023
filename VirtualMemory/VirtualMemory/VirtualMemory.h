
// VirtualMemory.h : main header file for the VirtualMemory application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CVirtualMemoryApp:
// See VirtualMemory.cpp for the implementation of this class
//

class CVirtualMemoryApp : public CWinApp
{
public:
	CVirtualMemoryApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CVirtualMemoryApp theApp;
