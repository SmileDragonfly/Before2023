#pragma once

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <new.h>
#include <signal.h>
#include <exception>
#include <sys/stat.h>
#include <psapi.h>
#include <rtcapi.h>
#include <Shellapi.h>
#include <dbghelp.h>

class CCrashHandler  
{
public:

    // Constructor
    CCrashHandler();

	// Constructor
	CCrashHandler(const wchar_t* szAppName);

    // Destructor
    virtual ~CCrashHandler();

	void SetAllExceptionHandlers();

    // Sets exception handlers that work on per-process basis
    void SetProcessExceptionHandlers();

    // Installs C++ exception handlers that function on per-thread basis
    void SetThreadExceptionHandlers();
private:
	static const wchar_t* m_szAppName;

	// Collects current process state.
    static void GetExceptionPointers(
        DWORD dwExceptionCode, 
        EXCEPTION_POINTERS** pExceptionPointers);

	// This method creates minidump of the process
	static void CreateMiniDump(EXCEPTION_POINTERS* pExcPtrs);

    /* Exception handler functions. */

    static LONG WINAPI SehHandler(PEXCEPTION_POINTERS pExceptionPtrs);
    static void __cdecl TerminateHandler();
    static void __cdecl UnexpectedHandler();

    static void __cdecl PureCallHandler();

    static void __cdecl InvalidParameterHandler(const wchar_t* expression, 
        const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved);

    static int __cdecl NewHandler(size_t);

    static void SigabrtHandler(int);
    static void SigfpeHandler(int /*code*/, int subcode);
    static void SigintHandler(int);
    static void SigillHandler(int);
    static void SigsegvHandler(int);
    static void SigtermHandler(int);

	static void TerminateProcess( IN HANDLE hProcess, IN UINT uExitCode);

	static CString GetDumpFilePath();
};


