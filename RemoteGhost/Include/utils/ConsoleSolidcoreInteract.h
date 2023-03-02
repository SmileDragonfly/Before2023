// Copyright (c) 2015 Microtec. All rights reserved.

#ifndef MICROTEC_CONSOLE_SOLIDCORE_INTERACT_H_
#define MICROTEC_CONSOLE_SOLIDCORE_INTERACT_H_

/*********************************************
* Author: HieuTT
* Module: Symphony EJM SDM RKM
* Created: 02-July-2015
* Modified: 
* Change log:
* Description: communicate with cmd command console
**********************************************/
#include "Console.h"

//////////////////////////////////////////////////////////////////////////

#define ____SC_BEGIN_UPDATE			_T("sadmin bu")
#define ____SC_END_UPDATE			_T("sadmin eu")
#define ____SC_STATUS				_T("sadmin status")

// #define ____SC_ASK_PASSWORD			_T("Password:")
// #define ____SC_WRONG_PASSWORD		_T("Wrong password")
// #define ____SC_TIMEOUT_PASSWORD		_T("Password entry timed out")
// 
// #define ____SC_ENTER_UPDATE_MODE	_T("Entering update mode")
// #define ____SC_READY_IN_UPDATE_MODE _T("This operation is not allowed in update mode")
// #define ____SC_STATUS_UPDATE		_T("Update")
// 
// #define ____SC_EXIT_UPDATE_MODE		_T("Exiting update mode")
// #define ____SC_READY_IN_ENABLE_MODE _T("This operation is not allowed in enabled mode")
// #define ____SC_STATUS_ENABLE		_T("Enabled")


//  [14-July-2015 HIEUTT]
//-------- WINXP - WIN7 cmd command failed----------------------
/* 'sadmin' is not recognized as an internal or external command,
operable program or batch file.*/
// #define ____SC_DONT_INSTALL_1			_T("'sadmin' is not recognized as an internal or external command,")
// #define ____SC_DONT_INSTALL_2			_T("operable program or batch file.")
// 
// //-------- WINNT cmd command failed------------------------------
// /* The name specified is not recognized as an
// internal or external command, operable program or batch file.*/
// #define ____SC_DONT_INSTALL_WINNT_1		_T("The name specified is not recognized as an")
// #define ____SC_DONT_INSTALL_WINNT_2		_T("internal or external command, operable program or batch file.")
// 
// #define ____SC_SERVICE_NOT_RUNNING		_T("Service is not running")


enum SOLIDCORE_MODE
{
	ENTER_MODE, // sadmin bu
	EXIT_MODE	// sadmin eu
};

//////////////////////////////////////////////////////////////////////////

class CConsoleSolidcoreInteract : public CObject
{
	DECLARE_DYNAMIC(CConsoleSolidcoreInteract);

public:
	CConsoleSolidcoreInteract();
	~CConsoleSolidcoreInteract(void);
	void SetCommandParams(CString strCommand, DWORD dwTime);
	void SetSolidcoreParams(
		CString strPaswd,		
		CString strConfigPasswordAsk,
		CString strConfigPasswordWrong,
		CString strConfigPasswordTimeout,
		CString strBUText,
		CString strEUText,
		CString strStatusUpdate,
		CString strStatusEnabled);

public:
	BOOL RunSolidCoreBU(CString& szMessage, std::vector<CString>& vConsoleData);
	BOOL RunSolidCoreEU(CString& szMessage, std::vector<CString>& vConsoleData);
	BOOL RunCommandLine(CString& szMessage, CString& szResult);

private:
	CString m_strShell;
	CConsole m_console;

	CString m_strCommand;
	CString m_strPassword;
	DWORD   m_dwTimeOut;	

	CString m_strConfigPasswordAsk;
	CString m_strConfigPasswordWrong;
	CString m_strConfigPasswordTimeout;
	CString m_strBUText;
	CString m_strEUText;
	CString m_strStatusUpdate;
	CString m_strStatusEnabled;

	CString m_strLastErrorMessage;
	CStringArray m_listData;

	/*****************************************************************
	* Function: StartConsole
	* Description: start shell command cmd.exe
	* Input values: 
	* Output value:
	******************************************************************/
	BOOL StartConsole();

	/*****************************************************************
	* Function: SendSolidcoreCommand
	* Description: send command to console
	* Input values: 
	* Output value:
	******************************************************************/
	int  SendSolidcoreCommand();

	/*****************************************************************
	* Function: SendSolidcorePassword
	* Description: send password to console
	* Input values: 
	* Output value:
	******************************************************************/
	BOOL SendSolidcorePassword();

	/*****************************************************************
	* Function: GetSolidcoreStatus
	* Description: get solidcore status after send command/password failed
	* Input values: 
	* Output value:
	******************************************************************/
	BOOL GetSolidcoreStatus();

	/*****************************************************************
	* Function: SolidcorePasswordDecrypt
	* Description: decrypt password
	* Input values: 
	* Output value:
	******************************************************************/
	CString SolidcorePasswordDecrypt();

	/*****************************************************************
	* Function: SendCommandLine
	* Description: send normal command line
	* Input values: 
	* Output value:
	******************************************************************/
	BOOL SendCommandLine(CString& strResult);

	void kill_process_tree(DWORD parentID);
	int get_process_tree(DWORD *list, int len, int max_len);
};

#endif //MICROTEC_CONSOLE_SOLIDCORE_INTERACT_H_