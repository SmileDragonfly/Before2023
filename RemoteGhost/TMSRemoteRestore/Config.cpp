// Copyright (c) 2015 Microtec. All rights reserved.

#include "StdAfx.h"
#include "Config.h"

const CString CConfig::m_sConfigFileName = CONFIG_FILE_NAME;

CConfig::CConfig(void) 
{
	m_sRootPath = _T("");
	m_sConfigFilepath = _T("");
}

CConfig::~CConfig(void){}

/*****************************************************************
* Function:		Init
* Description:	find DailyBackupConfig.ini in the same folder of DailyBackup.exe
* Input values: 
* Output value:
******************************************************************/
BOOL CConfig::Init()
{
	TRY
	{
		//=================================================================================
		// Find config file
		CString sLog(_T(""));
		CString	strPath;
		GetModuleFileName(NULL, strPath.GetBuffer(_MAX_PATH), _MAX_PATH);
		strPath.ReleaseBuffer();
		int index = strPath.ReverseFind('\\');
		if (index <= 0)
		{
			sLog.Format(_T("Scanner cannot start because config path folder invalid: %s"), strPath);
			LogEvent(sLog);
			return FALSE;
		}

		// set root path
		m_sRootPath = strPath.Mid(0, index);
		if (m_sRootPath.IsEmpty() || !PathIsDirectory(m_sRootPath))
		{
			sLog.Format(_T("Root folder path is not directory. Quit."));
			LogEvent(sLog);
			return FALSE;
		}

		m_sConfigFilepath = MakeFullPath(m_sRootPath, m_sConfigFileName);
		if (m_sConfigFilepath.IsEmpty())
		{
			sLog.Format(_T("Config file path is empty. Quit."));
			LogEvent(sLog);
			return FALSE;
		}
		
		// Start logging
		Get_PATH_LOG();
		if (m_PATH_LOG.IsEmpty())
			PATH_LOG(m_sRootPath);
		if (!CreateTreeFolder(PATH_LOG()))
		{
			sLog.Format(_T("Cannot create log folder. Quit."));
			LogEvent(sLog);
			return FALSE;
		}
		// Start logging
		if (!InitLog(PATH_LOG()))
		{
			LogEvent(_T("Cannot init agent because init log object failed!"));
			return FALSE;
		}

		sLog.Format(_T("Config file path: %s"), m_sConfigFilepath);
		LOGINFO(sLog);

		//=================================================================================
		// Load config of [PATH]
		Get_PATH_WORKING();
		if (m_PATH_WORKING.IsEmpty())
			PATH_WORKING(MakeFullPath(m_sRootPath, _T("Working")));
		if (!CreateTreeFolder(m_PATH_WORKING))
		{
			LOGERROR(_T("Cannot create report folder"));
			return FALSE;
		}
		sLog.Format(_T("Path working: %s"), m_PATH_WORKING);
		LOGINFO(sLog);

		Get_GHOST_TOOL();
		if (m_GHOST_TOOL.IsEmpty())
			m_GHOST_TOOL = _T("Ghost64.exe");

		//=================================================================================
		// Load config of [SCRIPTS]
		Get_FOLDER_SCRIPTS();
		Get_CHECK_BOOT_SCRIPT();
		Get_CHANGE_BOOT_SCRIPT();
		Get_RESTORE_SCRIPT();
		Get_RESTART_SCRIPT();
		Get_SET_IP_SCRIPT();
		Get_TIMEOUT_COMMAND();
		Get_DELAY_RESTART();

		//=================================================================================
		// Load config of [GHOSTTOOL]
		Get_TIMEOUT_GHOST_TOOL();
		Get_CHECK_GHOST_TOOL_INTERVAL();

		return TRUE;
	}
	CATCH_ALL(e)
	{
		LOGERROR(_T("Exception"));
		return FALSE;
	}
	END_CATCH_ALL;
}

BOOL CConfig::Init(CString strRootPath)
{
	TRY
	{
		CString sLog(_T(""));
		// set root path
		m_sRootPath = strRootPath;
		if (m_sRootPath.IsEmpty() || !PathIsDirectory(m_sRootPath))
		{
			sLog.Format(_T("Root folder path is not directory. Quit."));
			LogEvent(sLog);
			return FALSE;
		}

		m_sConfigFilepath = MakeFullPath(m_sRootPath, m_sConfigFileName);
		if (m_sConfigFilepath.IsEmpty())
		{
			sLog.Format(_T("Config file path is empty. Quit."));
			LogEvent(sLog);
			return FALSE;
		}

		//=================================================================================
		// Load config of [PATH]
		Get_PATH_LOG();
		Get_PATH_WORKING();
		Get_GHOST_TOOL();
		if (m_GHOST_TOOL.IsEmpty())
			m_GHOST_TOOL = _T("Ghost64.exe");

		//=================================================================================
		// Load config of [SCRIPTS]
		Get_FOLDER_SCRIPTS();
		Get_CHECK_BOOT_SCRIPT();
		Get_CHANGE_BOOT_SCRIPT();
		Get_RESTORE_SCRIPT();
		Get_RESTART_SCRIPT();
		Get_SET_IP_SCRIPT();
		Get_TIMEOUT_COMMAND();
		Get_DELAY_RESTART();

		return TRUE;
	}
		CATCH_ALL(e)
	{
		LOGERROR(_T("Exception"));
		return FALSE;
	}
	END_CATCH_ALL;
}

BOOL CConfig::InitLog(CString pathLog)
{
	CLog::Default = CLog::New();
	if (!CLog::Default)
	{
		LogEvent(_T("Cannot create Log object"));
		return FALSE;
	}

	CLog::Default->SetDirectoryLog(pathLog);
	CLog::Default->SetDaySaveLog(90);
	CLog::Default->SetLogNamePrefix(_T("TMSRemoteGhost"));

	LOGINFO(_T("Initializing Logger..."));

	return TRUE;
}

/*****************************************************************
* Function:		Read
* Description:	read data in ini file
* Input values: 
* Output value:
******************************************************************/
CString CConfig::Read(CString header, CString tag, CString _default)
{	
	return ReadINI(m_sConfigFilepath, header, tag, _default);
}

/*****************************************************************
* Function:		ReadINI
* Description:	implement API GetPrivateProfileString() for read data of ini file
* Input values: 
* Output value:
******************************************************************/
CString CConfig::ReadINI(CString strIniPath, CString strHeader, CString strTag, CString strDefault /*= _T("")*/ )
{
	TRY
	{
		if (strIniPath.IsEmpty()) 
			return _T("");

		CByteArray data;
		data.SetSize(BUFFER_SIZE);
		memset(data.GetData(), 0, BUFFER_SIZE);
		
		DWORD nr = 0;
		nr = GetPrivateProfileString(strHeader,
									 strTag,
									 strDefault,
									 (TCHAR*) data.GetData(),
									 BUFFER_SIZE,
									 strIniPath);

		CString temp = CString((TCHAR*) data.GetData());
		return temp;
	}
	CATCH_ALL(e)
	{
		return _T("");
	}
	END_CATCH_ALL;
}

/*****************************************************************
* Function:		Write
* Description:	write config data to ini file (automation)
* Input values: 
* Output value:
******************************************************************/
BOOL CConfig::Write(CString header, CString tag, CString value)
{
	if (m_sConfigFilepath.IsEmpty()) 
		return FALSE;

	return WritePrivateProfileString(header, tag, value, m_sConfigFilepath);
}

/*****************************************************************
* Function:		GetExeFilePath
* Description:	return full file path of DailyBackup.exe 
* Input values: 
* Output value:
******************************************************************/
CString CConfig::GetRootPath()
{
	return m_sRootPath;
}
