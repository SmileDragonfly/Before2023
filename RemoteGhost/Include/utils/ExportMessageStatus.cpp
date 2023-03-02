#include "stdafx.h"
#include "ExportMessageStatus.h"

/************************************************************************/
/*
	Message		Source				Frequency/TRXN		Frequency/day (or 200 TRXN)
	-------------------------------------------------------------------------------- 
	 MSG		TMSStatus				5					1000
	 ECA		TMSStatus				2					400
	 TIP		TMSStatus				--					0-1
	 APP		TMSStatus				--					1440
	 EJM		TMSStatus				1					200
	 DCA		TMSDeviceMonitor		1					200
	 PRM		TMSProfileMonitor		--					24	(per Hour)		
	 EJD		TMSStreaming			1					200
	 CPK		TMSCPK					--					0-1
	 GDK		TMSGDK					--					0-1
	 ECP		ECP						--					0-1
	 --------------------------------------------------------------------------------
	 TOTAL												  ~3468
*/
/************************************************************************/


static CString gMessageType[16] = {
	_T("MSG"),
	_T("ECA"),
	_T("TIP"),
	_T("APP"),
	_T("EJM"),
	_T("DCA"),
	_T("PRM"),
	_T("EJD"),
	_T("CPK"),
	_T("GDK"),
	_T("ECP"),
	_T("ISO"),
	_T("GRG"),
	_T("VER"),
	_T("DVS"),
	_T("RLT")
};

CExportMessageStatus::CExportMessageStatus()
{
}

CExportMessageStatus::CExportMessageStatus(CString strMsgFolder)
{
	m_strMessageFolder = strMsgFolder;
}


CExportMessageStatus::~CExportMessageStatus()
{
}

void CExportMessageStatus::SetMessageFolder(CString strMsgFolder)
{
	m_strMessageFolder = strMsgFolder;
}

// create and write file
BOOL CExportMessageStatus::ExportStatus(int nMessageType, CStringA szContent, CString &sMessageFile)
{
	sMessageFile = _T("");

	// get new file name
	CString strFileName(_T("")), strFilePath(_T(""));
	CreateSubFolder(nMessageType, strFilePath, strFileName);	

	// update return value
	sMessageFile = MakeFullPath(strFilePath, strFileName);

	// write file
	CFile file;
	CFileException e;
	if (!file.Open(sMessageFile, CFile::modeCreate | CFile::modeWrite, &e))
	{
		CString sLog(_T(""));
		DWORD err = GetLastError();
		sLog.Format(_T("WatcherConfig: Data file could not be opened: %s. Last_error=%s"), sMessageFile, GetWin32ErrorMessage(err));
		LOGERROR(sLog);
		return FALSE;
	}

	file.Write(szContent, szContent.GetLength());
	file.Flush();
	file.Close();
	
	return TRUE;
}

BOOL CExportMessageStatus::ExportStatus(int nMessageType, CString strContent, CString &sMessageFile)
{
	sMessageFile = _T("");
	CStringA szContent;
	szContent = CT2A(strContent);
	// get new file name
	CString strFileName(_T("")), strFilePath(_T(""));
	CreateSubFolder(nMessageType, strFilePath, strFileName);

	// update return value
	sMessageFile = MakeFullPath(strFilePath, strFileName);

	// write file
	CFile file;
	CFileException e;
	if (!file.Open(sMessageFile, CFile::modeCreate | CFile::modeWrite, &e))
	{
		CString sLog(_T(""));
		DWORD err = GetLastError();
		sLog.Format(_T("WatcherConfig: Data file could not be opened: %s. Last_error=%s"), sMessageFile, GetWin32ErrorMessage(err));
		LOGERROR(sLog);
		return FALSE;
	}

	file.Write(szContent, szContent.GetLength());
	file.Flush();
	file.Close();

	return TRUE;
}

void CExportMessageStatus::CreateSubFolder(int nMessageType, CString &subfolder, CString &filename)
{
	SYSTEMTIME currTime;
	::GetLocalTime(&currTime);
	
	// create export sub folder
	CString tempPath(_T("")), sSubPath(_T(""));
	CString sFileName(_T("")), sMessageType(_T(""));

	sSubPath.Format(_T("%.4i\\%.2i\\%.2i\\%.2i"), currTime.wYear, currTime.wMonth, currTime.wDay, currTime.wHour);
	tempPath = MakeFullPath(m_strMessageFolder, sSubPath);

	// Create tree folder
	// Retry 10 times
	CString sLog(_T(""));
	int nRetry = 0;
	while (nRetry < 10)
	{
		if (!CreateTreeFolder(tempPath))
		{
			sLog.Format(_T("Watcher: cannot create sub tree folder: %s. Retry=%d"), tempPath, nRetry);
			LOGINFO(sLog);
		}
		else
			break;
		nRetry++;
	}

	// get message type string
	if (nMessageType < TOTAL_MESSAGE_TYPE)
		sMessageType = gMessageType[nMessageType];
	else
		sMessageType = MESSAGE_TYPE_UNKNOW;

	// create file name
	if (nRetry < 10)
	{
		sFileName.Format(_T("%.2i_%.2i_%.3i_%s"), currTime.wMinute, currTime.wSecond, currTime.wMilliseconds, sMessageType);
	}
	else
	{
		tempPath = m_strMessageFolder;
		sFileName.Format(_T("%.4i_%.2i_%.2i_%.2i_%.2i_%.2i_%.3i_%s"),  
			currTime.wYear, currTime.wMonth, currTime.wDay, currTime.wHour,
			currTime.wMinute, currTime.wSecond, currTime.wMilliseconds, sMessageType);
	}

	// return value
	subfolder = tempPath;
	filename = sFileName;
}

void CExportMessageStatus::RemoveSubFolderEmpty(CString sourcePath, CString currentFolder)
{
	CString sLog(_T(""));

	WIN32_FIND_DATA fileInfo;
	HANDLE hFind;

	CString sPath(_T(""));
	CString source = _T("");

	source = sourcePath + _T("\\*.*");
	hFind = FindFirstFile(source, &fileInfo);
	if (hFind == INVALID_HANDLE_VALUE)
		return;

	do
	{
		// check dot, directory
		if (_tcsicmp(fileInfo.cFileName, _T(".")) == 0 ||
			_tcsicmp(fileInfo.cFileName, _T("..")) == 0)
			continue;

		if (fileInfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			sPath = MakeFullPath(sourcePath, fileInfo.cFileName);

			// thu muc chuan bi duoc tao, neu da ton tai thi bo qua
			if (sPath.CompareNoCase(currentFolder) == 0)
			{
				continue;
			}

			// check and remove empty directory
			if (PathIsDirectoryEmpty(sPath))
			{
				sLog.Format(_T("Watcher: RemoveEmptyFolder remove directory empty: %s"), sPath);
				LOGINFO(sLog);

				if (!RemoveDirectory(sPath))
				{
					DWORD errorCode = GetLastError();
					sLog.Format(_T("Watcher: RemoveEmptyFolder cannot remove directory: %s. Last_error=%s"), sPath, GetWin32ErrorMessage(errorCode));
					LOGERROR(sLog);
				}
			}
			else
			{
				// loop check sub folder
				RemoveSubFolderEmpty(sPath, currentFolder);

				// recheck source path
				if (PathIsDirectoryEmpty(sPath))
				{
					sLog.Format(_T("Watcher: RemoveEmptyFolder remove directory empty: %s"), sPath);
					LOGINFO(sLog);

					if (!RemoveDirectory(sPath))
					{
						DWORD errorCode = GetLastError();
						sLog.Format(_T("Watcher: RemoveEmptyFolder cannot remove directory: %s. Last_error=%s"), sPath, GetWin32ErrorMessage(errorCode));
						LOGERROR(sLog);
					}
				}
			}
		}
	} while (FindNextFile(hFind, &fileInfo) != 0);
	FindClose(hFind);
}