#pragma once

#include <vector>
#include <string>
#include <wtypes.h>


using namespace std;


#define ____ONE_GB	1073741824
#define ____ONE_MB	1048576
#define ____ONE_KB	1024

void LogEvent(LPCTSTR pszFormat, ...);

CString GetWin32ErrorMessage(DWORD& errcode);
CString GetExceptionMessage(CException &e);
CString GetCurrentPath();
CString GetFullPath(CString path);
CString MakeFullPath(CString fpath, CString fname);
BOOL CreateTreeFolder(CString lpszDirectory);

BOOL	GetPathInfo(CString path, CString *pdrive, CString *pdir, CString *pfname, CString *pext);
BOOL	GetPathInfo(CString path, CString *pdir, CString *pfname);
CString GetRootFolder();
void EmptyFolder(CString path, BOOL removeSubFolder = FALSE );




long ToNumber(CString s);
//DWORD ToNumber2(CString slargeNumber);

long ParseParameters(CString& params, CString splitter, std::vector<CString>& result);
long ParseParameters(CString& params, CString splitter, CStringArray& result);

CString ConvertULONGLONGToCString(ULONGLONG s);
CString ConvertCurrentDateTimeToString();
CStringA ConvertUnicodeToUTF8(wchar_t* pUnicode);
CStringW ConvertUTF8ToUnicode(char* pUtf8);

BOOL GetUncProcessPathA(__in LPCSTR lpszFullPath, __out LPSTR lpszPath, __in size_t nPath);
BOOL GetUserPathFromDevicePath(__in CString strFilename, __out CString& strUserPath, __in size_t nUserPath);
BOOL GetDevicePathFromUserPath(__in CString strFullPath, __out CString& strDevicePath, __in size_t nDevicePath);

BOOL Execute(CString command, DWORD timeout, CString* serr = NULL);
BOOL ExecuteCommandLine(CString sCommand, CString& sResult);

int  KillProcessByName(LPCTSTR fname);

#if (_MSC_VER >= 1700)
inline std::tm Localtime(const time_t &time_tt);
std::chrono::system_clock::time_point NextRotationTp(int rotation_hour = 0, int rotation_minute = 0);
#else
inline std::tm Localtime(const time_t &time_tt);
#endif

CHAR** WINAPI MBStringToMBArgv(LPSTR str, INT *pargc);


void ChangeProcessPriority();
BOOL EnablePrivilege(LPCTSTR PrviName);

CString FormatSize(DWORDLONG dwValue);

enum SRCH_TYPE_e
{
	ST_DLL = 10,
	ST_PROCESS
};

/**
*
* SEARCH_CRITERIA - Search criteria structure.
*
* @author :    Nibu babu thomas
* @version:    1.0            Date:  2007-05-13
*/
typedef struct SEARCH_CRITERIA
{
	SEARCH_CRITERIA() : eSrchType(ST_PROCESS)
	{}

	CString csSrch;
	SRCH_TYPE_e eSrchType;
}*PSEARCH_CRITERIA;

namespace Utils
{
	struct AutoHandleMgr
	{
		HANDLE m_hHandle;
		explicit AutoHandleMgr() : m_hHandle(0)
		{}

		explicit AutoHandleMgr(HANDLE hHandle_i) : m_hHandle(hHandle_i)
		{}

		~AutoHandleMgr()
		{
			Close();
		}

		operator HANDLE() const
		{
			return m_hHandle;
		}

		operator HANDLE&()
		{
			return m_hHandle;
		}

		void Attach(HANDLE& hHandleToAttach_io)
		{
			Close();
			m_hHandle = hHandleToAttach_io;
			hHandleToAttach_io = 0;
		}

		HANDLE Detach()
		{
			HANDLE hForReturn = m_hHandle;
			m_hHandle = 0;
			return hForReturn;
		}

		AutoHandleMgr& operator = (HANDLE hHandle_i)
		{
			if (m_hHandle != hHandle_i)
			{
				// Close previous
				Close();
				m_hHandle = hHandle_i;
			}
			return *this;
		}

		void Close()
		{
			// Close handle if and only if handle is valid
			IsValid() && CloseHandle(m_hHandle);
			m_hHandle = INVALID_HANDLE_VALUE;
		}

		bool operator !() const
		{
			return !m_hHandle;
		}

		bool IsValid() const
		{
			return m_hHandle && m_hHandle != INVALID_HANDLE_VALUE;
		}
	};
}

