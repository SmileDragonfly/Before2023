
// TMSRemoteRestore.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TMSRemoteRestore.h"
#include "utils\CrashHandler.h"
#include "utils\FileVersionInfo.h"
#include "utils\Crc32Faster.h"
#include "utils\CRC32.h"
#include <TlHelp32.h>
#include "utils\ExportMessageStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCrashHandler g_crashHandler(_T("PROFILEMON"));

// CTMSRemoteRestoreApp

BEGIN_MESSAGE_MAP(CTMSRemoteRestoreApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTMSRemoteRestoreApp construction

CTMSRemoteRestoreApp::CTMSRemoteRestoreApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTMSRemoteRestoreApp object

CTMSRemoteRestoreApp theApp;


// CTMSRemoteRestoreApp initialization

BOOL CTMSRemoteRestoreApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	// Init crash handler
	g_crashHandler.SetAllExceptionHandlers();
	// Create single instance
	if (CSingleInstance::Create(_T("Microtec-TMS-Remote-Restore")) == FALSE)
		return FALSE;
	// Load config
	if (!gConfig.Init())
	{
		LOGERROR(_T("Can not load configuration."));
		return FALSE;
	}
	m_strExportFolder = gConfig.PATH_WORKING();
	// Load script
	if (!gScript.Init())
	{
		LOGERROR(_T("Can not load script."));
		return FALSE;
	}

	LOGINFO(_T("-----------------------------------------------------------------------"));
	LOGINFO(_T("TMSRemoteRestore monitor start"));

	// Log version
	CString sLog(_T(""));
	CFileVersionInfo f;
	f.Create();

	CString	strPath(_T(""));
	GetModuleFileName(NULL, strPath.GetBuffer(_MAX_PATH), _MAX_PATH);
	strPath.ReleaseBuffer();

	DWORD dwCRC32;
	CCRC32::Crc32File(strPath, dwCRC32);
	sLog.Format(_T("Version %s | CRC: %08X"), f.GetFileVersion(), dwCRC32);
	LOGINFO(sLog);

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	ParseCommandLine(__targv, __argc);
	return FALSE;
}



int CTMSRemoteRestoreApp::Run()
{
	return CWinThread::Run();
}

BOOL CTMSRemoteRestoreApp::ParseCommandLine(WCHAR **argv, int argc)
{
	if (argc > 1)
	{
		LPCTSTR pszParam = argv[1];
		if (lstrcmpi(pszParam, COMMAND_CHECK) == 0)
		{
			if (argc > 3)
			{
				CString strLog(_T(""));
				CString strErr(_T(""));
				CString strGhostPath(argv[2]);
				CString strTaskID(argv[3]);
				strLog.Format(_T("Command line: -check. Ghost file path: %s"), strGhostPath);
				if (!DoCheck(strGhostPath, strTaskID, strErr))
				{
					strLog.Format(_T("Step 1: Failed. Error=%s"), strErr);
					LOGINFO_FUNC(strLog);
					ExportResult(GHOST_STATUS_STRING, strTaskID, GHOST_STATUS_FAIL, strLog);
					ResetScript();
				}
				else
				{
					LOGINFO_FUNC(_T("Step 1: Succeed. Change boot and restart"));
					ExportResult(GHOST_EVENT_STRING, strTaskID, GHOST_STATUS_SUCCEED, _T("Step 1: Succeed. Change boot and restart"));
				}
			}
			else
			{
				LOGINFO_FUNC(_T("Command line: -check. Invalid call command check"));
			}
		}
		else if (lstrcmpi(pszParam, COMMAND_START) == 0)
		{
			LOGINFO_FUNC(_T("Command line: -start"));
			DoStart();
		}
		else if (lstrcmpi(pszParam, COMMAND_RETRY) == 0)
		{
			LOGINFO_FUNC(_T("Command line: -retry"));
			CString strErr(_T(""));
			if (!DoRetry(strErr))
			{
				LOGINFO_FUNC(_T("Retry failed"));
				ExportResult(GHOST_STATUS_STRING, gScript.TASK_ID(), GHOST_STATUS_FAIL, _T("Begin retry"));
			}
			else
			{
				LOGINFO_FUNC(_T("Begin retry"));
				ExportResult(GHOST_EVENT_STRING, gScript.TASK_ID(), GHOST_STATUS_SUCCEED, _T("Begin retry"));
			}
		}
		else
		{
			LOGINFO_FUNC(_T("Invalid command line"));
		}
	}
	else
	{
		LOGINFO_FUNC(_T("Not support no command line"));
	}
	return FALSE;
}

BOOL CTMSRemoteRestoreApp::DoCheck(CString strGhostPath, CString strTaskID, CString& strErr)
{
	// Check ghost file exist
	if (!PathFileExists(strGhostPath))
	{
		strErr = _T("File ghost do not exist");
		LOGINFO_FUNC(_T("File ghost do not exist"));
		return FALSE;
	}
	else
	{
		if (gScript.GHOST_FLAG() != 0)
		{
			strErr = _T("Ghost process is running");
			LOGINFO_FUNC(_T("Ghost process is running"));
			return FALSE;
		}
		else
		{
			CString strErr(_T(""));
			CString sLog(_T(""));
			if (!CheckAndChangeBoot(strErr))
			{
				LOGINFO_FUNC(strErr);
				return FALSE;
			}
			// Update file Script.ini on current OS and next OS
			if (!UpdateScriptS1(strGhostPath, strTaskID, strErr))
			{
				LOGINFO_FUNC(strErr);
				return FALSE;
			}

			// Restart
			if (!RestartComputer())
			{
				strErr = _T("Cannot restart computer");
				LOGINFO_FUNC(strErr);
				return FALSE;
			}
		}
	}
	return TRUE;
}

void CTMSRemoteRestoreApp::DoStart()
{
	// Check script ghost_flag and prev_os_flag
	CString strLog(_T(""));
	CString strErr(_T(""));
	if (gScript.PREV_OS_FLAG() == 1)
	{
		switch (gScript.GHOST_FLAG())
		{
		case 1:
			ChangeBootFailedS1(); // run command -check to retry
			break;
		case 2:
			if (!DoCheckAndRestartS2(strErr))
			{
				strLog.Format(_T("Step 2.2: Failed. Error=%s"), strErr);
				LOGINFO_FUNC(strLog);
				ExportResult(GHOST_STATUS_STRING, gScript.TASK_ID(), GHOST_STATUS_FAIL, strLog);
			}
			else
			{
				strLog.Format(_T("Step 2.2: Check next OS succeed. Change boot and restart"));
				LOGINFO_FUNC(strLog);
				ExportResult(GHOST_EVENT_STRING, gScript.TASK_ID(), GHOST_STATUS_SUCCEED, strLog);
			}
			break;
		case 3:
			ChangeBootFailedS2(); // run command -check to retry
			break;
		default:
			strLog.Format(_T("Invalid state: GHOST_FLAG=%d,PREV_OS_FLAG=%d"), gScript.PREV_OS_FLAG(), gScript.GHOST_FLAG());
			LOGINFO_FUNC(strLog);
			ExportResult(GHOST_STATUS_STRING, gScript.TASK_ID(), GHOST_STATUS_FAIL, strLog);
			ResetScript();
			break;
		}
	}
	else
	{
		switch (gScript.GHOST_FLAG())
		{
		case 0:
			LOGINFO_FUNC(_T("Do nothing. Quit"));
			break;
		case 1:
			if (!DoGhostAndRestartS2(strErr))
			{
				strLog.Format(_T("Step 2.1: Failed. Error=%s"), strErr);
				LOGINFO_FUNC(strLog);
				ExportResult(GHOST_STATUS_STRING, gScript.TASK_ID(), GHOST_STATUS_FAIL, strLog);
			}
			else
			{
				strLog.Format(_T("Step 2.1: Restore succeed. Restart"));
				LOGINFO_FUNC(strLog);
				ExportResult(GHOST_EVENT_STRING, gScript.TASK_ID(), GHOST_STATUS_SUCCEED, strLog);
			}
			break;
		//case 2:
		//	DoCheckAndRestartS2();
		//	break;
		case 3:
			DoneRestore();
			break;
		default:
			strLog.Format(_T("Invalid flag: GHOST_FLAG=%d,PREV_OS_FLAG=%d. Reset flags"), gScript.PREV_OS_FLAG(), gScript.GHOST_FLAG());
			LOGINFO_FUNC(strLog);
			ExportResult(GHOST_STATUS_STRING, gScript.TASK_ID(), GHOST_STATUS_FAIL, strLog);
			ResetScript();
			break;
		}
	}
}

BOOL CTMSRemoteRestoreApp::DoRetry(CString& strErr)
{
	if (((gScript.GHOST_FLAG() == 1) && (gScript.PREV_OS_FLAG() == 0)) ||
		((gScript.GHOST_FLAG() == 2) && (gScript.PREV_OS_FLAG() == 1)) ||
		((gScript.GHOST_FLAG() == 3) && (gScript.PREV_OS_FLAG() == 1)))
	{
		return DoGhostAndRestartS2(strErr);
	}
	else
	{
		strErr = _T("Retry failed. Cannot retry in this state");
		return FALSE;
	}
}

BOOL CTMSRemoteRestoreApp::UpdateScriptS1(CString strGhostFilePath,CString strTaskID, CString &strErr)
{
	CString strLog(_T(""));
	// Convert file path to device path
	CString strNextOSPath = gDrive.BOOT_DRIVE();

	CString strScriptNextOSPath = MakeFullPath(strNextOSPath, TMS_FOLDER_NAME);
	strScriptNextOSPath = MakeFullPath(strScriptNextOSPath, TMS_REMOTE_RESTORE_NAME);

	if (!gNextOSScript.Init(strScriptNextOSPath))
	{
		strErr = _T("Can not init Script.ini on next OS");
		LOGINFO_FUNC(_T("Can not init Script.ini on next OS"));
		return FALSE;
	}
	// Update current OS
	gScript.TASK_ID(strTaskID);
	gScript.PREV_OS_FLAG(1);
	gScript.GHOST_FLAG(1);

	// Update file Script.ini on next OS
	CString strGhostDevicePath(_T(""));
	CString strDestinationPath(_T(""));
	CString strDestinationDevicePath(_T(""));
	strDestinationPath = gConfig.GetRootPath();
	strDestinationPath = strDestinationPath.Mid(0, strDestinationPath.Find('\\') + 1);
	GetDevicePathFromUserPath(strDestinationPath, strDestinationDevicePath, MAX_PATH);
	gNextOSScript.DESTINATION_TO_RESTORE_PATH(strDestinationDevicePath);

	GetDevicePathFromUserPath(strGhostFilePath, strGhostDevicePath, MAX_PATH);
	gNextOSScript.GHOST_FILE_PATH(strGhostDevicePath);
	gNextOSScript.TASK_ID(strTaskID);
	gNextOSScript.PREV_OS_FLAG(0);
	gNextOSScript.GHOST_FLAG(1);

	strLog.Format(_T("strDestinationPath=%s,strGhostPath=%s"), strDestinationPath, strGhostFilePath);
	LOGINFO_FUNC(strLog);
	strLog.Format(_T("DESTINATION_TO_RESTORE_PATH=%s,GHOST_FILE_PATH=%s"), strDestinationDevicePath, strGhostDevicePath);
	LOGINFO_FUNC(strLog);
	return TRUE;
}

BOOL CTMSRemoteRestoreApp::UpdateScriptS2(CString &strErr)
{
	CString strLog(_T(""));
	// Convert file path to device path
	CString strNextOSPath = gDrive.BOOT_DRIVE();

	CString strScriptNextOSPath = MakeFullPath(strNextOSPath, TMS_FOLDER_NAME);
	strScriptNextOSPath = MakeFullPath(strScriptNextOSPath, TMS_REMOTE_RESTORE_NAME);

	if (!gNextOSScript.Init(strScriptNextOSPath))
	{
		strErr = _T("Can not init Script.ini on next OS");
		LOGINFO_FUNC(_T("Can not init Script.ini on next OS"));
		return FALSE;
	}
	// Update current OS
	gScript.PREV_OS_FLAG(1);
	gScript.GHOST_FLAG(3);

	// Update file Script.ini on next OS
	gNextOSScript.DESTINATION_TO_RESTORE_PATH(_T(""));
	gNextOSScript.GHOST_FILE_PATH(_T(""));
	gNextOSScript.PREV_OS_FLAG(0);
	gNextOSScript.GHOST_FLAG(3);
	gNextOSScript.TASK_ID(gScript.TASK_ID());
	return TRUE;  

}

BOOL CTMSRemoteRestoreApp::DoGhostAndRestartS2(CString& strErr)
{
	CString strLog(_T(""));
	if (!RunGhost(strErr))
	{
		LOGINFO_FUNC(strErr);
		return FALSE;
	}

	// Restart computer
	if (!RestartComputer())
	{
		strErr = _T("Cannot restart computer");
		LOGINFO_FUNC(_T("Cannot restart computer"));
		return FALSE;
		//ResetFlag();
	}
	else
	{
		// Update flags
		gScript.PREV_OS_FLAG(1);
		gScript.GHOST_FLAG(2);
	}
	return TRUE;
}

BOOL CTMSRemoteRestoreApp::DoCheckAndRestartS2(CString& strErr)
{
	CString strLog(_T(""));
	LOGINFO_FUNC(_T("Step 2: Check next OS, change boot then restart"));
	if (!CheckAndChangeBoot(strErr))
	{
		LOGINFO_FUNC(strErr);
		return FALSE;
	}

	// Restart computer
	if (!RestartComputer())
	{
		strErr = _T("Cannot restart computer");
		LOGINFO_FUNC(_T("Cannot restart computer"));
		return FALSE;
		//ResetFlag();
	}
	else
	{
		// Update flag on script.ini current OS and next OS
		if (!UpdateScriptS2(strErr))
		{
			LOGINFO_FUNC(strErr);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CTMSRemoteRestoreApp::CheckAndChangeBoot(CString &strErr)
{
	// Call check boot script
	CString strLog(_T(""));
	if (!CheckBoot(strErr))
	{
		strLog.Format(_T("Check boot failed"));
		LOGINFO_FUNC(strLog);
		return FALSE;
	}
	else
	{
		if (!gDrive.Init())
		{
			strErr = _T("Check Drive.ini failed");
			LOGINFO_FUNC(_T("Check Drive.ini failed"));
			return FALSE;
		}
	}

	// Check conditions to run ghost on OS2
	CString strNextOSPath = gDrive.BOOT_DRIVE();
	if (!CheckOSEnv(strNextOSPath, strErr))
	{
		strLog.Format(_T("Check next OS env failed"));
		LOGINFO_FUNC(strLog);
		return FALSE;
	}

	// Call change boot script
	if (!ChangeBoot(strErr))
	{
		strLog.Format(_T("Call change boot script failed"));
		LOGINFO_FUNC(strLog);
		return FALSE;
	}
	return TRUE;
}

BOOL CTMSRemoteRestoreApp::CheckBoot(CString &strErr)
{
	CString strBCDEditPath(_T(""));
	CString strDriveFilePath(_T(""));
	CString strParam(_T(""));
	CString strCommand(_T(""));

	strBCDEditPath = MakeFullPath(gConfig.GetRootPath(), _T("bcdedit.exe"));
	strParam = MakeFullPath(gConfig.GetRootPath(), gConfig.FOLDER_SCRIPTS());
	strParam = MakeFullPath(strParam, gConfig.CHECK_BOOT_SCRIPT());
	strDriveFilePath = gDrive.GetFilePath();
	if (strDriveFilePath.IsEmpty())
	{
		strErr = _T("Cannot create file Drive.ini to check boot");
		LOGINFO_FUNC(_T("Cannot create file Drive.ini to check boot"));
		return FALSE;
	}
	if (!PathFileExists(strBCDEditPath))
	{
		strErr = _T("BCDEdit tool does not exist in current folder");
		LOGINFO_FUNC(_T("BCDEdit tool does not exist in current folder"));
		return FALSE;
	}
	strCommand.Format(_T("%s /c %s %s %s"), _T("cmd.exe"), strParam, strBCDEditPath, strDriveFilePath);
	CString sError(_T(""));
	BOOL bEx = Execute(strCommand, gConfig.TIMEOUT_COMMAND(), &sError);
	if (!bEx)
	{
		strErr = _T("Call script to change boot failed");
		LOGINFO_FUNC(_T("Call script to change boot failed"));
		return FALSE;
	}
	return TRUE;
}

BOOL CTMSRemoteRestoreApp::ChangeBoot(CString &strErr)
{
	CString strBCDEditPath(_T(""));
	CString strParam(_T(""));
	CString strCommand(_T(""));

	LOGINFO_FUNC(_T("Begin change boot"));
	strBCDEditPath = MakeFullPath(gConfig.GetRootPath(), _T("bcdedit.exe"));
	strParam = MakeFullPath(gConfig.GetRootPath(), gConfig.FOLDER_SCRIPTS());
	strParam = MakeFullPath(strParam, gConfig.CHANGE_BOOT_SCRIPT());
	strCommand.Format(_T("%s /c %s %s %s"), _T("cmd.exe"), strParam, strBCDEditPath, gDrive.BOOT_DRIVE_ID());
	CString sError(_T(""));
	BOOL bEx = Execute(strCommand, gConfig.TIMEOUT_COMMAND(), &sError);
	if (!bEx)
	{
		strErr = _T("Call script to change boot failed");
		LOGINFO_FUNC(_T("Call script to change boot failed"));
		return FALSE;
	}
	return TRUE;
}

BOOL CTMSRemoteRestoreApp::CheckOSEnv(CString strOSPath, CString &strErr)
{
	// Check TMSfolder
	CString strLog(_T(""));
	CString strTMSPath = MakeFullPath(strOSPath, TMS_FOLDER_NAME);
	strLog.Format(_T("Begin check OS environment: %s"), strTMSPath);
	LOGINFO_FUNC(strLog);
	if (!PathFileExists(strTMSPath))
	{
		strLog.Format(_T("No TMS folder on %s"), strOSPath);
		strErr = strLog;
		LOGINFO_FUNC(strLog);
		return FALSE;
	}

	// Check TMSRemoteRestore folder
	CString strTMSRemoteRestorePath = MakeFullPath(strTMSPath, TMS_REMOTE_RESTORE_NAME);
	if (!PathFileExists(strTMSRemoteRestorePath))
	{
		strLog.Format(_T("No TMSRemoteRestore folder in %s"), strTMSRemoteRestorePath);
		strErr = strLog;
		LOGINFO_FUNC(strLog);
		return FALSE;
	}

	// Check bcdedit.exe
	CString strBCDEditPath = MakeFullPath(strTMSRemoteRestorePath, BCDEDIT_TOOL_NAME);
	if (!PathFileExists(strBCDEditPath))
	{
		strLog.Format(_T("No bcdedit.exe in %s"), strTMSRemoteRestorePath);
		strErr = strLog;
		LOGINFO_FUNC(strLog);
		return FALSE;
	}

	// Check file in TMSRemoteRestore on this OS
	CConfig config;
	if (!config.Init(strTMSRemoteRestorePath))
	{
		strLog.Format(_T("Can't read config.ini in %s"), strTMSRemoteRestorePath);
		strErr = strLog;
		LOGINFO_FUNC(strLog);
		return FALSE;
	}
	// Check norton ghost tool
	CString strGhostToolPath = MakeFullPath(config.GetRootPath(), config.GHOST_TOOL());
	if (!PathFileExists(strGhostToolPath))
	{
		strLog.Format(_T("Can't find norton ghost tool, path: %s"), strGhostToolPath);
		strErr = strLog;
		LOGINFO_FUNC(strLog);
		return FALSE;
	}

	// Check scripts
	CString strScriptsPath = MakeFullPath(strTMSRemoteRestorePath, config.FOLDER_SCRIPTS());
	if (!PathFileExists(strBCDEditPath))
	{
		strLog.Format(_T("No Scripts folder in %s"), strTMSRemoteRestorePath);
		strErr = strLog;
		LOGINFO_FUNC(strLog);
		return FALSE;
	}
	CString strCheckBootScript = MakeFullPath(strScriptsPath, config.CHECK_BOOT_SCRIPT());
	CString strChangeBootScript = MakeFullPath(strScriptsPath, config.CHANGE_BOOT_SCRIPT());
	CString strSetIPScript = MakeFullPath(strScriptsPath, config.SET_IP_SCRIPT());
	CString strRestartScript = MakeFullPath(strScriptsPath, config.SET_IP_SCRIPT());
	if (!PathFileExists(strCheckBootScript) || !PathFileExists(strChangeBootScript) 
		|| !PathFileExists(strRestartScript) || !PathFileExists(strSetIPScript))
	{
		strLog.Format(_T("Not enough script file in %s"), strScriptsPath);
		strErr = strLog;
		LOGINFO_FUNC(strLog);
		return FALSE;
	}
	return TRUE;
}

BOOL CTMSRemoteRestoreApp::RestartComputer()
{
	CString strParam(_T(""));
	CString strCommand(_T(""));
	strParam = MakeFullPath(gConfig.GetRootPath(), gConfig.FOLDER_SCRIPTS());
	strParam = MakeFullPath(strParam, gConfig.RESTART_SCRIPT());
	INT nDelay = gConfig.DELAY_RESTART();

	CString strLog(_T(""));
	strLog.Format(_T("Delay and restart computer. Time delay: %d"), nDelay);
	LOGINFO_FUNC(strLog);
	strCommand.Format(_T("%s /c %s %d"), _T("cmd.exe"), strParam, nDelay);
	CString sError(_T(""));
	BOOL bEx = Execute(strCommand, gConfig.TIMEOUT_COMMAND(), &sError);
	if (!bEx)
	{
		LOGINFO_FUNC(_T("Call script to restart computer failed"));
		return FALSE;
	}
	return TRUE;
}

BOOL CTMSRemoteRestoreApp::RunGhost(CString &strErr)
{
	// Recheck OS env before start ghost
	LOGINFO_FUNC(_T("Run ghost"));
	CString strLog(_T(""));
	if (!CheckOSEnv(_T("C:"), strErr))
	{
		strLog.Format(_T("Check env failed.Error=%s"), strErr);
		LOGINFO_FUNC(strLog);
		return FALSE;
	}
	// Get script.ini
	if (!gScript.Init())
	{
		strErr = _T("Cannot get ghost file and destination information");
		LOGINFO_FUNC(_T("Cannot get ghost file and destination information"));
		return FALSE;
	}
	// Get destination path and ghost file path
	CString strDestinationDevicePath(_T(""));
	CString strGhostFileDevicePath(_T(""));
	strDestinationDevicePath = gScript.DESTINATION_TO_RESTORE_PATH();
	strGhostFileDevicePath = gScript.GHOST_FILE_PATH();
	// Call ghost64.exe to restore
	if (!StartGhostTool(strDestinationDevicePath, strGhostFileDevicePath, strErr))
	{
		LOGINFO_FUNC(_T("Run ghost tool failed"));
		return FALSE;
	}
	LOGINFO_FUNC(_T("Run ghost tool success"));
	return TRUE;
}

BOOL CTMSRemoteRestoreApp::StartGhostTool(CString strDestinationDevicePath, CString strGhostFileDevicePath, CString &strErr)
{
	// Check exist
	LOGINFO_FUNC(_T("Start ghost tool"));
	CString strLog(_T(""));
	CString strDestinationPath(_T(""));
	CString strGhostFilePath(_T(""));
	GetUserPathFromDevicePath(strDestinationDevicePath, strDestinationPath, MAX_PATH);
	GetUserPathFromDevicePath(strGhostFileDevicePath, strGhostFilePath, MAX_PATH);
	strLog.Format(_T("DESTINATION_TO_RESTORE_PATH=%s,GHOST_FILE_PATH=%s"), strDestinationPath, strGhostFilePath);
	LOGINFO_FUNC(strLog);
	if (!strDestinationPath.IsEmpty())
	{
		if (!PathFileExists(strDestinationPath))
		{
			strErr = _T("Cannot find out destination to restore");
			LOGINFO_FUNC(_T("Cannot find out destination to restore"));
			return FALSE;
		}
	}
	if (!PathFileExists(strGhostFilePath))
	{
		strErr = _T("Cannot find out ghost file");
		LOGINFO_FUNC(_T("Cannot find out ghost file"));
		return FALSE;
	}

	// Call ghost tool to restore
	CString strParamScript(_T(""));
	CString strParamGhostTool(_T(""));
	CString strParamGhostPath(_T(""));
	CString strParamDestinationPartition(_T(""));

	CString strCommand(_T(""));
	strParamGhostTool = MakeFullPath(gConfig.GetRootPath(), gConfig.GHOST_TOOL());
	strParamGhostPath = strGhostFilePath;

	INT nPartionNumberOffset = strDestinationDevicePath.Find(HARDDISK_VOLUME_STRING) + CString(HARDDISK_VOLUME_STRING).GetLength();
	strParamDestinationPartition = strDestinationDevicePath.Mid(nPartionNumberOffset, 1);

	strCommand.Format(_T("%s -clone,mode=prestore,src=%s:1,dst=1:%s -sure -batch -fx"), strParamGhostTool, strParamGhostPath, strParamDestinationPartition);
	CString sError(_T(""));
	INT nTimeout = gConfig.TIMEOUT_GHOST_TOOL() * 60 * 1000; // minute->milli second
	BOOL bEx = ExecuteGhostTool(strCommand, nTimeout, &sError);
	if (!bEx)
	{
		strErr = sError;
		return FALSE;
	}
	return TRUE;
}

void CTMSRemoteRestoreApp::DoneRestore()
{
	LOGINFO_FUNC(_T("Step 3: Success"));
	ExportResult(GHOST_STATUS_STRING, gScript.TASK_ID(), GHOST_STATUS_SUCCEED, _T("Step 3: Success"));
	ResetScript();
	return;
}

void CTMSRemoteRestoreApp::ChangeBootFailedS1()
{
	LOGINFO_FUNC(_T("Step 1: Change boot failed after restart"));
	ResetScript();
	ExportResult(GHOST_STATUS_STRING, gScript.TASK_ID(), GHOST_STATUS_FAIL, _T("Step 1: Change boot failed after restart"));
}

void CTMSRemoteRestoreApp::ChangeBootFailedS2()
{
	LOGINFO_FUNC(_T("Step 2: Change boot failed after restart"));
	ExportResult(GHOST_STATUS_STRING, gScript.TASK_ID(), GHOST_STATUS_FAIL, _T("Step 2: Change boot failed after restart"));
	// ResetFlag();
}

void CTMSRemoteRestoreApp::ResetScript()
{
	LOGINFO_FUNC(_T("Reset all fields in Script.ini"));
	gScript.ResetAll();
}

BOOL CTMSRemoteRestoreApp::ExecuteGhostTool(CString command, DWORD timeout, CString* serr /*= NULL*/)
{
	TRY
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		CString sLog;
		sLog.Format(_T("Begin execute: %s"), command);
		LOGINFO(sLog);
		// Start the child process. 
		if (!CreateProcess(NULL,				// No module name (use command line). 
			(LPTSTR)(LPCTSTR)command,
			NULL,				// Process handle not inheritable. 
			NULL,				// Thread handle not inheritable. 
			TRUE,				// Set handle inheritance to TRUE. 	
			//0,                // No creation flags. 
			CREATE_NO_WINDOW,
			NULL,				// Use parent's environment block. 
			NULL,				// Use parent's starting directory. 
			&si,				// Pointer to STARTUPINFO structure.
			&pi))				// Pointer to PROCESS_INFORMATION structure.
		{
			DWORD ec = GetLastError();
			sLog.Format(_T("Cannot execute (%s), error: %i, %s"), command, ec, GetWin32ErrorMessage(ec));
			LOGERROR(sLog, ec);
			if (serr)
				*serr = sLog;
			return FALSE;
		}
		sLog.Format(_T("Timeout command: %i"), timeout);
		LOGINFO(sLog);
		// wait timeout
		if (timeout)
		{
			// Begin thread monitor ghost tool
			m_pi = &pi;
			CWinThread* pThread = AfxBeginThread(MonitorGhostToolThread, this);
			if (pThread)
			{
				LOGINFO(_T("Create monitor ghost tool thread succeed"));
				pThread->m_bAutoDelete = TRUE;

				WaitForSingleObject(pThread->m_hThread, INFINITE);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
				// Get return value from thread
				DWORD dwRetVal = 0;
				GetExitCodeThread(pThread->m_hThread, &dwRetVal);
				sLog.Format(_T("Monitor thread stopped. Return value: %d"), dwRetVal);
				LOGINFO(sLog);

				if (dwRetVal == 1)
				{
					KillProcessByName(gConfig.GHOST_TOOL());
					LOGINFO(_T("Run ghost tool timeout"));
					if (serr)
					{
						*serr = _T("Run ghost tool timeout");
					}
					return FALSE;
				}
				else if (dwRetVal == 2)
				{
					KillProcessByName(gConfig.GHOST_TOOL());
					LOGINFO(_T("Run ghost tool failed"));
					if (serr)
					{
						*serr = _T("Run ghost tool failed");
					}
					return FALSE;
				}
			}
			else
			{
				LOGINFO(_T("Cannot create thread for monitor ghost tool"));
				INT nTimeout = gConfig.TIMEOUT_GHOST_TOOL() * 60 * 1000;
				DWORD dwRet = WaitForSingleObject(pi.hProcess, nTimeout);
				if (dwRet == WAIT_TIMEOUT)
				{
					LOGINFO(_T("Run ghost tool timeout"));
					if (serr)
					{
						*serr = _T("Run ghost tool timeout");
					}
					return FALSE;
				}
				else if (dwRet != WAIT_OBJECT_0)
				{
					LOGINFO(_T("Run ghost tool failed"));
					if (serr)
					{
						*serr = _T("Run ghost tool failed");
					}
					return FALSE;
				}
			}

		}
		sLog.Format(_T("End execute: %s"), command);
		LOGINFO(sLog);
		return TRUE;
	}
	CATCH_ALL(e)
	{
		CString sLog;
		sLog.Format(_T("Exception execute (%s)"), command);
		LOGEXCEPTION(*e, sLog);
		if (serr)
			*serr = sLog;
		return FALSE;
	}
	END_CATCH_ALL;
}

void CTMSRemoteRestoreApp::ExportResult(CString strType, CString strTaskID, INT nStatus, CString strDescription)
{
	CString strExport(_T(""));
	CString strLog(_T(""));
	RLT_DATA_OBJECT resultDataObject;

	CInforStructJSON json;
	CStringA sType = "type";
	json.SetValue(sType, strType);

	resultDataObject.taskID = strTaskID;
	resultDataObject.status = nStatus;
	resultDataObject.desc = strDescription;
	json.SetValue(RLT_DATA_OBJECT::getClassName(), resultDataObject);

	strExport = json.GetJsonDoc();
	strLog.Format(_T("Export data: %s"), strExport);
	LOGINFO_FUNC(strLog);

	CExportMessageStatus exportStatus(m_strExportFolder);
	CString strMsgFile(_T(""));
	if (exportStatus.ExportStatus(STATUS_FILE_TYPE_RLT, strExport, strMsgFile))
	{
		strLog.Format(_T("Export message succeed. Filename: %s"), strMsgFile);
		LOGINFO_FUNC(strLog);
		return;
	}
	else
	{
		strLog.Format(_T("Export message failed. Filename: %s"), strMsgFile);
		LOGINFO_FUNC(strLog);
		return;
	}
}

UINT MonitorGhostToolThread(LPVOID lpParam)
{
	LOGINFO(_T("Begin thread monitor ghost tool"));
	CString strLog(_T(""));
	CTMSRemoteRestoreApp* pApp = (CTMSRemoteRestoreApp*)lpParam;
	CString strTaskID = pApp->gScript.TASK_ID();
	INT nCheckTime = pApp->gConfig.CHECK_GHOST_TOOL_INTERVAL() * 1000; // second -> milli second
	INT nTimeout = pApp->gConfig.TIMEOUT_GHOST_TOOL() * 60 * 1000; // minute -> milli second
	INT nTotalTime = 0;
	DWORD dwRet = 0;
	// Export start ghost tool msg
	pApp->ExportResult(GHOST_STATUS_STRING, strTaskID, GHOST_STATUS_SUCCEED, _T("Ghost tool is started"));
	while (1)
	{
		dwRet = WaitForSingleObject(pApp->m_pi->hProcess, nCheckTime);
		if (dwRet == WAIT_TIMEOUT)
		{
			nTotalTime += nCheckTime;
			if (nTotalTime > nTimeout)
			{
				LOGINFO(_T("Ghost tool is timeout"));
				pApp->ExportResult(GHOST_STATUS_STRING, strTaskID, GHOST_STATUS_FAIL, _T("Ghost tool is timeout"));
				ExitThread(1);
			}
			else
			{
				LOGINFO(_T("Ghost tool is in progress"));
				pApp->ExportResult(GHOST_EVENT_STRING, strTaskID, GHOST_STATUS_SUCCEED, _T("Ghost tool is in progress"));
			}
		}
		else if (dwRet == WAIT_OBJECT_0)
		{
			GetExitCodeProcess(pApp->m_pi->hProcess, &dwRet);
			if (dwRet == 0)
			{
				strLog.Format(_T("Ghost tool run successfully. Exit code:%d"), dwRet);
				LOGINFO(strLog);
				pApp->ExportResult(GHOST_EVENT_STRING, strTaskID, GHOST_STATUS_SUCCEED, _T("Ghost tool run successfully"));
				ExitThread(0);
			}
			else
			{
				strLog.Format(_T("Ghost tool run failed. Exit code:%d"), dwRet);
				LOGINFO(strLog);
				pApp->ExportResult(GHOST_STATUS_STRING, strTaskID, GHOST_STATUS_FAIL, _T("Ghost tool run failed"));
				ExitThread(2);
			}
		}
		else
		{
			LOGINFO(_T("Ghost tool run failed"));
			pApp->ExportResult(GHOST_STATUS_STRING, strTaskID, GHOST_STATUS_FAIL, _T("Ghost tool run failed"));
			ExitThread(2);
		}
	}
}