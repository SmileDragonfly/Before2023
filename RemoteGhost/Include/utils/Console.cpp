// Copyright (c) 2015 Microtec. All rights reserved.

#include "stdafx.h"
#include "Console.h"
#include <Tlhelp32.h>

#define ____DEFAULT_WINDOW_CONSOLE_TITLE	_T("Symphony Console Command Window")
BOOL g_bTraceLog = FALSE;

IMPLEMENT_DYNAMIC(CConsole, CObject)

//CCriticalSection CConsole::m_cs;

CConsole::CConsole()
{
	m_bInit = FALSE;
	m_strShell				= _T("");
	m_strLastErrorMessage	= _T("");
	m_dwLastErrorCode		= 0;
	//m_vData.clear();
	m_hWndConsole			= NULL;
	//m_dwRows				= 50;
	m_dwBufferColumns		= DEFAUL_CONSOLE_BUFFER_COLUMN;
	m_dwBufferRows			= DEFAUL_CONSOLE_BUFFER_ROW;
	m_hStdOut				= NULL;
	m_hStdOutFresh			= NULL;
	//m_hQuitEvent			= NULL;
	m_hConsoleProcess		= NULL;
	//m_hMonitorThread		= NULL;
	m_pScreenBuffer			= NULL;
}

CConsole::~CConsole(void)
{
	m_bInit = FALSE;

	// clear data buffer
	DEL_ARR(m_pScreenBuffer);
	//m_vData.clear();

	// set quit event
	//::SetEvent(m_hQuitEvent);

	// stop and destroy thread
	//if (m_hMonitorThread)
	//{
	//	::WaitForSingleObject(m_hMonitorThread, 2000);
	//	::CloseHandle(m_hMonitorThread);
	//	m_hMonitorThread = 0;
	//}

	// stop and terminal process of console cmd.exe
	if (m_hConsoleProcess)
	{
		if (!::TerminateProcess(m_hConsoleProcess, 0))
		{
			DWORD er = GetLastError();
			if (er != ERROR_ACCESS_DENIED)
				LOGERROR(_T("Cannot terminate executing application"), er);
		}
	}		
	::CloseHandle(m_hConsoleProcess);
	m_hConsoleProcess = 0;

	// close event
	//::CloseHandle(m_hQuitEvent);

	// shutdown console process
	::CloseHandle(m_hStdOut);
	::CloseHandle(m_hStdOutFresh);

	// free this console
	::FreeConsole();
}

/*****************************************************************
* Function: InitHandles
* Description: init path shell command, create new console read cmd.exe output
* Input values: 
* Output value:
******************************************************************/
BOOL CConsole::InitHandles()
{
	try
	{
		// get cmd.exe
		if (m_strShell.GetLength() == 0)
		{
			TCHAR szComspec[MAX_PATH];
			if (::GetEnvironmentVariable(_T("COMSPEC"), szComspec, MAX_PATH) > 0) 
				m_strShell = szComspec;		
			else
				m_strShell = _T("cmd.exe");
		}
		LOGNULL(m_strShell);

		// create the console window
		TCHAR szConsoleTitle[MAX_PATH];
		if (!::AllocConsole())
		{		
			m_strLastErrorMessage = _T("Cannot init console window");
			m_dwLastErrorCode     = GetLastError();
			return FALSE;
		}

		// we use this to avoid possible problems with multiple console instances running
		_stprintf_s(szConsoleTitle, _T("%i"), ::GetCurrentThreadId());
		if (!::SetConsoleTitle(szConsoleTitle))
		{		
			m_strLastErrorMessage = _T("Cannot set console title by ThreadID");
			m_dwLastErrorCode     = GetLastError();
			return FALSE;
		}
		
		// retry wait and get this hWnd console window in 30s
		int nRetry = 600;
		while ((nRetry > 0) && (m_hWndConsole == NULL)) 
		{		
			m_hWndConsole = ::FindWindow(NULL, szConsoleTitle);
			if (m_hWndConsole)
				break;
			::Sleep(50);
			nRetry --;
		}
		
		if (m_hWndConsole == NULL)
		{
			m_strLastErrorMessage = _T("Cannot get hWnd console by FindWindow using ThreadID title");
			m_dwLastErrorCode     = GetLastError();
			return FALSE;
		}			
		::ShowWindow(m_hWndConsole, SW_HIDE);

		// close all cmd with title "Symphony Console Command Window"
		HWND hWndOldCMD = NULL;
		nRetry = 10;
		while ((nRetry > 0) && (hWndOldCMD == NULL))
		{		
			hWndOldCMD = ::FindWindow(NULL, ____DEFAULT_WINDOW_CONSOLE_TITLE);
			if (hWndOldCMD)
				CloseWindow(hWndOldCMD);
			::Sleep(50);
			nRetry --;
		}

		//////////////////////////////////////////////////////////////////////////
		// set new title default for this console
		if (!::SetConsoleTitle(____DEFAULT_WINDOW_CONSOLE_TITLE))
		{
			m_strLastErrorMessage = _T("Cannot set console title by default Symphony console window title");
			m_dwLastErrorCode	  = GetLastError();
			return FALSE;
		}

		m_hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
		if (m_hStdOut == INVALID_HANDLE_VALUE)
		{
			m_strLastErrorMessage = _T("Cannot get handle hstdOut ");
			m_dwLastErrorCode = GetLastError();
			return FALSE;
		}

		// reset size console and buffer
		COORD dwLargestConsoleSize = ::GetLargestConsoleWindowSize(m_hStdOut);
		if (dwLargestConsoleSize.Y > 0)
			m_dwBufferRows = dwLargestConsoleSize.Y;

		if (dwLargestConsoleSize.X > 0)
			m_dwBufferColumns = min(dwLargestConsoleSize.X, (SHORT)((48 * 1024) / (m_dwBufferRows*sizeof(CHAR_INFO))));

		CString sLog(_T(""));
		sLog.Format(_T("Console window largest size (X=%d-Y=%d) m_dwBufferRows:%d - m_dwBufferColumns:%d"), 
			dwLargestConsoleSize.X, dwLargestConsoleSize.Y, m_dwBufferRows, m_dwBufferColumns);
		LOGNULL(sLog);

		//////////////////////////////////////////////////////////////////////////
		// this is a little hack needed to support columns greater than standard 80
		if (!RefreshStdOut())
			return FALSE;
		
		InitConsoleWndSize();

		m_strLastErrorMessage = _T("Init handles successfully.");
		m_dwLastErrorCode	  = GetLastError();

		// set flag init is TRUE
		m_bInit = TRUE;

		return TRUE;
	}
	catch (...)
	{
		DWORD err = GetLastError();
		LOGERROR(_T("Exception"), err);
		return FALSE;
	}
}

/*****************************************************************
* Function: StartShellProcess
* Description: start shell new console cmd.exe, monitor this console
* Input values: 
* Output value:
******************************************************************/
BOOL CConsole::StartShellProcess(CString processToken /*= _T("")*/)
{
	try 
	{
		// init handles
		if (!InitHandles())
			return FALSE;

		// check process token
// 		if (processToken.CompareNoCase(_T("")) != 0)
// 		{
// 			// change priority to access process
// 			ChangeProcessPriority();
// 			EnablePrivilege(SE_DEBUG_NAME);
// 			
// 			CString sErr(_T(""));
// 			CString sLog(_T(""));
// 
// 			PROCESS_INFORMATION pi;
// 			STARTUPINFO si;
// 			BOOL bResult = FALSE;
// 			DWORD winlogonPid = 0;
// 			HANDLE hPToken, hProcess;
// 
// 			// Find the WinLogon process
// 			PROCESSENTRY32 procEntry;
// 
// 			HANDLE hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
// 			if (hSnap == INVALID_HANDLE_VALUE)
// 				return FALSE;
// 
// 			procEntry.dwSize = sizeof(PROCESSENTRY32);
// 			if (!::Process32First(hSnap, &procEntry))
// 				return FALSE;
// 
// 			BOOL bFindProcess = FALSE;
// 			do
// 			{
// 				if (_tcsicmp(procEntry.szExeFile, processToken) == 0)	//|| _tcsicmp(procEntry.szExeFile, L"winlogon.exe") == 0)
// 				{
// 					// We found a winlogon process
// 					// Dont care to check sessionID, just get the process ID
// 					winlogonPid = procEntry.th32ProcessID;
// 					bFindProcess = TRUE;
// 					break;
// 				}
// 			} while (::Process32Next(hSnap, &procEntry));
// 
// 			// return if can not find process token
// 			if (!bFindProcess || winlogonPid == 0)
// 			{
// 				sLog.Format(_T("Cannot find process token: %s"), processToken);
// 				LOGNULL(sLog);
// 				return FALSE;
// 			}
// 
// 			// Get process token
// 			hProcess = ::OpenProcess(MAXIMUM_ALLOWED, FALSE, winlogonPid);
// 			if (!::OpenProcessToken(hProcess,
// 				TOKEN_ADJUST_PRIVILEGES |
// 				TOKEN_QUERY |
// 				TOKEN_DUPLICATE |
// 				TOKEN_ASSIGN_PRIMARY |
// 				TOKEN_ADJUST_SESSIONID |
// 				TOKEN_READ |
// 				TOKEN_WRITE,
// 				&hPToken))
// 			{
// 				// Just a sample of error message
// 				DWORD err = GetLastError();
// 				sErr = GetWin32ErrorMessage(err);
// 
// 				sLog.Format(_T("OpenProcessToken failed. Error=%s"), sErr);
// 				LOGNULL(sLog);
// 				return FALSE;
// 			}
// 			if (hProcess)
// 				::CloseHandle(hProcess);
// 
// 			// Launch the process in the client's log on session.
// 			ZeroMemory(&pi, sizeof(pi));
// 			ZeroMemory(&si, sizeof(STARTUPINFO));
// 
// 			si.cb = sizeof(STARTUPINFO);
// 			si.lpDesktop = _T("winsta0\\default");
// 			si.dwFlags |= STARTF_USESHOWWINDOW;
// 			si.wShowWindow = SW_HIDE;	// auto create cmd.exe hidden
// 
// 			bResult = ::CreateProcessAsUser(
// 				hPToken,        // client's access token
// 				NULL,			// file to execute //szExeFile/
// 				(LPWSTR)(LPCTSTR)m_strShell,           // command line
// 				NULL,           // pointer to process SECURITY_ATTRIBUTES
// 				NULL,           // pointer to thread SECURITY_ATTRIBUTES
// 				TRUE,           // handles are not inheritable
// 				0,				// creation flags
// 				NULL,           // pointer to new environment block 
// 				NULL,           // name of current directory 
// 				&si,			// pointer to STARTUPINFO structure
// 				&pi				// receives information about new process
// 				);
// 
// 			// Error
// 			if (bResult == 0)
// 			{
// 				// Just a sample of error message
// 				DWORD err = GetLastError();
// 				sErr = GetWin32ErrorMessage(err);
// 
// 				sLog.Format(_T("CreateProcessAsUser failed. Command=%s. Error=%s"), (LPWSTR)(LPCTSTR)m_strShell, sErr);
// 				LOGNULL(sLog);
// 				return FALSE;
// 			}
// 			
// 			if (hPToken)
// 				::CloseHandle(hPToken);
// 			if (pi.hThread)
// 				::CloseHandle(pi.hThread);
// 
// 			sLog.Format(_T("CreateProcessAsUser success. PID=%d"), pi.dwProcessId);
// 			LOGNULL(sLog);
// 
// 			// set handles
// 			m_hConsoleProcess = pi.hProcess;
// 			m_strLastErrorMessage = _T("Start shell process [with token] successfully.");
// 			LOGNULL(m_strLastErrorMessage);
// 
// 			m_dwLastErrorCode = GetLastError();
// 			return TRUE;
// 		}
// 		else
// 		{
			// setup the start up info struct
			PROCESS_INFORMATION pi;
			STARTUPINFO si;
			::ZeroMemory(&si, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);
			si.dwFlags |= STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_HIDE;	// auto create cmd.exe hidden

			if (!::CreateProcess(NULL,
				(LPWSTR)(LPCTSTR)m_strShell,       // command line 
				NULL,			// process security attributes 
				NULL,			// primary thread security attributes 
				TRUE,			// handles are inherited 
				0,				// creation flags 
				NULL,			// use parent's environment 
				NULL,			// use parent's current directory 
				&si,			// STARTUPINFO pointer 
				&pi))			// receives PROCESS_INFORMATION 
			{
				m_strLastErrorMessage.Format(_T("Can not start command shell: \"%s\""), m_strShell);
				m_dwLastErrorCode = GetLastError();
				return FALSE;
			}

			//  [7-July-2015 HIEUTT] - Run with SAService --> never show console window
			//::ShowWindow(m_hWndConsole, SW_SHOWNORMAL);
			//::SetForegroundWindow(m_hWndConsole);

			// close main thread handle
			::CloseHandle(pi.hThread);

			// set handles
			m_hConsoleProcess = pi.hProcess;
			//m_hQuitEvent	  = ::CreateEvent(NULL, FALSE, FALSE, NULL);
			//if (!m_hQuitEvent)
			//{
			//	m_strLastErrorMessage = _T("Can not create thread monitor console");
			//	m_dwLastErrorCode	  = GetLastError();
			//	return FALSE;
			//}	

			// create exit monitor thread
			//DWORD dwThreadID;
			//m_hMonitorThread = ::CreateThread(NULL, 0, CConsole::MonitorThreadStatic, this, CREATE_SUSPENDED, &dwThreadID);
			//if (!m_hMonitorThread)
			//{
			//	m_strLastErrorMessage = _T("Can not create thread monitor console");
			//	m_dwLastErrorCode	  = GetLastError();
			//	return FALSE;
			//}

			// now we can start the monitor thread
			//if(!::ResumeThread(m_hMonitorThread))
			//{
			//	m_strLastErrorMessage = _T("Can not resume thread monitor console");
			//	m_dwLastErrorCode     = GetLastError();
			//	return FALSE;
			//}

			m_strLastErrorMessage = _T("Start shell process successfully.");
			LOGNULL(m_strLastErrorMessage);

			m_dwLastErrorCode = GetLastError();

			return TRUE;
		}
//	}
	catch (...)
	{
		DWORD err = GetLastError();
		LOGERROR(_T("Exception"), err);
		return FALSE;
	}
}

/*****************************************************************
* Function: RefreshStdOut
* Description: register handle read output of console: 
			   text from console of cmd.exe --> copy to this console
* Input values: 
* Output value:
******************************************************************/
BOOL CConsole::RefreshStdOut() 
{
	if (m_hStdOutFresh) 
		::CloseHandle(m_hStdOutFresh);
	m_hStdOutFresh = ::CreateFile(_T("CONOUT$"), 
								  GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 
								  NULL, OPEN_EXISTING, 0, 0);
	if (m_hStdOutFresh == INVALID_HANDLE_VALUE)
	{		
		m_strLastErrorMessage = _T("Cannot create file hstdOutRefresh");
		m_dwLastErrorCode     = GetLastError();
		return FALSE;
	}

	return TRUE;
}

/*****************************************************************
* Function: RefreshScreenBuffer
* Description: read all text in this console screen buffer
* Input values: 
* Output value:
******************************************************************/
void CConsole::RefreshScreenBuffer() 
{
	// console screen buffer info for console repainting
	CONSOLE_SCREEN_BUFFER_INFO csbiConsole;	
	::GetConsoleScreenBufferInfo(m_hStdOutFresh, &csbiConsole);

	CString sLog(_T(""));
	if (g_bTraceLog)
	{
		sLog.Format(_T("1-dwCursorPosition (X=%d-Y=%d) dwMaximumWindowSize (X=%d-Y=%d) dwSize (X=%d-Y=%d) srWindow (%d-%d-%d-%d) wAttributes (%d)"),
			csbiConsole.dwCursorPosition.X,
			csbiConsole.dwCursorPosition.Y,
			csbiConsole.dwMaximumWindowSize.X,
			csbiConsole.dwMaximumWindowSize.Y,
			csbiConsole.dwSize.X,
			csbiConsole.dwSize.Y,
			csbiConsole.srWindow.Top,
			csbiConsole.srWindow.Bottom,
			csbiConsole.srWindow.Left,
			csbiConsole.srWindow.Right,
			csbiConsole.wAttributes);
		LOGNULL(sLog);
	}


	CONSOLE_SCREEN_BUFFER_INFO csbiCon;
	::GetConsoleScreenBufferInfo(m_hStdOut, &csbiCon);

	if (g_bTraceLog)
	{
		sLog.Format(_T("2-dwCursorPosition (X=%d-Y=%d) dwMaximumWindowSize (X=%d-Y=%d) dwSize (X=%d-Y=%d) srWindow (%d-%d-%d-%d) wAttributes (%d)"),
			csbiCon.dwCursorPosition.X,
			csbiCon.dwCursorPosition.Y,
			csbiCon.dwMaximumWindowSize.X,
			csbiCon.dwMaximumWindowSize.Y,
			csbiCon.dwSize.X,
			csbiCon.dwSize.Y,
			csbiCon.srWindow.Top,
			csbiCon.srWindow.Bottom,
			csbiCon.srWindow.Left,
			csbiCon.srWindow.Right,
			csbiCon.wAttributes);
		LOGNULL(sLog);
	}

	COORD		coordBufferSize;
	COORD		coordStart;
	SMALL_RECT	srRegion;

	coordStart.X		= 0;
	coordStart.Y		= 0;

	coordBufferSize.X	= m_dwBufferColumns;
	coordBufferSize.Y	= m_dwBufferRows;

	srRegion.Top		= csbiConsole.srWindow.Top;
	srRegion.Left		= 0;
	srRegion.Bottom		= csbiConsole.srWindow.Top + m_dwBufferRows - 1;
	srRegion.Right		= m_dwBufferColumns - 1;

	DEL_ARR(m_pScreenBuffer);
	m_pScreenBuffer = new CHAR_INFO[m_dwBufferRows * m_dwBufferColumns];

	if(!::ReadConsoleOutput(m_hStdOutFresh, m_pScreenBuffer, coordBufferSize, coordStart, &srRegion))
	{
		DEL_ARR(m_pScreenBuffer);
	}
}

/*****************************************************************
* Function: UpdateArrayData
* Description: update data from screen buffer to array: 1 line = 1 item
* Input values: 
* Output value:
******************************************************************/
void CConsole::GetArrayData(std::vector<CString>& vArrayData)
{
	try 
	{
		vArrayData.clear();

		if (!m_bInit)
		{
			LOGNULL(_T("Cannot get console data because console didn't inited"));
			return;
		}
		
		// get new content
		RefreshStdOut();
		RefreshScreenBuffer();

		if (!m_pScreenBuffer)
		{
			LOGNULL(_T("Cannot get console screen buffer because pointer is null"));
			return;
		}

		// read text of screen to vector
		DWORD dwOffset = 0;		
		for (SHORT i = 0; i < m_dwBufferRows; ++i)
		{
			CString strText(_T(""));
			strText = m_pScreenBuffer[dwOffset].Char.UnicodeChar;
			++dwOffset;

			for (SHORT j = 1; j < m_dwBufferColumns; ++j)
			{		
				strText += m_pScreenBuffer[dwOffset].Char.UnicodeChar;
				++dwOffset;
			}
			
			// push data to array
			vArrayData.push_back(strText);

			// logging
			strText.Trim();

			if (g_bTraceLog)
			{
				if (!strText.IsEmpty())
				{
					LOGNULL(strText);
				}
			}
		}
	}
	catch (...)
	{
		DWORD err = GetLastError();
		LOGERROR(_T("Exception"), err);
	}
}

/*****************************************************************
* Function: InitConsoleWndSize
* Description: using default size for console window rows=25,columns=80
* Input values: 
* Output value:
******************************************************************/
void CConsole::InitConsoleWndSize()
{
	COORD coordConsoleSize;
	coordConsoleSize.X = (SHORT)m_dwBufferColumns;
	coordConsoleSize.Y = (SHORT)m_dwBufferRows;

	SMALL_RECT	srConsoleRect;
	srConsoleRect.Top	= srConsoleRect.Left = 0;
	srConsoleRect.Right = m_dwBufferColumns - 1;
	srConsoleRect.Bottom = m_dwBufferRows - 1;

	CString sLog(_T(""));

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!::GetConsoleScreenBufferInfo(m_hStdOutFresh, &csbi))
	{
		sLog.Format(_T("Cannot get console screen buffer size (m_hStdOutFresh). Lasterr = %d"), GetLastError());
		LOGNULL(sLog);
	}

	// If the Current Buffer is Larger than what we want,  
	// Resize the Console Window First, then the Buffer
	if (csbi.dwSize.X * csbi.dwSize.Y > m_dwBufferColumns * m_dwBufferRows)
	{
		if (!::SetConsoleScreenBufferSize(m_hStdOutFresh, coordConsoleSize))
		{
			sLog.Format(_T("Cannot set console screen buffer size (1-m_hStdOutFresh). Lasterr = %d"), GetLastError());
			LOGNULL(sLog);
		}
		if (!::SetConsoleWindowInfo(m_hStdOutFresh, TRUE, &srConsoleRect))
		{
			sLog.Format(_T("Cannot set console window info (1-m_hStdOutFresh). Lasterr = %d"), GetLastError());
			LOGNULL(sLog);
		}
	} 

	// If the Current Buffer is Smaller than what we want, 
	// Resize the Buffer First, then the Console Window 
	if ((csbi.dwSize.X < m_dwBufferColumns) ||
		(csbi.dwSize.Y < m_dwBufferRows) ||
		((csbi.srWindow.Bottom - csbi.srWindow.Top + 1) != m_dwBufferRows))
	{
		if (!::SetConsoleScreenBufferSize(m_hStdOutFresh, coordConsoleSize))
		{
			sLog.Format(_T("Cannot set console screen buffer size (2-m_hStdOutFresh). Lasterr = %d"), GetLastError());
			LOGNULL(sLog);
		}

		if (!::SetConsoleWindowInfo(m_hStdOutFresh, TRUE, &srConsoleRect))
		{
			sLog.Format(_T("Cannot set console window info (2-m_hStdOutFresh). Lasterr = %d"), GetLastError());
			LOGNULL(sLog);
		}
	}
}

/*****************************************************************
* Function: SendTextToConsole
* Description: WINAPI thread monitor console event
* Input values: 
* Output value:
******************************************************************/
BOOL CConsole::SendTextToConsole(LPCTSTR pszText) 
{
	if (!pszText || (_tcslen(pszText) == 0))
	{
		m_strLastErrorMessage = _T("Text to send to console is empty.");
		m_dwLastErrorCode	  = GetLastError();
		return FALSE;
	}

	HANDLE hStdIn = ::CreateFile(_T("CONIN$"), 
								 GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 
								 NULL, 
								 OPEN_EXISTING, 
								 0, 
								 0);
	if (hStdIn == INVALID_HANDLE_VALUE)
	{
		m_strLastErrorMessage = _T("Cannot create file hStdIn");
		m_dwLastErrorCode	  = GetLastError();
		return FALSE;
	}

	DWORD	dwTextLen		= _tcslen(pszText);
	DWORD	dwTextWritten	= 0;

	INPUT_RECORD* pKeyEvents = new INPUT_RECORD[dwTextLen + 1];
	::ZeroMemory(pKeyEvents, sizeof(INPUT_RECORD)*(dwTextLen + 1));

	DWORD i = 0;
	for (i = 0; i < dwTextLen; ++i)
	{
		if (g_bTraceLog)
		{
			LOGNULL(CString(pszText[i]));
		}		

		pKeyEvents[i].EventType = KEY_EVENT;
		pKeyEvents[i].Event.KeyEvent.bKeyDown = TRUE;
		pKeyEvents[i].Event.KeyEvent.wRepeatCount = 1;
		pKeyEvents[i].Event.KeyEvent.wVirtualKeyCode = 0;
		pKeyEvents[i].Event.KeyEvent.wVirtualScanCode = 0;
		pKeyEvents[i].Event.KeyEvent.uChar.UnicodeChar = pszText[i];
		pKeyEvents[i].Event.KeyEvent.dwControlKeyState = 0;
	}

	// append Enter key
	i = dwTextLen;
	pKeyEvents[i].EventType = KEY_EVENT;
	pKeyEvents[i].Event.KeyEvent.bKeyDown = TRUE;
	pKeyEvents[i].Event.KeyEvent.wRepeatCount = 1;
	pKeyEvents[i].Event.KeyEvent.wVirtualKeyCode = VK_RETURN;
	pKeyEvents[i].Event.KeyEvent.wVirtualScanCode = 0;
	pKeyEvents[i].Event.KeyEvent.uChar.UnicodeChar = VK_RETURN;
	pKeyEvents[i].Event.KeyEvent.dwControlKeyState = 0;

	if (!::WriteConsoleInput(hStdIn, pKeyEvents, dwTextLen + 1, &dwTextWritten))
	{
		m_strLastErrorMessage = _T("Cannot write to console input");
		m_dwLastErrorCode	  = GetLastError();
		return FALSE;
	}

	DEL_ARR(pKeyEvents);
	::CloseHandle(hStdIn);
	
	return TRUE;
}


/*****************************************************************
* Function: GetLastErrorMessage
* Description: return last error message
* Input values: 
* Output value:
******************************************************************/
CString CConsole::GetLastErrorMessage()
{
	return m_strLastErrorMessage;
}

/*****************************************************************
* Function: GetLastErrorCode
* Description: return last error code
* Input values: 
* Output value:
******************************************************************/
DWORD CConsole::GetLastErrorCode()
{
	return m_dwLastErrorCode;
}

/*****************************************************************
* Function: MonitorThreadStatic
* Description: WINAPI thread monitor console event
* Input values: 
* Output value:
******************************************************************/
// DWORD WINAPI CConsole::MonitorThreadStatic(LPVOID lpParam) 
// {
// 	return ((CConsole*)lpParam)->MonitorThread();
// }

/*****************************************************************
* Function: MonitorThread
* Description: implement thread monitor wait: console process or console buffer output or quit event
* Input values: 
* Output value:
******************************************************************/
// DWORD CConsole::MonitorThread() 
// {
// 	HANDLE arrHandles[] = {m_hConsoleProcess, m_hQuitEvent, m_hStdOut};
// 
// 	while (TRUE) 
// 	{
// 		DWORD dwWait = ::WaitForMultipleObjects(3, arrHandles, FALSE, INFINITE);
// 
// 		if (dwWait == WAIT_OBJECT_0)
// 		{
// 			// console process had been stop
// 			m_strLastErrorMessage = _T("Console process stop.");
// 			m_dwLastErrorCode	  = 0;
// 			break;
// 		} 
// 		else if (dwWait == WAIT_OBJECT_0 + 1) 
// 		{
// 			// this object send event quit thread
// 			m_strLastErrorMessage = _T("Console object detroy and set quit event.");
// 			m_dwLastErrorCode	  = 0;
// 			break;
// 		} 
// 		else if (dwWait == WAIT_OBJECT_0 + 2)
// 		{
// 			// only once return with this event
// 			::Sleep(1000);
// 			::ResetEvent(m_hStdOut);
// 
// 			if (m_bInit)
// 			{				
// 				UpdateArrayData();
// 			}
// 		}
// 	}
// 
// 	return 0;
// }
// 
