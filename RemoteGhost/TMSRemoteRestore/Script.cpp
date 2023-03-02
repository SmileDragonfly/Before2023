// Copyright (c) 2015 Microtec. All rights reserved.

#include "StdAfx.h"
#include "Script.h"

const CString CScript::m_sScriptFileName = SCRIPT_FILE_NAME;

CScript::CScript(void)
{
	m_sRootPath = _T("");
	m_sScriptFilepath = _T("");
	m_bInited = FALSE;
}

CScript::~CScript(void){}

/*****************************************************************
* Function:		Init
* Description:	find DailyBackupConfig.ini in the same folder of DailyBackup.exe
* Input values: 
* Output value:
******************************************************************/
BOOL CScript::Init()
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

		m_sScriptFilepath = MakeFullPath(m_sRootPath, m_sScriptFileName);
		if (m_sScriptFilepath.IsEmpty())
		{
			sLog.Format(_T("Script file path is empty. Quit."));
			LogEvent(sLog);
			return FALSE;
		}
		
		Get_TASK_ID();
		sLog.Format(_T("TASK_ID = %s"), m_TASK_ID);
		LOGINFO(sLog);

		Get_DESTINATION_TO_RESTORE_PATH();
		sLog.Format(_T("DESTINATION_TO_RESTORE_PATH = %s"), m_DESTINATION_TO_RESTORE_PATH);
		LOGINFO(sLog);

		Get_GHOST_FILE_PATH();
		sLog.Format(_T("GHOST_FILE_PATH = %s"), m_GHOST_FILE_PATH);
		LOGINFO(sLog);

		Get_PREV_OS_FLAG();
		sLog.Format(_T("PREV_OS_FLAG = %d"), m_PREV_OS_FLAG);
		LOGINFO(sLog);

		Get_GHOST_FLAG();
		sLog.Format(_T("GHOST_FLAG = %d"), m_GHOST_FLAG);
		LOGINFO(sLog);

		m_bInited = TRUE;
		return TRUE;
	}
	CATCH_ALL(e)
	{
		LOGERROR(_T("Exception"));
		return FALSE;
	}
	END_CATCH_ALL;
}

BOOL CScript::Init(CString strRootPath)
{
	TRY
	{
		// set root path
		CString sLog(_T(""));
		m_sRootPath = strRootPath;
		if (m_sRootPath.IsEmpty() || !PathIsDirectory(m_sRootPath))
		{
			sLog.Format(_T("Root folder path is not directory. Quit."));
			LogEvent(sLog);
			return FALSE;
		}

		m_sScriptFilepath = MakeFullPath(m_sRootPath, m_sScriptFileName);
		if (m_sScriptFilepath.IsEmpty())
		{
			sLog.Format(_T("Script file path is empty. Quit."));
			LogEvent(sLog);
			return FALSE;
		}

		Get_DESTINATION_TO_RESTORE_PATH();
		sLog.Format(_T("DESTINATION_TO_RESTORE_PATH = %s"), m_DESTINATION_TO_RESTORE_PATH);
		LOGINFO(sLog);

		Get_GHOST_FILE_PATH();
		sLog.Format(_T("GHOST_FILE_PATH = %s"), m_GHOST_FILE_PATH);
		LOGINFO(sLog);

		Get_TASK_ID();
		sLog.Format(_T("TASK_ID = %s"), m_TASK_ID);
		LOGINFO(sLog);

		Get_PREV_OS_FLAG();
		sLog.Format(_T("PREV_OS_FLAG = %d"), m_PREV_OS_FLAG);
		LOGINFO(sLog);

		Get_GHOST_FLAG();
		sLog.Format(_T("GHOST_FLAG = %d"), m_GHOST_FLAG);
		LOGINFO(sLog);

		m_bInited = TRUE;
		return TRUE;
	}
	CATCH_ALL(e)
	{
		LOGERROR(_T("Exception"));
		return FALSE;
	}
	END_CATCH_ALL;
}

/*****************************************************************
* Function:		Read
* Description:	read data in ini file
* Input values: 
* Output value:
******************************************************************/
CString CScript::Read(CString header, CString tag, CString _default)
{	
	return ReadINI(m_sScriptFilepath, header, tag, _default);
}

/*****************************************************************
* Function:		ReadINI
* Description:	implement API GetPrivateProfileString() for read data of ini file
* Input values: 
* Output value:
******************************************************************/
CString CScript::ReadINI(CString strIniPath, CString strHeader, CString strTag, CString strDefault /*= _T("")*/)
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
BOOL CScript::Write(CString header, CString tag, CString value)
{
	if (m_sScriptFilepath.IsEmpty()) 
		return FALSE;

	return WritePrivateProfileString(header, tag, value, m_sScriptFilepath);
}

/*****************************************************************
* Function:		GetExeFilePath
* Description:	return full file path of DailyBackup.exe 
* Input values: 
* Output value:
******************************************************************/
CString CScript::GetRootPath()
{
	return m_sRootPath;
}

void CScript::ResetFlag()
{
	PREV_OS_FLAG(0);
	GHOST_FLAG(0);
}

void CScript::ResetAll()
{
	DESTINATION_TO_RESTORE_PATH(_T(""));
	GHOST_FILE_PATH(_T(""));
	TASK_ID(_T(""));
	PREV_OS_FLAG(0);
	GHOST_FLAG(0);
}

BOOL CScript::IsInit()
{
	return m_bInited;
}
