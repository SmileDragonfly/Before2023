
// BAITHUCHANH1.h : main header file for the BAITHUCHANH1 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CBAITHUCHANH1App:
// See BAITHUCHANH1.cpp for the implementation of this class
//

class CBAITHUCHANH1App : public CWinApp
{
public:
	CBAITHUCHANH1App();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBAITHUCHANH1App theApp;
