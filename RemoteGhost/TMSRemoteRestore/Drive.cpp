// Copyright (c) 2015 Microtec. All rights reserved.

#include "StdAfx.h"
#include "Drive.h"

const CString CDrive::m_sDriveFileName = DRIVE_FILE_NAME;

CDrive::CDrive(void)
{
	m_sRootPath = _T("");
	m_sDriveFilePath = _T("");
}

CDrive::~CDrive(void){}

/*****************************************************************
* Function:		Init
* Description:	find DailyBackupConfig.ini in the same folder of DailyBackup.exe
* Input values: 
* Output value:
******************************************************************/
BOOL CDrive::Init()
{
	TRY
	{
		//=================================================================================
		// Find config file
		CString sLog(_T(""));
		if (GetFilePath().IsEmpty())
		{
			return FALSE;
		}
		Get_BOOT_DRIVE();
		sLog.Format(_T("BOOT_DRIVE = %s"), m_BOOT_DRIVE);
		LOGINFO(sLog);

		Get_BOOT_DRIVE_ID();
		sLog.Format(_T("BOOT_DRIVE_ID = %s"), m_BOOT_DRIVE_ID);
		LOGINFO(sLog);

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
CString CDrive::Read(CString header, CString tag, CString _default)
{	
	return ReadINI(m_sDriveFilePath, header, tag, _default);
}

/*****************************************************************
* Function:		ReadINI
* Description:	implement API GetPrivateProfileString() for read data of ini file
* Input values: 
* Output value:
******************************************************************/
CString CDrive::ReadINI(CString strIniPath, CString strHeader, CString strTag, CString strDefault /*= _T("")*/)
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
BOOL CDrive::Write(CString header, CString tag, CString value)
{
	if (m_sDriveFilePath.IsEmpty()) 
		return FALSE;

	return WritePrivateProfileString(header, tag, value, m_sDriveFilePath);
}

/*****************************************************************
* Function:		GetExeFilePath
* Description:	return full file path of DailyBackup.exe 
* Input values: 
* Output value:
******************************************************************/
CString CDrive::GetRootPath()
{
	return m_sRootPath;
}

CString CDrive::GetFilePath()
{
	CString sLog(_T(""));
	if (m_sDriveFilePath.IsEmpty())
	{
		CString	strPath;
		GetModuleFileName(NULL, strPath.GetBuffer(_MAX_PATH), _MAX_PATH);
		strPath.ReleaseBuffer();
		int index = strPath.ReverseFind('\\');
		if (index <= 0)
		{
			sLog.Format(_T("Scanner cannot start because config path folder invalid: %s"), strPath);
			LogEvent(sLog);
		}

		// set root path
		m_sRootPath = strPath.Mid(0, index);
		if (m_sRootPath.IsEmpty() || !PathIsDirectory(m_sRootPath))
		{
			sLog.Format(_T("Root folder path is not directory. Quit."));
			LogEvent(sLog);
		}

		m_sDriveFilePath = MakeFullPath(m_sRootPath, m_sDriveFileName);
		if (m_sDriveFilePath.IsEmpty())
		{
			sLog.Format(_T("Script file path is empty. Quit."));
			LogEvent(sLog);
		}
	}
	return m_sDriveFilePath;
}
