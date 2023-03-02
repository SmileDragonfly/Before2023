#pragma once
#include "shared_queue.h"
#include <afxmt.h>
#include <afxwin.h>

// CLog

class CLog : public CWinThread
{
	struct DIR_REMOVE
	{
		int days;
		CString dir;
		CString namePattern;
	};

	DECLARE_DYNCREATE(CLog)

public:
	static CLog *Default;
	static CLog *CaptureLog;

	static CLog *DispenserLog;
	// [1/2/2020 VuongDV]
	static CLog *CameraLog;
	// [1/2/2020 VuongDV]

	static CLog *WhiteListLog;


protected:
	CLog (CString path=_T("Log"), DWORD maxlengthKB=1024);           // protected constructor used by dynamic creation
	virtual ~CLog();
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int Run();

	static CLog* New();	

	static void RemoveOldLogFile(int days, CString dir, CString namePrefix);
	static void CleanUpOldFile( int days, 
								CString path, 
								CString patternFileName, 
								BOOL cleanUpSubFolder = TRUE, 
								BOOL DeleteSubFolder = TRUE);

protected:
	CFile   m_file;
	CMutex  m_mutex;

	int		m_nDaysSaveLog; 

	CString	m_strCurrentLogDate;
	DWORD	m_maxlength;
	DWORD	m_logCounter;

	CString m_nameLog;
	CString m_path;
	CString m_nameLogPrefix;

	DWORD	m_error;
	BOOL	m_inited;

	//shared_queue<CString> mq_;
	shared_queue<std::string> mq_;

	CString GetCurrentDateTimeString();
	CString GetCurrentDateTimeString(CTime& Now);
	BOOL	SwitchNewLogFile();
	BOOL	FullLogFile();
	CString GetNewLogName(BOOL nestFolder = TRUE);
	BOOL	RenameFullLogFile(CString strCurrentLogFileName);
	BOOL    Init();

	int m_nEventLogCounter;
	void WriteEventLog(CString msg);
	void FlushCurrentQueue();

private:
	static DWORD WINAPI Thread_RemoveOldLogFile(LPVOID p);

public:
	void SetDirectoryLog(CString pathlog);
	void SetDaySaveLog(int days);
	void SetLogNamePrefix(CString logNamePrefix);

	void Log(LPCTSTR msg);	
	void LogError(LPCTSTR msg, DWORD error = 0);
	void Log(CException &e, LPCTSTR msg);

	void Log(CObject * pobj, LPCTSTR fname, LPCTSTR msg);	
	void LogError(CObject * pobj, LPCTSTR fname, LPCTSTR msg,DWORD error = 0);
	void Log(CObject * pobj, LPCTSTR fname, CException &e, LPCTSTR msg);

	void Log(CObject * pobj, LPCTSTR fname, long instance_code, LPCTSTR msg);	
	void LogError(CObject * pobj, LPCTSTR fname, long instance_code, LPCTSTR msg,DWORD error = 0);
	void Log(CObject * pobj, LPCTSTR fname, CException &e, long instance_code, LPCTSTR msg);

	void LogCore(std::string logState, std::string fname, std::string msg);
	void LogCap(std::string logState, std::string fname, std::string msg);

	void Log(std::string msg);
};


