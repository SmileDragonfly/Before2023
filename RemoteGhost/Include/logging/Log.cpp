#include "stdafx.h"
#include "Log.h"

//#define SYM_LOG			WM_USER + 20
#define ___TRY_LOG		5
// #define ___OneMB		1024*1024
#define ___DAY_SAVE_LOG	60
#define ____TIMEOUT_LOG 60000

#ifdef ____DEBUG_LOGING
void levent(LPCTSTR msg)
{
	//pszFormat
	//static TCHAR chMsg[4086];
	//static TCHAR ServiceName[] = _T("TMS Agent Debug Log\0");
	HANDLE hEventSource;
	LPTSTR lpszStrings[1];

	lpszStrings[0] = (LPTSTR) msg;

	/* Get a handle to use with ReportEvent(). */
	hEventSource = RegisterEventSource(NULL, g_LogEventServiceName);
	if (hEventSource != NULL)
	{
		/* Write to event log. */
		//ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
		ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
		DeregisterEventSource(hEventSource);
	}
}
#else
#define levent(msg)
#endif

static CString m_months[13] = {L"", L"Jan", L"Feb", L"Mar", L"Apr", L"May", L"Jun", L"Jul", L"Aug", L"Sep", L"Oct", L"Nov", L"Dec"};


// [2/26/2018 HieuTT] 
// Default objects
CLog *CLog::Default = NULL;
CLog *CLog::CaptureLog = NULL;
CLog *CLog::DispenserLog = NULL;

// Create File Log camera [1/2/2020 VuongDV]
CLog *CLog::CameraLog = NULL;
// [1/2/2020 VuongDV]

CLog *CLog::WhiteListLog = NULL;

IMPLEMENT_DYNCREATE(CLog, CWinThread)

CLog::CLog(CString path, DWORD maxlengthKB)
{
	m_nameLog.Empty();
	m_nameLogPrefix.Empty();
	m_path		= path;
	m_maxlength	= maxlengthKB * 1024;

	m_inited	= FALSE;	
	m_error		= 0;
	m_logCounter= 0;

	m_nDaysSaveLog = ___DAY_SAVE_LOG;
	m_nEventLogCounter = 0;
}

CLog::~CLog()
{
	if (m_file.m_hFile != CFile::hFileNull)
		m_file.Close();
}

BOOL CLog::InitInstance()
{	
// 	int itry = ___TRY_LOG;
// 	while (itry > 0)
// 	{
// 		if (PostThreadMessage(SYM_LOG, 0, 0))
// 			break;
// 		itry--;
// 	}
	return TRUE;
}

int CLog::ExitInstance()
{	
	return CWinThread::ExitInstance();
}

CLog* CLog::New()
{
	TRY
	{
		CLog* pobj = dynamic_cast<CLog*> (AfxBeginThread(RUNTIME_CLASS(CLog), 0, 0, 0, NULL));
		if (pobj)
			return pobj;
		else
			return NULL;
	}
	CATCH_ALL(e)
	{		
		return NULL;
	}
	END_CATCH_ALL;
	return NULL;
}

BEGIN_MESSAGE_MAP(CLog, CWinThread)
END_MESSAGE_MAP()

// BOOL CLog::PreTranslateMessage(MSG* pMsg)
// {
// 	switch (pMsg->message)
// 	{
// 	case SYM_LOG:
// 		{
// 			// write log
// //			FlushCurrentQueue();
// 
// // 			int itry = ___TRY_LOG;
// // 			while (itry > 0)
// // 			{
// // 				if (PostThreadMessage(SYM_LOG, 0, 0))
// // 					break;
// // 				itry--;
// // 			}
// 
// 			while (1)
// 			{
// 				FlushCurrentQueue();
// 			}
// 
// 			return TRUE;
// 		}
// 	default:
// 		return CWinThread::PreTranslateMessage(pMsg);
// 	}	
// }

int CLog::Run()
{
	while (1)
	{
		FlushCurrentQueue();
	}

	return TRUE;
}

void CLog::WriteEventLog(CString msg)
{
	if (m_nEventLogCounter > 1)
		return;
	
	CString sLog;
	DWORD err = GetLastError();
	sLog.Format(_T("%s: %s\r\n\t\t\tCode: %X"), 
		msg,
		GetWin32ErrorMessage(err), 
		err);
	LogEvent(sLog);
	Sleep(2000);

	m_nEventLogCounter ++;
}

void CLog::FlushCurrentQueue()
{
	//levent(_T("FlushCurrentQueue Trace 1"));
	TRY
	{
		if (!m_inited)
		{
			if (!Init())
			{
// 				if (!m_error)
// 				{
// 					WriteEventLog(_T("Error init log file"));
// 				}
				return;
			}
		}

		// Check log date for switch log file[10/15/2014 AnhNT2]
		CTime Now = CTime::GetCurrentTime();
		if (m_strCurrentLogDate.CompareNoCase(Now.Format(_T("%Y%m%d"))) != 0)
		{
			if (!Init())
			{
// 				if (!m_error)
// 				{
// 					WriteEventLog(_T("Error init log file"));
// 				}
				return;
			}
			RemoveOldLogFile(m_nDaysSaveLog, m_path, m_nameLogPrefix);
		}
		else if ((m_logCounter % 100)== 0) //  [10/15/2014 AnhNT2]
		{
			if (FullLogFile())
			{
				if (!Init())
				{
// 					if (!m_error)
// 					{
// 						WriteEventLog(_T("Error init log file"));
// 					}
					return;
				}
				//RemoveOldLogFile(m_nDaysSaveLog, m_path);
			}
		}

		//levent(_T("FlushCurrentQueue Trace 2"));
		TRY
		{
// 			CString msg(_T(""));
// 			CStringA nsa("");
// 			for (int i=0; i<10; i++)
// 			{
// 				m_logCounter++;
// 
// 				mq_.wait_and_pop(msg);
// 				nsa = CStringA(msg);
// 
// 				//m_file.SeekToEnd();				
// 				m_file.Write(nsa.GetBuffer(), nsa.GetLength()*sizeof(CHAR));
				//m_file.Flush();
//			}
			
			
			m_logCounter++;
			
			std::string msgs("");
			if (mq_.wait_and_pop(msgs))
				m_file.Write(msgs.c_str(), (int)msgs.length()*sizeof(CHAR));

			//levent(_T("FlushCurrentQueue Trace 3"));		
		}
		CATCH_ALL(pe)
		{
			CFileException *fe = (CFileException*)pe;
			switch (fe->m_cause)
			{
			case CFileException::fileNotFound:
				if (!Init())
				{
					if (!m_error)
					{
						WriteEventLog(_T("Error while retry write log after exception"));
					}
					return;
				}		
						
				CString ns;
				CStringA nsa;
				nsa = CStringA(ns);

				m_file.SeekToEnd();
				m_file.Write(nsa.GetBuffer(), nsa.GetLength()*sizeof(CHAR));
				m_file.Flush();

				return;
			}

			WriteEventLog(_T("Exception write log"));
			//Sleep(2000);

			return;	
		}
		END_CATCH_ALL;
	}
	CATCH_ALL(pe)
	{
		WriteEventLog(_T("Exception while writing log message"));
		//Sleep(2000);

		return;	
	}
	END_CATCH_ALL;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
BOOL CLog::Init()
{
	CSingleLock lock(&m_mutex);
	if (lock.Lock(____TIMEOUT_LOG))
	{
		//  [2/26/2018 HieuTT]
		if (m_nameLogPrefix.IsEmpty())
			return FALSE;

		m_inited = FALSE;
		int itry = ___TRY_LOG;
		while (itry)
		{
			if (SwitchNewLogFile())
				break;
			if (m_error)
				return FALSE;
			Sleep(2000);
			itry--;
		}
		if (!itry)
			return FALSE;
		m_inited = TRUE;
		return TRUE;
	}

    return FALSE;
}

void CLog::SetDirectoryLog(CString pathlog)
{
	m_path	 = pathlog;
	m_inited = FALSE;
}

void CLog::SetDaySaveLog(int days)
{
	m_nDaysSaveLog = days;
}

void CLog::SetLogNamePrefix(CString logNamePrefix)
{
	if (logNamePrefix.IsEmpty())
		m_nameLogPrefix = _T("TMS");
	else
		m_nameLogPrefix = logNamePrefix;
}

BOOL CLog::FullLogFile()
{
	if (!m_inited)
		return FALSE;
	if (m_file.GetLength() > m_maxlength)
		return TRUE;
	return FALSE;
}

CString CLog::GetCurrentDateTimeString()
{
	CTime t = CTime::GetCurrentTime();
	CString txt = t.Format(_T("%Y%m%d_%H%M%S"));
	return txt;
}

CString CLog::GetCurrentDateTimeString(CTime& Now)
{
	CString txt = Now.Format(_T("%Y%m%d_%H%M%S"));
	return txt;
}

CString CLog::GetNewLogName(BOOL nestFolder /*= TRUE*/ )
{
	CString strNewLogName(_T(""));
	CString strNameOnly(_T(""));

	CTime Now = CTime::GetCurrentTime();
	srand((unsigned)time(NULL));
	
	m_strCurrentLogDate = Now.Format(_T("%Y%m%d"));

	// check file exist
	SYSTEMTIME currTime;
	::GetLocalTime(&currTime);
	strNameOnly.Format(_T("%s%.4i%s%.2i"), m_nameLogPrefix, currTime.wYear, m_months[currTime.wMonth], currTime.wDay);
	strNewLogName = strNameOnly + _T(".log");
	CString currentLogPath = MakeFullPath(m_path, strNewLogName);
	if (PathFileExists(currentLogPath))
	{
		CFileException e;
		CFile file;
		if (file.Open(currentLogPath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite | CFile::modeNoTruncate, &e))
		{
			// check full log file
			if (file.GetLength() < m_maxlength)
			{
				file.Close();
				return strNewLogName;
			}
			else
			{
				file.Close();

				// rename current log file
				// continue return log file name by day
				if (!RenameFullLogFile(strNameOnly))
				{
					// cannot rename
					// truncate file ?
					WriteEventLog(_T("Cannot rename current full log file"));
				}
			}			
		}
		else
		{
			levent(_T("Cannot open current log file to check file size. Create new log file."));
		}
	}

	return strNewLogName;
}
BOOL CLog::RenameFullLogFile(CString strNameOnly)
{
	// if file exist
	// find all file same pattern: DCA2019Feb14_*.log
	CString sLog(_T(""));

	CString strLogFilePattern(_T(""));
	strLogFilePattern.Format(_T("%s_*.log"), strNameOnly);

	CFileFind finder;
	CString search = MakeFullPath(m_path, strLogFilePattern);
	BOOL bWorking = finder.FindFile(search);

	int nRetry = 0, nMax = 0;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
			continue;

		CString sFilename = finder.GetFileName();
		sFilename.TrimLeft(strNameOnly);
		sFilename.TrimLeft(_T("_"));
		sFilename.TrimRight(_T(".log"));

		nRetry =  _ttoi((LPCTSTR)sFilename);
		if (nRetry > nMax)
		{
			nMax = nRetry;
		}
	}
	finder.Close();

	// Rename log file: DCA2019Feb14.log -> DCA2019Feb14_x.log
	CString strCurrentLogName(_T(""));
	CString strNewLogName(_T(""));

	strCurrentLogName.Format(_T("%s.log"), strNameOnly);
	strNewLogName.Format(_T("%s_%d.log"), strNameOnly, nMax+1);
	
	CString fullold(_T(""));
	CString fullnew(_T(""));

	fullold = MakeFullPath(m_path, strCurrentLogName);
	fullnew = MakeFullPath(m_path, strNewLogName);

	int nMaxRetry = 5;
	int itry = 1;
	BOOL done = FALSE;
	while (itry <= nMaxRetry)
	{
		if (MoveFile(fullold, fullnew))
		{
			done = TRUE;
			break;
		}
		else
		{
			Sleep(1000);
		}
		itry++;
	}		

	if (!done)
		return FALSE;
	return TRUE;
}

BOOL CLog::SwitchNewLogFile()
{
	if (m_file.m_hFile != CFile::hFileNull)
		m_file.Close();

	m_nameLog = MakeFullPath(m_path, GetNewLogName());

	CFileException e;
	if(!m_file.Open(m_nameLog, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite | CFile::modeNoTruncate, &e))
	{
		m_error = (DWORD) e.m_cause;
		
 		CString msg;
		msg.Format(_T("Log file could not be opened: %s. File Exception Cause: %d"), m_nameLog, m_error);
		WriteEventLog(msg);

		return FALSE;
	}
	m_file.Seek(0L, CFile::end);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
void CLog::Log(CObject * pobj, LPCTSTR fname, LPCTSTR msg)
{
	CString txt;
	TRY
	{
		if (pobj)
		{
			CRuntimeClass* prc = pobj->GetRuntimeClass();		
			if (prc)
			{
				CString cname = CString(prc->m_lpszClassName);
				txt.Format(_T("%s - %s - %s"), 
					cname,
					fname,
					msg);
			}
			else
				txt.Format(_T("%s - %s - %s"), 
				_T("NULL CLASS"),
				fname,
				msg);
		}
		else
		{
			//txt.Format(_T("%s - %s"), fname, msg);
			txt.Format(_T("%s"), msg);
		}
	}
	CATCH_ALL(pe)
	{
		//txt.Format(_T("%s - %s"), fname, msg);
		txt.Format(_T("%s"), msg);
	}
	END_CATCH_ALL;

	Log(txt);
}

void CLog::LogError(CObject * pobj, LPCTSTR fname, LPCTSTR msg,DWORD error /*= 0*/ )
{
	CString txt;	
	TRY
	{
		if (pobj)
		{
			CRuntimeClass* prc = pobj->GetRuntimeClass();		
			if (prc)
			{
				CString cname = CString(prc->m_lpszClassName);
				txt.Format(_T("%s - %s - %s"), 
					cname,
					fname,
					msg);
			}
			else
				txt.Format(_T("%s - %s - %s"), 
				_T("NULL CLASS"),
				fname,
				msg);
		}
		else
		{
			//txt.Format(_T("%s - %s"), fname, msg);
			txt.Format(_T("%s"), msg);
		}
	}
	CATCH_ALL(pe)
	{
		//txt.Format(_T("%s - %s"), fname, msg);
		txt.Format(_T("%s"), msg);
	}
	END_CATCH_ALL;
	LogError(txt,error);
}

void CLog::Log(CObject * pobj, LPCTSTR fname, CException &e, LPCTSTR msg)
{
	CString txt;	
	TRY
	{
		if (pobj)
		{
			CRuntimeClass* prc = pobj->GetRuntimeClass();		
			if (prc)
			{
				CString cname = CString(prc->m_lpszClassName);
				txt.Format(_T("%s - %s - %s"), 
					cname,
					fname,
					msg);
			}
			else
				txt.Format(_T("%s - %s - %s"), 
				_T("NULL CLASS"),
				fname,
				msg);
		}
		else
			txt.Format(_T("%s - %s"), 
			fname,
			msg);
	}
	CATCH_ALL(pe)
	{
		txt.Format(_T("%s - %s"), 
			fname,
			msg);
	}
	END_CATCH_ALL;
	Log(e,txt);
}

//----------------------------------------------------------------------------
void CLog::Log(CObject * pobj, LPCTSTR fname, long instance_code, LPCTSTR msg)
{
	CString txt;
	TRY
	{
		if (pobj)
		{
			CRuntimeClass* prc = pobj->GetRuntimeClass();		
			if (prc)
			{
				CString cname = CString(prc->m_lpszClassName);
				txt.Format(_T("%s - %s - %i - %s"), 
					cname,
					fname,
					instance_code,
					msg);
			}
			else
				txt.Format(_T("%s - %s - %i - %s"), 
				_T("NULL CLASS"),
				fname,
				instance_code,
				msg);
		}
		else
			txt.Format(_T("%s - %i - %s"), 
			fname,
			instance_code,
			msg);
	}
	CATCH_ALL(pe)
	{
		txt.Format(_T("%s - %i - %s"), 
			fname,
			instance_code,
			msg);
	}
	END_CATCH_ALL;

	Log(txt);
}

void CLog::LogError(CObject * pobj, LPCTSTR fname, long instance_code, LPCTSTR msg, DWORD error /*= 0*/ )
{
	CString txt;
	TRY
	{
		if (pobj)
		{
			CRuntimeClass* prc = pobj->GetRuntimeClass();		
			if (prc)
			{
				CString cname = CString(prc->m_lpszClassName);
				txt.Format(_T("%s - %s - %i - %s"), 
					cname,
					fname,
					instance_code,
					msg);
			}
			else
				txt.Format(_T("%s - %s - %i - %s"), 
				_T("NULL CLASS"),
				fname,
				instance_code,
				msg);
		}
		else
			txt.Format(_T("%s - %i - %s"), 
			fname,
			instance_code,
			msg);
	}
	CATCH_ALL(pe)
	{
		txt.Format(_T("%s - %i - %s"), 
			fname,
			instance_code,
			msg);
	}
	END_CATCH_ALL;
	LogError(txt,error);
}

void CLog::Log(CObject * pobj, LPCTSTR fname, CException &e, long instance_code, LPCTSTR msg)
{
	CString txt;	
	TRY
	{
		if (pobj)
		{
			CRuntimeClass* prc = pobj->GetRuntimeClass();		
			if (prc)
			{
				CString cname = CString(prc->m_lpszClassName);
				txt.Format(_T("%s - %s - %i - %s"), 
					cname,
					fname,
					instance_code,
					msg);
			}
			else
				txt.Format(_T("%s - %s - %i - %s"), 
				_T("NULL CLASS"),
				fname,
				instance_code,
				msg);
		}
		else
			txt.Format(_T("%s - %i - %s"), 
			fname,
			instance_code,
			msg);
	}
	CATCH_ALL(pe)
	{
		txt.Format(_T("%s - %i - %s"), 
			fname,
			instance_code,
			msg);
	}
	END_CATCH_ALL;

	Log(e,txt);
}

//----------------------------------------------------------------------------
void CLog::LogError(LPCTSTR msg, DWORD error /* = 0 */)
{
	//  [2/23/2018 HieuTT]
	error = GetLastError();

	CString msgerror;
	msgerror = GetWin32ErrorMessage(error);
	CString ns;
	ns.Format(_T("%s\r\n\t\t\tCode: %X\r\n\t\t\t%s"), msg, error, msgerror );
	Log(ns);
}

void CLog::Log(CException &e,LPCTSTR msg)
{
	TCHAR buf[1024];
	buf[0] = 0;
	e.GetErrorMessage(buf,1024);
	CString ns;
	ns.Format(_T("%s\r\n\t\t\tException: %s"), msg, buf );
	Log(ns);
}

//----------------------------------------------------------------------------
void CLog::LogCore(std::string logState, std::string fname, std::string msg)
{
	CString txt;

	CString sLogState = CString(CA2T(logState.c_str()));//, (int)logState.length()*sizeof(CHAR));
	CString sFame = CString(CA2T(fname.c_str()));//, (int)fname.length()*sizeof(CHAR));
	CString sMsg = CString(CA2T(msg.c_str()));//, (int)msg.length()*sizeof(CHAR));

	//txt.Format(_T("[%s]%s - %s"), sLogState, sFame, sMsg);
	txt.Format(_T("[%s]%s"), sLogState, sMsg);

	Log(txt);
}

//----------------------------------------------------------------------------
void CLog::LogCap(std::string logState, std::string fname, std::string msg)
{
	//CString txt;
	//txt.Format(_T("[%s]%s - %s"), logState, fname, msg);
	//txt.Format(_T("[%s]%s"), logState, msg);
	std::string szmsg;
	szmsg = std::string("[")+logState+std::string("]")+msg;
	Log(szmsg);
}

//----------------------------------------------------------------------------

void CLog::Log(LPCTSTR msg)
{
	//levent(_T("Trace log start post"));
	TRY 
	{
		CString sLog(_T(""));

		SYSTEMTIME currentTime;
		::GetLocalTime(&currentTime);
		
		sLog.Format(_T("%.2i%s%.4i %.2i:%.2i:%.2i.%.3i [%i] %s\r\n"), 
			currentTime.wDay, 
			m_months[currentTime.wMonth], 
			currentTime.wYear, 
			currentTime.wHour, 
			currentTime.wMinute, 
			currentTime.wSecond, 
			currentTime.wMilliseconds,
			GetCurrentThreadId(), 
			msg);

		//mq_.push(sLog);

		std::string wstrLog;
		wstrLog = CStringA(sLog).GetString();

		mq_.push(wstrLog);
	}
	CATCH_ALL(e)
	{
		levent(_T("Trace log exception"));
	}
	END_CATCH_ALL
}

void CLog::Log(std::string msg)
{
	//levent(_T("Trace log start post"));
	TRY 
	{
		CStringA sLog("");

		SYSTEMTIME currentTime;
		::GetLocalTime(&currentTime);

		sLog.Format("%.2i%s%.4i %.2i:%.2i:%.2i.%.3i [%i] ", 
			currentTime.wDay, 
			m_months[currentTime.wMonth], 
			currentTime.wYear, 
			currentTime.wHour, 
			currentTime.wMinute, 
			currentTime.wSecond, 
			currentTime.wMilliseconds,
			GetCurrentThreadId(), 
			msg);

		//mq_.push(sLog);
		
		std::string wstrLog;
		wstrLog = sLog.GetString() + msg + std::string("\r\n");

		mq_.push(wstrLog);
	}
	CATCH_ALL(e)
	{
		levent(_T("Trace log exception"));
	}
	END_CATCH_ALL
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//

void CLog::RemoveOldLogFile(int days, CString dir, CString namePrefix)
{
	DIR_REMOVE *pData = new DIR_REMOVE();
	pData->days = days;
	pData->dir = dir;

	CString sLogPattern(_T(""));
	sLogPattern.Format(_T("%s*.log"), namePrefix);
	pData->namePattern = sLogPattern;

	HANDLE hThread = ::CreateThread( NULL, 0, Thread_RemoveOldLogFile, (LPVOID) pData, 0, NULL);
	if (hThread == NULL)
	{
		LOGINFO(_T("Cannot start remove old log files"));
	}
	::CloseHandle(hThread);
}

DWORD WINAPI CLog::Thread_RemoveOldLogFile(LPVOID p)
{
	TRY
	{
		DIR_REMOVE *pData = reinterpret_cast<DIR_REMOVE *> (p);
		
		if (pData == NULL)
			return 0;
		else
		{
			int nDays = pData->days;
			CString sDir(_T("")), sNamePattern(_T(""));
			sDir = pData->dir;
			sNamePattern = pData->namePattern;
			
			CString sLog;
			sLog.Format(_T("Clean old log file: days=%i, dir=%s, namePrefix=%s"), nDays, sDir, sNamePattern);
			LOGINFO(sLog);

			CleanUpOldFile(nDays, sDir, sNamePattern);
		}
		
		if (pData)
		{
			delete pData;
			pData = NULL;
		}
	}
	CATCH_ALL(pe)// process exception [7/1/2015 AnhNT2]
	{
		TCHAR buf[1024];
		buf[0] = 0;
		pe->GetErrorMessage(buf, 1024);
		CString sLog;
		sLog.Format(_T("Can not remove old LogFile: %s. "), buf);
		LOGINFO(sLog);
	}
	END_CATCH_ALL;

	return 0;
}

void CLog::CleanUpOldFile(int days,
						  CString path, 
						  CString patternFileName, 
						  BOOL cleanUpSubFolder /* = TRUE */, 
						  BOOL DeleteSubFolder /*= TRUE*/ )
{
	CFileFind finder;
	BOOL bWorking;
	CTime lastWrite,now;
	CTimeSpan diff;

	CString search;
	now = CTime::GetCurrentTime();

	search = MakeFullPath(path, patternFileName);
	bWorking = finder.FindFile(search);


	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
			continue;

		if (finder.GetLastWriteTime(lastWrite))
		{
			diff = now - lastWrite;
			if (diff.GetDays() > days)	  
			{
				// remove attribute (readonly, hidden, system) before delete file
				SetFileAttributes(finder.GetFilePath(), FILE_ATTRIBUTE_ARCHIVE);
				if (!DeleteFile(finder.GetFilePath()))
				{
					//DWORD errorCode = GetLastError();
					LOGERROR(_T("Can not remove old LogFile: ") + finder.GetFilePath());
				}
			}
		}

	}
	finder.Close();

	if (!cleanUpSubFolder)
		return;

	search = MakeFullPath(path, _T("*.*"));
	bWorking = finder.FindFile(search);


	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots())
			continue;	

		if (finder.IsDirectory())
		{
			if (DeleteSubFolder)
			{
				if (finder.GetLastWriteTime(lastWrite))
				{
					diff = now - lastWrite;
					if (diff.GetDays() > days)  
					{
						CString sPath = finder.GetFilePath();

						// remove all file-folder-sub foler
						EmptyFolder(sPath, TRUE);

						// delete itself
						SetFileAttributes(sPath, FILE_ATTRIBUTE_ARCHIVE);
						if (RemoveDirectory(sPath))
						{
							LOGINFO(_T("Remove dir success: ") + sPath);
						}
						else
						{
							LOGINFO(_T("Remove dir failed: ") + sPath);
						}
					}
				}
				CleanUpOldFile(days,finder.GetFilePath(), patternFileName, cleanUpSubFolder , DeleteSubFolder);
			}
			else
				CleanUpOldFile(days,finder.GetFilePath(), patternFileName, cleanUpSubFolder , DeleteSubFolder);
			continue;
		}
	}
	finder.Close();
}