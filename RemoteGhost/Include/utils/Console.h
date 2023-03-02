// Copyright (c) 2015 Microtec. All rights reserved.

#ifndef MICROTEC_CONSOLE_H_
#define MICROTEC_CONSOLE_H_


#define DEL_ARR(p) if (p) {delete [] p; p = NULL;}
#define DEFAUL_CONSOLE_BUFFER_COLUMN 80
#define DEFAUL_CONSOLE_BUFFER_ROW	 40

class CConsole : public CObject
{
	DECLARE_DYNAMIC(CConsole);

public:
	CConsole(void);
	~CConsole(void);

private:
	// Console window procedure
	//static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	//void ReleaseHandles();

	BOOL StartShellProcess(CString processToken = _T(""));
	BOOL SendTextToConsole(LPCTSTR pszText);

	// shell activities monitor
	//static DWORD WINAPI MonitorThreadStatic(LPVOID lpParam);
	//DWORD MonitorThread();	

	void	GetArrayData(std::vector<CString>& vArrayData);
	CString GetLastErrorMessage();
	DWORD	GetLastErrorCode();

protected:
	BOOL InitHandles();
	BOOL RefreshStdOut();
	void RefreshScreenBuffer();	
	void InitConsoleWndSize();
	//void UpdateArrayData();

public:
	//static CCriticalSection m_cs;
	
	// init value
	BOOL m_bInit;

	// shell cmd.exe
	CString m_strShell;

	// last error message, last error code
	CString m_strLastErrorMessage;
	DWORD   m_dwLastErrorCode;

	// array of text line in console buffer
	//std::vector<std::string> m_vData;

	// buffer screen data
	CHAR_INFO*	m_pScreenBuffer;

	// console stdouts
	HANDLE	m_hStdOut;
	HANDLE	m_hStdOutFresh;

	// console window handle
	HWND	m_hWndConsole;	

	// set by monitor thread when detects console process exit
	HANDLE	m_hConsoleProcess;

	// handle to monitor thread
	//HANDLE	m_hMonitorThread;

	// set when quitting the application
	//HANDLE	m_hQuitEvent;

	// console rows & columns
	//DWORD	m_dwRows;
	SHORT	m_dwBufferColumns;
	SHORT	m_dwBufferRows;
};

#endif //MICROTEC_CONSOLE_H_