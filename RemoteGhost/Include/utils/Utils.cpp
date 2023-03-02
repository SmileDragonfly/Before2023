#include "stdafx.h"
#include "Utils.h"
#include <strsafe.h>
#include "utils/ConsoleSolidcoreInteract.h"
#include "TMSRemoteRestore.h"

void LogEvent(LPCTSTR pszFormat, ...)
{
	static TCHAR chMsg[4086];
	static TCHAR ServiceName[] = _T("Profile monitor agent\0");

	HANDLE hEventSource;
	LPTSTR lpszStrings[1];
	va_list pArg;

	va_start(pArg, pszFormat);
	_vstprintf_s(chMsg, pszFormat, pArg);
	va_end(pArg);

	lpszStrings[0] = chMsg;

	/* Get a handle to use with ReportEvent(). */
	hEventSource = RegisterEventSource(NULL, ServiceName);
	if (hEventSource != NULL)
	{
		/* Write to event log. */
		ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*)&lpszStrings[0], NULL);
		DeregisterEventSource(hEventSource);
	}
}


CString GetCurrentPath()
{
	CString	strFullPath(_T("")), strPath(_T(""));
	GetModuleFileName(NULL, strFullPath.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFullPath.ReleaseBuffer();
	strPath = strFullPath.Left(strFullPath.ReverseFind('\\'));

	return strPath;
}




CString GetWin32ErrorMessage(DWORD& errcode)
{
	LPVOID lpMsgBuf;
	CString msgnew;
	if (!errcode)
		errcode = GetLastError();
	::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
		);
	msgnew.Format(_T("%s"), lpMsgBuf);
	LocalFree(lpMsgBuf);
	return msgnew;
}

CString GetExceptionMessage(CException &e)
{
	TCHAR buf[1024];
	buf[0] = 0;
	e.GetErrorMessage(buf, 1024);
	CString sLog(_T(""));
	sLog.Format(_T("Exception: %s"), buf);
	return sLog;
}


CString GetFullPath(CString path)
{
	TCHAR full[_MAX_PATH*sizeof(TCHAR)* 2];
	if (_tfullpath(full, path, _MAX_PATH*sizeof(TCHAR)* 2) != NULL)
		return path;
	else
		return full;
}

CString MakeFullPath(CString fpath, CString fname)
{
	TCHAR path_buffer[_MAX_PATH*sizeof(TCHAR)* 2];
	_tmakepath_s(path_buffer, NULL, fpath, fname, NULL);
	return path_buffer;
}

BOOL CreateTreeFolder(CString lpszDirectory)
{
	CString strResult = lpszDirectory;

	if (strResult.Right(1) != _T('\\'))
		strResult += _T('\\');

	CString strDir(_T(""));
	BOOL bResult = FALSE;
	DWORD dwLastError = 0;
	try
	{
		// create directory structure one part at a time
		while (!strResult.IsEmpty())
		{
			strDir += strResult.Left(strResult.Find(_T("\\")) + 1);
			strResult = strResult.Mid(strResult.Find(_T("\\")) + 1);
			//LOGEVENT(strDir);
			bResult = CreateDirectory(strDir, 0);
			dwLastError = GetLastError();
		}

	}
	catch (...)
	{
		//LOGERRORNULL(_T("Create tree folder failure"), GetLastError());
		return FALSE;
	}

	if (!bResult)
	{
		if (dwLastError == ERROR_ALREADY_EXISTS)
			return TRUE;
		else
			return FALSE;
	}

	return TRUE;
}

long ToNumber(CString s)
{
	long v;
	v = _ttol((LPCTSTR)s);
	return v;
}

// DWORD ToNumber2(CString slargeNumber)
// {
// 	DWORD dw;
// 	BSTR bs = slargeNumber.AllocSysString();
// 
// 	TRY
// 	{
// 		VARIANT v;
// 		v.bstrVal = bs;
// 		v.vt = VT_BSTR;
// 		HRESULT hr = VariantChangeType(&v, &v, 0, VT_UI4);
// 		if (hr != S_OK)
// 			dw = 0;
// 		else
// 			dw = v.ulVal;
// 	}
// 	CATCH_ALL(e)
// 	{
// 		//LOGEXCEPTIONNULL(*e, _T("Exception"));
// 	}
// 	END_CATCH_ALL;
// 
// 	//SysFreeString(bs);
// }

long ParseParameters(CString& params, CString splitter, std::vector<CString>& result)
{
	TRY
	{
		CString apart;
		int curPos = 0;
		long counter = 0;
		result.clear();

		apart = params.Tokenize(splitter, curPos);
		while (!apart.IsEmpty())
		{
			result.push_back(apart);
			counter++;
			apart = params.Tokenize(splitter, curPos);
		};
		return counter;
	}
	CATCH_ALL(e)
	{
		return -1;
	}
	END_CATCH_ALL;

}

long ParseParameters(CString& params, CString splitter, CStringArray& result)
{
	TRY
	{
		CString apart;
		int curPos = 0;
		long counter = 0;
		result.RemoveAll();
		long maxPos = params.GetLength();
		apart = params.Tokenize(splitter, curPos);
		//while (!apart.IsEmpty() || ( ( 0 <= curPos) &&  (curPos < maxPos) ))
		while (!apart.IsEmpty())
		{
			result.Add(apart);
			counter++;
			apart = params.Tokenize(splitter, curPos);
		};
		return counter;
	}
	CATCH_ALL(e)
	{
		CString sLog(_T(""));
		sLog.Format(_T("Utils: Exception at ParseParameters. %s"), GetExceptionMessage(*e));
		LOGERROR(sLog);
		return -1;
	}
	END_CATCH_ALL;
}

CString ConvertULONGLONGToCString(ULONGLONG s)
{
	TRY
	{
	if (s < 0)
	return _T("error");
	else
	{
		CString sTemp(_T(""));

		// 			if (s >= ____ONE_GB)
		// 			{
		// 				double fileGB =  s/(double)____ONE_GB;
		// 				sTemp.Format(_T("%.2f GB (%I64u bytes)"), fileGB , s);
		// 			}
		// 			else if (s >= ____ONE_MB)
		// 			{
		// 				double fileMB =  s/(double)____ONE_MB;
		// 				sTemp.Format(_T("%.2f MB (%I64u bytes)"), fileMB, s);
		// 			}
		// 			else 
		if (s >= ____ONE_KB)
		{
			//double fileKB =  s/(double)____ONE_KB;
			//sTemp.Format(_T("%.2f KB"), fileKB);

			ULONGLONG fileKB = s / ____ONE_KB;
			if (s%____ONE_KB > 0)
				fileKB += 1;
			sTemp.Format(_T("%I64u KB"), fileKB);
		}
		else
		{
			//sTemp.Format(_T("%I64u bytes"), s);
			sTemp = _T("1 KB");
		}

		return sTemp;
	}
}
	CATCH_ALL(e)
	{
		//LOGEXCEPTIONNULL(*e, _T("Exception"));
		return _T("error");
	}
	END_CATCH_ALL;
}

CString ConvertCurrentDateTimeToString()
{
	CString sDateTime(_T(""));

	//SYSTEMTIME /*stUTC, */stLocal;
	SYSTEMTIME stLocal;
	::GetLocalTime(&stLocal);

	// Convert the last-write time to local time.
	//FileTimeToSystemTime(&(aFT.ftLastWriteTime), &stUTC);
	//SystemTimeToTzSpecificLocalTime(NULL, &now, &stLocal);

	// Build a string showing the date and time.
	sDateTime.Format(_T("%d-%02d-%02d %02d:%02d:%02d:%d"),
		stLocal.wYear, stLocal.wMonth, stLocal.wDay, stLocal.wHour, stLocal.wMinute, stLocal.wSecond, stLocal.wMilliseconds);

	return sDateTime;
}

CStringW ConvertUTF8ToUnicode(char* pUtf8)
{
	if (pUtf8==NULL) 
		return _T(""); // nothing to do

	CStringW uni(_T(""));
	int len = MultiByteToWideChar(CP_UTF8, 0, pUtf8, -1, NULL, 0);

	// get length (cc) of the new widechar excluding the \0 terminator first
	if (len>1)
	{ 
		// convert
		wchar_t *buf = (wchar_t *)calloc(len+1,sizeof(wchar_t));
		if (buf)
			MultiByteToWideChar(CP_UTF8, 0, pUtf8, -1, buf, len);
		uni = CString(buf);

		if (buf)
			free(buf);
	}
	return uni;
}

CStringA ConvertUnicodeToUTF8(wchar_t* pUnicode)
{
	CStringA utf8("");
	int len = WideCharToMultiByte(CP_UTF8, 0, pUnicode, -1, NULL, 0, 0, 0);
	if (len>1)
	{ 
		char* pUTF8 = (char *)calloc(len+1,sizeof(char));
		if (pUTF8) 
			WideCharToMultiByte(CP_UTF8, 0, pUnicode, -1, pUTF8, len, 0, 0);

		utf8 = CStringA(pUTF8);

		if (pUTF8)
			free(pUTF8);
	}
	return utf8;
}

#if (_MSC_VER >=1700)

inline std::tm Localtime(const time_t &time_tt)
{
#ifdef _WIN32
	//std::tm tm;
	//localtime_s(&tm, &time_tt);

#ifdef _USE_32BIT_TIME_T
	std::tm tm;
	_localtime32_s(&tm, (__time32_t*)&time_tt);
#else
	std::tm tm;
	_localtime64_s(&tm, (__time64_t*)&time_tt);
#endif
#else
	std::tm tm;
	localtime_r(&time_tt, &tm);
#endif

	return tm;
}

std::chrono::system_clock::time_point NextRotationTp(int rotation_hour, int rotation_minute)
{
	auto now = std::chrono::system_clock::now();
	time_t tnow = std::chrono::system_clock::to_time_t(now);
	tm date = Localtime(tnow);
	date.tm_hour = rotation_hour;
	date.tm_min = rotation_minute;
	date.tm_sec = 0;
	auto rotation_time = std::chrono::system_clock::from_time_t(mktime(&date));
	if (rotation_time > now)
		return rotation_time;
	else
		return std::chrono::system_clock::time_point(rotation_time + std::chrono::hours(24));
}

#else

inline std::tm Localtime(const time_t &time_tt)
{
#ifdef _WIN32
	//std::tm tm;
	//localtime_s(&tm, &time_tt);

#ifdef _USE_32BIT_TIME_T
	std::tm tm;
	_localtime32_s(&tm, (__time32_t*)&time_tt);
#else
	std::tm tm;
	_localtime64_s(&tm, (__time64_t*)&time_tt);
#endif
#else
	std::tm tm;
	localtime_r(&time_tt, &tm);
#endif

	return tm;
}

#endif



CHAR** WINAPI MBStringToMBArgv(LPSTR str, INT *pargc)
{
	// alloc memory for a wide version of str
	LPWSTR wstr = (LPWSTR)LocalAlloc(LMEM_FIXED,
		(lstrlenA(str) + 1) * sizeof(WCHAR));

	// "copy" str to wstr
	WCHAR *wp = wstr;
	while (*wp++ = *str++);

	// parse wstr
	WCHAR **wargv = CommandLineToArgvW(wstr, pargc);

	// cleanup
	LocalFree(wstr);

	// determine memory needed for argv
	size_t needed = *pargc * (sizeof(CHAR*)+1);        // ptrs and NULs
	for (INT i = 0; i < *pargc; i++)                                        // add arg lengths
		needed += lstrlenW(wargv[i]);

	// allocate memory for ptrs and args
	LPBYTE argv = (LPBYTE)LocalAlloc(LMEM_FIXED, needed);

	// fill the pointers and strings
	CHAR **ptrs = (CHAR**)argv;
	CHAR *parg = (CHAR*)((CHAR**)argv + *pargc);
	for (INT i = 0; i < *pargc; i++)
	{
		ptrs[i] = parg;
		wp = wargv[i];
		while (*parg++ = (CHAR)*wp++);
	}

	// cleanup
	LocalFree(wargv);

	// when done with it use LocalFree() on the returned pointer
	return (CHAR**)argv;
}


void ChangeProcessPriority()
{
	HANDLE hProcessViewerProc = GetCurrentProcess();

	if (hProcessViewerProc != NULL)
	{
		BOOL bRet = SetPriorityClass(hProcessViewerProc, ABOVE_NORMAL_PRIORITY_CLASS);
		if (!bRet)
			LOGERROR(_T("SetPriorityClass failure"));

		bRet = SetProcessPriorityBoost(hProcessViewerProc, FALSE);
		if (!bRet)
			LOGERROR(_T("SetProcessPriorityBoost failure"));
	}
	else
	{
		LOGERROR(_T("GetCurrentProcess failure"));
	}
}

BOOL EnablePrivilege(LPCTSTR PrviName)
{
	// 	/////////////////////////////////////////////////////////
	// 	//   Note: Enabling SeDebugPrivilege adapted from sample
	// 	//     MSDN @ http://msdn.microsoft.com/en-us/library/aa446619%28VS.85%29.aspx
	// 	// Enable SeDebugPrivilege
	// 	HANDLE hToken = NULL;
	// 	TOKEN_PRIVILEGES tokenPriv;
	// 	LUID luidDebug;
	// 	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken) != FALSE)
	// 	{
	// 		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luidDebug) != FALSE)
	// 		{
	// 			tokenPriv.PrivilegeCount = 1;
	// 			tokenPriv.Privileges[0].Luid = luidDebug;
	// 			tokenPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	// 			if (AdjustTokenPrivileges(hToken, FALSE, &tokenPriv, 0, NULL, NULL) != FALSE)
	// 			{
	// 				// Always successful, even in the cases which lead to OpenProcess failure
	// 				cout << "SUCCESSFULLY CHANGED TOKEN PRIVILEGES" << endl;
	// 			}
	// 			else
	// 			{
	// 				cout << "FAILED TO CHANGE TOKEN PRIVILEGES, CODE: " << GetLastError() << endl;
	// 			}
	// 		}
	// 	}
	// 	CloseHandle(hToken);
	// 	// Enable SeDebugPrivilege
	// 	/////////////////////////////////////////////////////////

// 	static bool bEnabled = false;
// 	if (bEnabled)
// 	{
// 		return TRUE;
// 	}	

	HANDLE hToken = 0;
	TOKEN_PRIVILEGES tkp = { 0 };
	BOOL             bRtn = FALSE;

	// Get a token for this process.
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		return FALSE;
	}

	// Get the LUID for the privilege. 
	if (LookupPrivilegeValue(NULL, PrviName, &tkp.Privileges[0].Luid))
	{
		tkp.PrivilegeCount = 1;  // one privilege to set    
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		// Set the privilege for this process. 
		if (AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), (PTOKEN_PRIVILEGES)NULL, NULL))
		{
			bRtn = TRUE;
			//bEnabled = true;
		}
	}

	CloseHandle(hToken);
	return bRtn;
}

CString FormatSize(DWORDLONG dwValue)
{
	CString vSize[] = { _T("KB"), _T("MB"), _T("GB") };
	int i = 0;
	CString sResult(_T("0"));

	for (i = 3; i > 0; i--)
	{
		double fpow = pow((double)1024, (double)i);
		if (dwValue >= fpow)
		{
			double fConvert = dwValue / fpow;
			sResult.Format(_T("%.1f %s"), fConvert, vSize[i - 1]);
			break;
		}
	}

	return sResult;
}

//////////////////////////////////////////////////////////////////////////


CString GetRootFolder()
{
	CString strFolder(_T(""));

	CString	strPath;
	GetModuleFileName(NULL, strPath.GetBuffer(_MAX_PATH), _MAX_PATH);
	strPath.ReleaseBuffer();
	int index = strPath.ReverseFind('\\');
	if (index <= 0)
	{
		return _T("");
	}

	strFolder = strPath.Mid(0, index);
	if (!PathIsDirectory(strFolder))
	{
		return _T("");
	}
	return strFolder;
}


BOOL GetPathInfo(CString path, CString *pdrive, CString *pdir, CString *pfname, CString *pext)
{
	try
	{
		WCHAR szDrive[_MAX_DRIVE];
		WCHAR szDir[_MAX_DIR];
		WCHAR szFname[_MAX_FNAME];
		WCHAR szExt[_MAX_EXT];
		CString fullpath = GetFullPath(path);
		_wsplitpath_s(fullpath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFname, _MAX_FNAME, szExt, _MAX_EXT);

		if (pdrive)
			*pdrive = CString(szDrive);
		if (pdir)
			*pdir = CString(szDir);
		if (pfname)
			*pfname = CString(szFname);
		if (pext)
			*pext = CString(szExt);

		return TRUE;
	}
	catch (CMemoryException* e)
	{
		return FALSE;
	}
	catch (CFileException* e)
	{
		return FALSE;
	}
	catch (CException* e)
	{
		return FALSE;
	}
}

BOOL GetPathInfo(CString path, CString *pdir, CString *pfname)
{
	try
	{
		WCHAR szPath[_MAX_PATH];
		WCHAR szDrive[_MAX_DRIVE];
		WCHAR szDir[_MAX_DIR];
		WCHAR szFname[_MAX_FNAME];
		WCHAR szExt[_MAX_EXT];
		CString fullpath = GetFullPath(path);
		_wsplitpath_s(fullpath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFname, _MAX_FNAME, szExt, _MAX_EXT);

		_wmakepath_s(szPath, _MAX_PATH, szDrive, szDir, NULL, NULL);
		if (pdir)
			*pdir = CString(szPath);
		if (pfname)
			*pfname = CString(szFname) + CString(szExt);

		return TRUE;
	}
	catch (CMemoryException* e)
	{
		return FALSE;
	}
	catch (CFileException* e)
	{
		return FALSE;
	}
	catch (CException* e)
	{
		return FALSE;
	}
}

void EmptyFolder(CString path, BOOL removeSubFolder /*= FALSE*/ )
{
	if (path.IsEmpty())
		return;

	CFileFind finder;
	BOOL bWorking;
	bWorking = finder.FindFile(MakeFullPath( path,_T("*.*")));

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files; otherwise, we'd
		// recur infinitely!

		if (finder.IsDots())
			continue;

		//if (finder.IsDirectory())
		//continue;

		if (finder.IsDirectory())
		{
			SetFileAttributes( finder.GetFilePath(), FILE_ATTRIBUTE_ARCHIVE);
			EmptyFolder(finder.GetFilePath(), removeSubFolder);
			if (removeSubFolder)
			{
				RemoveDirectory(finder.GetFilePath());
			}
			continue;
		}
		SetFileAttributes( finder.GetFilePath(), FILE_ATTRIBUTE_ARCHIVE);

		DeleteFile(finder.GetFilePath());
	}
	finder.Close();
}

#define BEGIN_PATH_TAG        ("\\??\\")
BOOL GetUncProcessPathA(__in LPCSTR lpszFullPath, __out LPSTR lpszPath, __in size_t nPath)
{
	size_t uNameLen = strlen(BEGIN_PATH_TAG);

	if (uNameLen < MAX_PATH)
	{
		BOOL bFound = FALSE;
		bFound = _strnicmp(lpszFullPath, BEGIN_PATH_TAG, uNameLen) == 0
			&& *(lpszFullPath + uNameLen + 1) == (':');

		if (bFound)
		{
			// Reconstruct pszFilename using szTempFile
			// Replace device path with DOS path
			CHAR szTempFile[MAX_PATH];
			StringCchPrintfA(szTempFile,
				MAX_PATH,
				("%s"),
				lpszFullPath + uNameLen);
			StringCchCopyNA(lpszPath, nPath, szTempFile, strlen(szTempFile));

			return TRUE;
		}
	}

	return FALSE;
}

BOOL GetUserPathFromDevicePath(__in CString strFilename, __out CString& strUserPath, __in size_t nUserPath)
{
	BOOL bFound = FALSE;
	// Translate path with device name to drive letters.
	TCHAR szTemp[MAX_PATH];
	szTemp[0] = '\0';
	LPCWSTR pszFilename = strFilename.GetString();
	WCHAR lpszUserPath[MAX_PATH];

	if (GetLogicalDriveStrings(MAX_PATH - 1, szTemp))
	{
		TCHAR szName[MAX_PATH];
		TCHAR szDrive[3] = TEXT(" :");
		TCHAR* p = szTemp;

		do
		{
			// Copy the drive letter to the template string
			szDrive[0] = p[0];
			szDrive[1] = p[1];
			szDrive[2] = L'\0';

			// Look up each device name
			if (QueryDosDevice(szDrive, szName, MAX_PATH))
			{
				size_t uNameLen = _tcslen(szName);

				if (uNameLen < MAX_PATH)
				{
					bFound = _tcsnicmp(pszFilename, szName, uNameLen) == 0
						&& *(pszFilename + uNameLen) == _T('\\');

					if (bFound)
					{
						// Reconstruct pszFilename using szTempFile
						// Replace device path with DOS path
						StringCchPrintf(lpszUserPath,
							nUserPath,
							TEXT("%s%s"),
							szDrive,
							pszFilename + uNameLen);
						strUserPath = CString(lpszUserPath);
					}
				}
			}

			// Go to the next NULL character.
			while (*p++);
		} while (!bFound && *p); // end of string
	}
	return bFound;
}

BOOL GetDevicePathFromUserPath(__in CString strFullPath, __out CString& strDevicePath, __in size_t nDevicePath)
{
	// Translate path with device name to drive letters.
	WCHAR szTemp[MAX_PATH * 2];
	szTemp[0] = L'\0';
	LPCWSTR lpszFullPath = strFullPath.GetString();
	WCHAR lpszDevicePath[MAX_PATH];

	if (GetLogicalDriveStringsW(_countof(szTemp) - 1, szTemp))
	{
		WCHAR szName[MAX_PATH];
		WCHAR szDrive[3] = L" :";
		BOOL bFound = FALSE;
		WCHAR* p = szTemp;

		do
		{
			// Copy the drive letter to the template string
			szDrive[0] = p[0];
			szDrive[1] = p[1];
			szDrive[2] = L'\0';

			// Look up each device name
			if (QueryDosDeviceW(szDrive, szName, MAX_PATH))
			{
				size_t uNameLen = wcslen(szDrive);

				if (uNameLen < MAX_PATH)
				{
					bFound = _wcsnicmp(lpszFullPath, szDrive, uNameLen) == 0
						&& *(lpszFullPath + uNameLen) == L'\\';

					if (bFound)
					{
						// Reconstruct pszFilename using szTempFile
						// Replace device path with DOS path
						WCHAR szTempFile[MAX_PATH];
						StringCchPrintfW(szTempFile,
							MAX_PATH,
							L"%s%s",
							szName,
							lpszFullPath + uNameLen);
						StringCchCopyNW(lpszDevicePath, nDevicePath, szTempFile, wcslen(szTempFile));
						strDevicePath = CString(lpszDevicePath);
					}
				}
			}

			// Go to the next NULL character.
			while (*p++);
		} while (!bFound && *p); // end of string
		return TRUE;
	}
	return FALSE;
}


BOOL Execute(CString command, DWORD timeout, CString* serr /*= NULL*/)
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
			WaitForSingleObject(pi.hProcess, timeout);

			sLog.Format(_T("End command: %s"), command);
			LOGINFO(sLog);

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
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

BOOL ExecuteCommandLine(CString sCommand, CString& sResult)
{
	DWORD er = 0;
	CConsoleSolidcoreInteract cs;
	cs.SetCommandParams(sCommand, 60000);

	CString sLog(_T(""));
	CString szMessage(_T(""));
	BOOL bRun = cs.RunCommandLine(szMessage, sResult);
	if (bRun)
	{
		sLog.Format(_T("Execute command successfully. \r\nMessage: %s \r\nResult: %s"), szMessage, sResult);
	}
	else
	{
		sLog.Format(_T("Execute command failure. \r\nMessage: %s \r\nResult: %s"), szMessage, sResult);
		sResult = sLog;
	}

	LOGNULL(sLog);

	return bRun;
}

int KillProcessByName(LPCTSTR fname)
{
	CDWordArray processBuff;
	DWORD cbNeeded, cProcesses, bsize, dwsize;
	dwsize = 1024;
	DWORD i;
	i = 0;
	do
	{
		i++;
		dwsize *= i;
		bsize = dwsize * sizeof(DWORD);
		processBuff.SetSize(dwsize);

		if (!EnumProcesses(processBuff.GetData(), bsize, &cbNeeded))
		{
			CString serr;
			serr.Format(_T("Cannot collect all process name (%i)"), i);
			LOGNULL(serr);
			return -1;
		}
	} while (cbNeeded >= bsize);

	cProcesses = cbNeeded / sizeof(DWORD);

	HANDLE hProcess;
	HMODULE hMod;
	TCHAR szProcessName[MAX_PATH];

	for (i = 0; i < cProcesses; i++)
	{
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, processBuff[i]);

		// Get the process name.
		if (hProcess)
		{
			if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
			{
				if (GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName)))
				{
					if (_tcsicmp(fname, szProcessName) == 0)
					{
						if (!TerminateProcess(hProcess, 0))
						{
							CString serr;
							serr.Format(_T("Cannot terminate process name %s. Error code = %i"), szProcessName, GetLastError());
							LOGNULL(serr);
							//return 0;
						}
						else
						{
							CString serr;
							serr.Format(_T("Terminated process name %s."), szProcessName);
							LOGNULL(serr);
							//return 0;			
						}
					}
				}
			}
		}
		CloseHandle(hProcess);
	}
	return -1;
}
