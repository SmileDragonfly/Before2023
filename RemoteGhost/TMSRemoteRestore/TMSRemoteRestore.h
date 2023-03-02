
// TMSRemoteRestore.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "utils/singleinstance.h"
#include "model\BusinessLayer\DataObject.h"
#include "json\InforStructJSON.h"

struct RLT_DATA_OBJECT
{
	template <class T> void orm(T &t)
	{
		t & ORM_COL(taskID);
		t & ORM_COL(status);
		t & ORM_COL(desc);
	}

	property<CString> taskID;
	property<int> status;
	property<CString> desc;

	template<class A> void mapobjects(A* t){}
	static CStringA getClassName()
	{
		return "data";
	}

	RLT_DATA_OBJECT()
	{
		CString sTemp(_T(""));
		taskID = desc = sTemp;
		status = 0;
	}
};


// CTMSRemoteRestoreApp:
// See TMSRemoteRestore.cpp for the implementation of this class
//

class CTMSRemoteRestoreApp : public CWinApp, public CSingleInstance
{
public:
	CTMSRemoteRestoreApp();

// Overrides
public:
	virtual BOOL InitInstance();
	CConfig gConfig;
	CScript gScript;
	CScript gNextOSScript;
	CDrive gDrive;
	CString m_strExportFolder;

	PROCESS_INFORMATION *m_pi;
// Implementation

	DECLARE_MESSAGE_MAP()
	virtual int Run();
public:
	void ExportResult(CString strType, CString strTaskID, INT nStatus, CString strDescription);
protected:
	BOOL ParseCommandLine(WCHAR **argv, int argc);

	BOOL DoCheck(CString strGhostPath, CString strTaskID, CString& strErr);
	void DoStart();
	BOOL DoRetry(CString& strErr);

	BOOL DoGhostAndRestartS2(CString& strErr);
	BOOL DoCheckAndRestartS2(CString& strErr);

	BOOL UpdateScriptS1(CString strGhostFilePath,CString strTaskID, CString &strErr);
	BOOL UpdateScriptS2(CString &strErr);
	
	BOOL CheckAndChangeBoot(CString &strErr);
	BOOL CheckBoot(CString &strErr);
	BOOL ChangeBoot(CString &strErr);
	BOOL CheckOSEnv(CString strOS2Path, CString &strErr);
	BOOL RestartComputer();

	BOOL RunGhost(CString &strErr);
	BOOL StartGhostTool(CString strDestinationDevicePath, CString strGhostFileDevicePath, CString &strErr);

	void DoneRestore();
	void ChangeBootFailedS1();
	void ChangeBootFailedS2();
	void ResetScript();
	BOOL ExecuteGhostTool(CString command, DWORD timeout, CString* serr = NULL);
};

extern CTMSRemoteRestoreApp theApp;
UINT MonitorGhostToolThread(LPVOID lpParam);