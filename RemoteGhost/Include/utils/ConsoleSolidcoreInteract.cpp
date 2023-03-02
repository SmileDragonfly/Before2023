// Copyright (c) 2015 Microtec. All rights reserved.

#include "stdafx.h"
#include "ConsoleSolidcoreInteract.h"
#include <Tlhelp32.h>
#include <shlwapi.h>
#include <psapi.h>

IMPLEMENT_DYNAMIC(CConsoleSolidcoreInteract, CObject)

CConsoleSolidcoreInteract::CConsoleSolidcoreInteract()
{
	m_strPassword = _T("");
	m_strConfigPasswordAsk = _T("");
	m_strConfigPasswordWrong = _T("");
	m_strConfigPasswordTimeout = _T("");
	m_strBUText = _T("");
	m_strEUText = _T("");
	m_strStatusUpdate = _T("");
	m_strStatusEnabled = _T("");

	m_strLastErrorMessage = _T("");
	m_listData.RemoveAll();

	m_strShell = _T("");
	if (m_strShell.GetLength() == 0)
	{
		TCHAR szComspec[MAX_PATH];
		if (::GetEnvironmentVariable(_T("COMSPEC"), szComspec, MAX_PATH) > 0)
			m_strShell = szComspec;
		else
			m_strShell = _T("cmd.exe");
	}
}

CConsoleSolidcoreInteract::~CConsoleSolidcoreInteract()
{

}

void CConsoleSolidcoreInteract::SetCommandParams(CString strCommand, DWORD dwTime)
{
	m_strCommand = strCommand;
	m_dwTimeOut = dwTime;
}

void CConsoleSolidcoreInteract::SetSolidcoreParams(
	CString strPaswd,
	CString strConfigPasswordAsk,
	CString strConfigPasswordWrong,
	CString strConfigPasswordTimeout,
	CString strBUText,
	CString strEUText,
	CString strStatusUpdate,
	CString strStatusEnabled)
{
	m_strPassword = strPaswd;
	m_strConfigPasswordAsk = strConfigPasswordAsk;
	m_strConfigPasswordWrong = strConfigPasswordWrong;
	m_strConfigPasswordTimeout = strConfigPasswordTimeout;
	m_strBUText = strBUText;
	m_strEUText = strEUText;
	m_strStatusUpdate = strStatusUpdate;
	m_strStatusEnabled = strStatusEnabled;
}

/*****************************************************************
* Function: StartConsole
* Description: start shell command cmd.exe
* Input values: 
* Output value:
******************************************************************/
BOOL CConsoleSolidcoreInteract::StartConsole()
{
	TRY 
	{
		// parser command
		// find TOKEN_PROCESS: winlogon.exe or other process
		// Create process as user with token process
		CString processToken(_T(""));

// 		CStringArray vParams;
// 		ParseParameters(m_strCommand, _T(" "), vParams);
// 		int nSize = (int)vParams.GetSize();
// 		if (nSize > 0)
// 		{
// 			for (int i = 0; i < nSize; i++)
// 			{
// 				if (vParams[i].Find(_T("--token_process=")) >= 0)
// 				{
// 					processToken = vParams[i];
// 					processToken.Replace(_T("--token_process="), _T(""));
// 					processToken.Trim();
// 
// 					// update command
// 					m_strCommand.Replace(vParams[i], _T(""));
// 					m_strCommand.Trim();
// 
// 					CString sLog(_T(""));
// 					sLog.Format(_T("process token: %s. command: %s"), processToken, m_strCommand);
// 					LOGNULL(sLog);
// 
// 					break;
// 				}
// 			}
// 		}

		// start console
		if (!m_console.StartShellProcess(processToken))
		{
			m_strLastErrorMessage.Format(_T("Cannot start command shell. \r\nLastErrorMessage: \"%s\""), 
										 m_console.GetLastErrorMessage());
			LOGNULL(m_strLastErrorMessage);
			return FALSE;
		}
		else
		{
			m_strLastErrorMessage.Format(_T("Start command shell successfully"));
			LOGNULL(m_strLastErrorMessage);
			return TRUE;
		}
	}
	CATCH_ALL(e)
	{
		LOGEXCEPTION(*e, _T("Exception start"));
		return FALSE;
	}
	END_CATCH_ALL;
}

/*****************************************************************
* Function: SendCommand
* Description: send command to console
* Input values: 
* Output value:
******************************************************************/
int CConsoleSolidcoreInteract::SendSolidcoreCommand()
{
	TRY 
	{
		CString sLog(_T(""));
		sLog.Format(_T("Send command: %s"), m_strCommand);
		LOGNULL(sLog);
		
		// append Enter key
		CString sCommand(_T(""));
		sCommand = m_strCommand;

		// wait 5s and send command
		long nRetry = 50;
		while (nRetry > 0)
		{
			nRetry--;
			Sleep(100);
			if (!m_console.SendTextToConsole(sCommand))
			{
				m_strLastErrorMessage.Format(_T("Cannot send command: %s. \r\nLastErrorMessage: \"%s\""),
					m_strCommand,
					m_console.GetLastErrorMessage());
				LOGNULL(m_strLastErrorMessage);
				return -1;
			}
			else
				break;
		}

		//  [3-August-2015 HIEUTT]
		// read config for ask password
		CString sScAskPasswordText = m_strConfigPasswordAsk;
		sScAskPasswordText.MakeLower();
		sScAskPasswordText.Trim();
		LOGNULL(sScAskPasswordText);

		// wait and check return value
		std::vector<CString> vData;		

		nRetry = m_dwTimeOut/100 + 1;
		while (nRetry > 0)
		{
			nRetry --;
			Sleep(100);

			m_console.GetArrayData(vData);
			
			CString szOutput(_T(""));

			int nSize =0;
			nSize = (int) vData.size();
			for (int i=0; i < nSize; i++)
			{
				szOutput = vData.at(i).MakeLower();
				
				// Find a specific element.
				for (int i=0; i<m_listData.GetSize(); i++)
				{
					if (szOutput.Find(m_listData[i]) >= 0)
					{
						m_strLastErrorMessage.Format(_T("Solidcore \"%s\" success: \"%s\""), m_strCommand, szOutput);
						LOGNULL(m_strLastErrorMessage);
						return 1;
					}
				}
				
				if (szOutput.Find(sScAskPasswordText) >= 0)
				{
					m_strLastErrorMessage = _T("Solidcore need password.");
					LOGNULL(m_strLastErrorMessage);
					return 0;
				}
			}
		}
		
		m_strLastErrorMessage = _T("Solidcore timeout.");
		LOGNULL(m_strLastErrorMessage);
		return -1;		
	}
	CATCH_ALL(e)
	{
		m_strLastErrorMessage = _T("Exception send command");
		LOGEXCEPTION(*e, m_strLastErrorMessage);
		return -1;
	}
	END_CATCH_ALL;
}

/*****************************************************************
* Function: SendPassword
* Description: send password to console
* Input values: 
* Output value:
******************************************************************/
BOOL CConsoleSolidcoreInteract::SendSolidcorePassword()
{
	TRY 
	{
		CString sLog(_T(""));
		sLog.Format(_T("Password in config: \"%s\""), m_strPassword);
		LOGNULL(sLog);

		// decrypt passwd
		CString strPass(_T(""));
		strPass = SolidcorePasswordDecrypt();
		if (strPass.IsEmpty())
		{
			m_strLastErrorMessage.Format(_T("Password invalid: %s."), m_strCommand);
			LOGNULL(m_strLastErrorMessage);
			return FALSE;
		}
		
		// send passwd to console
		LOGNULL(_T("Send password 1st time."));
		long nRetry = 50;
		while (nRetry > 0)
		{
			nRetry--;
			Sleep(100);
			if (!m_console.SendTextToConsole(strPass))
			{
				m_strLastErrorMessage.Format(_T("Cannot send password 1st time: %s. \r\nLastErrorMessage: \"%s\""),
					m_strCommand,
					m_console.GetLastErrorMessage());
				LOGNULL(m_strLastErrorMessage);
				return FALSE;
			}
			else
				break;
		}

		//////////////////////////////////////////////////////////////////////////
		//  [3-August-2015 HIEUTT]
		// read config for ask password
		CString sScAskPasswordText = m_strConfigPasswordAsk;
		sScAskPasswordText.MakeLower();
		sScAskPasswordText.Trim();
		LOGNULL(sScAskPasswordText);

		//////////////////////////////////////////////////////////////////////////
		// read config for wrong password
		CString sScWrongPasswordText = m_strConfigPasswordWrong;
		sScWrongPasswordText.MakeLower();
		sScWrongPasswordText.Trim();
		LOGNULL(sScWrongPasswordText);

		if (sScWrongPasswordText.IsEmpty())
		{
			m_strLastErrorMessage = _T("Config for solidcore wrong password text is empty!");
			LOGNULL(m_strLastErrorMessage);
			return FALSE;
		}

		CStringArray vListWrongText;
		ParseParameters(sScWrongPasswordText, _T("|"), vListWrongText);
		int nCountW = (int)vListWrongText.GetSize();
		if (nCountW <= 0)
		{
			m_strLastErrorMessage = _T("Cannot parse solidcore wrong password text!");
			LOGNULL(m_strLastErrorMessage);
			return FALSE;
		}

		//////////////////////////////////////////////////////////////////////////
		// read config for timeout password
		CString sScTimeoutPasswordText = m_strConfigPasswordTimeout;
		sScTimeoutPasswordText.MakeLower();
		sScTimeoutPasswordText.Trim();
		LOGNULL(sScTimeoutPasswordText);

		if (sScTimeoutPasswordText.IsEmpty())
		{
			m_strLastErrorMessage = _T("Config for solidcore timeout password text is empty!");
			LOGNULL(m_strLastErrorMessage);
			return FALSE;
		}

		CStringArray vListTimeoutText;
		ParseParameters(sScTimeoutPasswordText, _T("|"), vListTimeoutText);
		int nCountT = (int)vListTimeoutText.GetSize();
		if (nCountT <= 0)
		{
			m_strLastErrorMessage = _T("Cannot parse solidcore timeout password text!");
			LOGNULL(m_strLastErrorMessage);
			return FALSE;
		}

		//////////////////////////////////////////////////////////////////////////
		// wait and check return value		
		std::vector<CString> vData;

		nRetry = m_dwTimeOut/1000 + 1;
		while (nRetry > 0)
		{
			nRetry --;
			Sleep(1000);

			// reset counter "Password:"
			int nCountPassword = 0;

			vData.clear();
			m_console.GetArrayData(vData);
			
			CString szOutput(_T(""));

			int nSize =0;
			nSize = (int) vData.size();
			for (int i=0; i < nSize; i++)
			{
				szOutput = vData.at(i).MakeLower();

				// Search result from console into list value
				for (int i=0; i<m_listData.GetSize(); i++)
				{
					if (szOutput.Find(m_listData[i]) >= 0)
					{
						m_strLastErrorMessage.Format(_T("Solidcore \"%s\" success: \"%s\""), m_strCommand, szOutput);
						LOGNULL(m_strLastErrorMessage);
						return TRUE;
					}
				}
				
				// Search result from console into list wrong pass
				for (int i=0; i<nCountW; i++)
				{
					if (szOutput.Find(vListWrongText[i]) >= 0)
					{
						m_strLastErrorMessage.Format(_T("Solidcore password wrong: \"%s\""), szOutput);
						LOGNULL(m_strLastErrorMessage);
						return FALSE;
					}
				}
								
				// Search result from console into list timeout
				for (int i=0; i<nCountT; i++)
				{
					if (szOutput.Find(vListTimeoutText[i]) >= 0)
					{
						m_strLastErrorMessage.Format(_T("Solidcore password timeout: \"%s\""), szOutput);
						LOGNULL(m_strLastErrorMessage);
						return FALSE;
					}
				}

				// Compare result with ask password text
				if (szOutput.Find(sScAskPasswordText) >= 0)
				{
					nCountPassword ++;
				}
			}

			// check if failed pass to many times
			if (nCountPassword > 1)
			{
				m_strLastErrorMessage.Format(_T("Solidcore password wrong."));
				LOGNULL(m_strLastErrorMessage);				

				// append 2 pass and Enter key for send
				LOGNULL(_T("Continue re-send password."));
				CString sSecondThirdSend(_T(""));
				sSecondThirdSend.Format(_T("%s\r%s\r"), strPass, strPass);

				long nRetrySecond = 50;
				while (nRetrySecond > 0)
				{
					nRetrySecond--;
					Sleep(100);
					if (!m_console.SendTextToConsole(sSecondThirdSend))
					{
						m_strLastErrorMessage.Format(_T("Cannot send password 2nd time: %s. \r\nLastErrorMessage: \"%s\""),
							m_strCommand,
							m_console.GetLastErrorMessage());
						LOGNULL(m_strLastErrorMessage);
					}
					else
						break;
				}

				return FALSE;
			}
		}

		m_strLastErrorMessage = _T("Solidcore timeout.");
		LOGNULL(m_strLastErrorMessage);
		return FALSE;		
	}
	CATCH_ALL(e)
	{
		m_strLastErrorMessage = _T("Exception send password");
		LOGEXCEPTION(*e, m_strLastErrorMessage);
		return FALSE;
	}
	END_CATCH_ALL;
}

/*****************************************************************
* Function: SendCommand
* Description: send command to console
* Input values: 
* Output value:
******************************************************************/
int CConsoleSolidcoreInteract::GetSolidcoreStatus()
{
	TRY 
	{
		CString sLog(_T(""));
		sLog.Format(_T("Send command: %s"), ____SC_STATUS);
		LOGNULL(sLog);

		// append Enter key
		CString sCommand(_T(""));
		sCommand.Format(_T("%s\r"), ____SC_STATUS);

		// wait 5s and send command "sadmin status"
		long nRetry = 50;
		while (nRetry > 0)
		{
			nRetry--;
			Sleep(100);
			if (!m_console.SendTextToConsole(sCommand))
			{
				m_strLastErrorMessage.Format(_T("Cannot send command: %s. \r\nLastErrorMessage: \"%s\""),
					m_strCommand,
					m_console.GetLastErrorMessage());
				LOGNULL(m_strLastErrorMessage);
				return -1;
			}
			else
				break;
		}

		//  [3-August-2015 HIEUTT]
		// read config status update mode
		CString sScStatusUpdateText = m_strStatusUpdate;
		sScStatusUpdateText.MakeLower();
		sScStatusUpdateText.Trim();
		LOGNULL(sScStatusUpdateText);

		// read config status enabled mode
		CString sScStatusEnabledText = m_strStatusEnabled;
		sScStatusEnabledText.MakeLower();
		sScStatusEnabledText.Trim();
		LOGNULL(sScStatusEnabledText);

		// wait and check return value
		std::vector<CString> vData;		

		nRetry = m_dwTimeOut/100 + 1;
		while (nRetry > 0)
		{
			nRetry --;
			Sleep(100);

			vData.clear();
			m_console.GetArrayData(vData);			

			CString szOutput(_T(""));

			int nSize =0;
			nSize = (int) vData.size();
			for (int i=0; i < nSize; i++)
			{
				szOutput = vData.at(i).MakeLower();

				// Find a specific element.
				if (szOutput.Find(sScStatusUpdateText) >= 0)
				{
					m_strLastErrorMessage.Format(_T("update mode (bu): \"%s\""), szOutput);
					LOGNULL(m_strLastErrorMessage);
					return 1;
				}
				else if (szOutput.Find(sScStatusEnabledText) >= 0)
				{
					m_strLastErrorMessage.Format(_T("enabled mode (eu): \"%s\""), szOutput);
					LOGNULL(m_strLastErrorMessage);
					return 0;
				}
			}
		}

		m_strLastErrorMessage = _T("Solidcore timeout.");
		LOGNULL(m_strLastErrorMessage);
		return -1;		
	}
	CATCH_ALL(e)
	{
		m_strLastErrorMessage = _T("Exception send command");
		LOGEXCEPTION(*e, m_strLastErrorMessage);
		return -1;
	}
	END_CATCH_ALL;
}

/*****************************************************************
* Function: SolidcorePasswordDecrypt
* Description: decrypt password
* Input values: 
* Output value:
******************************************************************/
CString CConsoleSolidcoreInteract::SolidcorePasswordDecrypt()
{
	CString strAfterDecrypt(_T(""));
	strAfterDecrypt = m_strPassword;

// 	if (!CCryptor::SimpleDecrypt(____SC_KEY_1___ , strAfterDecrypt))
// 	{
// 		strAfterDecrypt.Empty();
// 		LOGNULL(_T("____SC_KEY_1___ is invalid !"));
// 	}
	
	return strAfterDecrypt;
}

/*****************************************************************
* Function: SendCommandLine
* Description: send command to console
* Input values: 
* Output value:
******************************************************************/
BOOL CConsoleSolidcoreInteract::SendCommandLine(CString& strResult)
{
	TRY 
	{
		CString sLog(_T(""));
		sLog.Format(_T("Send command: %s"), m_strCommand);
		LOGNULL(sLog);

		// append Enter key
		CString sCommand(_T(""));
		sCommand = m_strCommand;

		// wait 5s and send command
		long nRetry = 50;
		while (nRetry > 0)
		{
			nRetry--;
			Sleep(100);
			if (!m_console.SendTextToConsole(sCommand))
			{
				m_strLastErrorMessage.Format(_T("Cannot send command: %s. \r\nLastErrorMessage: \"%s\""),
					m_strCommand,
					m_console.GetLastErrorMessage());
				LOGNULL(m_strLastErrorMessage);
				return FALSE;
			}
			else
				break;
		}
		
		// Get current module folder
		CString	strPath(_T(""));
		GetCurrentDirectory(MAX_PATH, strPath.GetBuffer(MAX_PATH));
		strPath.ReleaseBuffer();

		strPath.Trim(_T("\\"));
		if (strPath.IsEmpty())
		{
			m_strLastErrorMessage = _T("Send command error because cannot get current directory.");
			LOGNULL(m_strLastErrorMessage);
			return TRUE;		
		}
		else
		{
			strPath += _T(">");
						
			// trim command and get line counter of Command
			CString strFullCommandLine(_T(""));
			strFullCommandLine = strPath + m_strCommand;

			int nFullCommandCounter = 0;
			nFullCommandCounter = (strFullCommandLine.GetLength() / m_console.m_dwBufferColumns) + 1;

			std::vector<CString> vConsoleData;
			int nStart = 0, nEnd = 0, nCount = 0;

			// retry
			nRetry = m_dwTimeOut / 100 + 1;
			while (nRetry > 0)
			{
				nRetry --;
				Sleep(100);

				// get console text
				vConsoleData.clear();							
				m_console.GetArrayData(vConsoleData);
				nCount = (int)vConsoleData.size();
				
				//sLog.Format(_T("Console line counter: %d"), nCount);
				//LOGNULL(sLog);

				// search "C:\Symphony\SymphonyAgent>"
				BOOL bSetStart = FALSE;
				nStart = nEnd = 0;
				for (int i = 0; i<nCount; i++)
				{
					if (vConsoleData[i].Find(strPath) == 0)
					{
						if (!bSetStart)
						{
							bSetStart = TRUE;
							nStart = i + nFullCommandCounter;
							continue;
						}
						else
						{
							nEnd = i;
							break;
						}
					}
				}

				// result appearance, break and return
				if (nEnd > nStart)
					break;
			}

			sLog.Format(_T("nStart=%d - nEnd=%d"), nStart, nEnd);
			LOGNULL(sLog);

			// return
			if (nEnd > nStart)
			{
				// has result in console
				if (nEnd > nStart)
				{
					for (int i=nStart; i<nEnd; i++)
					{
						strResult += vConsoleData[i] + _T("\r\n");
					}
					strResult.Trim();
				}
				else
					strResult = _T("");
				
				m_strLastErrorMessage = _T("Send command successful.");

				// log result
				sLog.Format(_T("Result: \r\n%s. LastError: %s"), strResult, m_strLastErrorMessage);
				LOGNULL(sLog);
			}
			else
			{
				if (nCount > 0)
				{
					for (int i = 0; i < nCount; i++)
					{
						strResult += vConsoleData[i] + _T("\r\n");
					}
					strResult.Trim();
					m_strLastErrorMessage = _T("Command result has too long.");

					// log result
					sLog.Format(_T("Result: \r\n%s. LastError: %s"), strResult, m_strLastErrorMessage);
					LOGNULL(sLog);
				}
				else
				{
					// break by time out
					strResult = _T("Cannot get result from console");
					m_strLastErrorMessage = _T("Send command finish by timeout.");

					// log result
					sLog.Format(_T("Result: \r\n%s. LastError: %s"), strResult, m_strLastErrorMessage);
					LOGNULL(sLog);
				}			
			}			
		}
		
		return TRUE;		
	}
	CATCH_ALL(e)
	{
		m_strLastErrorMessage = _T("Exception send command");
		LOGEXCEPTION(*e, m_strLastErrorMessage);
		return FALSE;
	}
	END_CATCH_ALL;
}


BOOL CConsoleSolidcoreInteract::RunSolidCoreBU(CString& szMessage, std::vector<CString>& vConsoleData)
{
	vConsoleData.clear();

	// start console command window
	if (!StartConsole())
	{
		szMessage = m_strLastErrorMessage;
		return FALSE;
	}
	
	//  [3-August-2015 HIEUTT]
	// parse value send command from config
	CString sSolidcoreBUText(_T(""));
	sSolidcoreBUText = m_strBUText;
	sSolidcoreBUText.MakeLower();
	sSolidcoreBUText.Trim();
	LOGNULL(sSolidcoreBUText);

	if (sSolidcoreBUText.IsEmpty())
	{
		m_strLastErrorMessage = _T("Solidcore begin update text is empty!");
		szMessage = m_strLastErrorMessage;
		return FALSE;
	}

	m_listData.RemoveAll();
	ParseParameters(sSolidcoreBUText, _T("|"), m_listData);
	int nCount = (int)m_listData.GetSize();
	if (nCount <= 0)
	{
		m_strLastErrorMessage = _T("Cannot parse begin update text!");
		szMessage = m_strLastErrorMessage;
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	int iSend = SendSolidcoreCommand();
	switch (iSend)
	{
	case 0:
		{
			//////////////////////////////////////////////////////////////////////////
			// solidcore need password, continue send password
			// send password to solidcore
			szMessage = m_strLastErrorMessage;
			if (!SendSolidcorePassword())
			{
				szMessage = m_strLastErrorMessage;
				break;
			}
			szMessage = m_strLastErrorMessage;

			// get console text
			m_console.GetArrayData(vConsoleData);

			return TRUE;
		}
	case 1:
		{
			// solidcore return value ok, continue Installation package
			szMessage = m_strLastErrorMessage;

			// get console text
			m_console.GetArrayData(vConsoleData);

			return TRUE;
		}
	default:
		{
			szMessage = m_strLastErrorMessage;
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Check solidcore status after send command/password failed
	szMessage += _T(" Check status: ");
	iSend = GetSolidcoreStatus();
	switch (iSend)
	{
	case 0:
		{
			// solidcore status in eu mode, cannot install
			szMessage += m_strLastErrorMessage;
			break;
			//return FALSE;
		}
	case 1:
		{
			// solidcore status in bu mode, continue Installation package
			szMessage += m_strLastErrorMessage;
			break;
			//return TRUE;
		}
	default:
		{
			szMessage += m_strLastErrorMessage;
			break;
			//return FALSE;
		}
	}

	// get console text
	m_console.GetArrayData(vConsoleData);

	return FALSE;
}


BOOL CConsoleSolidcoreInteract::RunSolidCoreEU(CString& szMessage, std::vector<CString>& vConsoleData)
{	
	vConsoleData.clear();

	CString sLog(_T(""));

	//////////////////////////////////////////////////////////////////////////
	// start console command window
	if (!StartConsole())
	{
		szMessage = m_strLastErrorMessage;
		return FALSE;
	}

	//  [3-August-2015 HIEUTT]
	// parse value send command from config
	CString sSolidcoreEUText(_T(""));
	sSolidcoreEUText = m_strEUText;
	sSolidcoreEUText.MakeLower();
	sSolidcoreEUText.Trim();
	LOGNULL(sSolidcoreEUText);

	if (sSolidcoreEUText.IsEmpty())
	{
		m_strLastErrorMessage = _T("Solidcore end update text is empty!");
		szMessage = m_strLastErrorMessage;
		return FALSE;
	}

	m_listData.RemoveAll();
	ParseParameters(sSolidcoreEUText, _T("|"), m_listData);
	int nCount = (int)m_listData.GetSize();
	if (nCount <= 0)
	{
		m_strLastErrorMessage = _T("Cannot parse end update text!");
		szMessage = m_strLastErrorMessage;
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	int iSend = SendSolidcoreCommand();
	switch (iSend)
	{
	case 0:
		{
			//////////////////////////////////////////////////////////////////////////
			// solidcore need password, continue send password
			// send password to solidcore
			szMessage = m_strLastErrorMessage;
			if (!SendSolidcorePassword())
			{
				szMessage = m_strLastErrorMessage;
				break;
			}
			szMessage = m_strLastErrorMessage;

			// get console text
			m_console.GetArrayData(vConsoleData);

			return TRUE;
		}
	case 1:
		{
			// solidcore return value ok, continue Installation package
			szMessage = m_strLastErrorMessage;

			// get console text
			m_console.GetArrayData(vConsoleData);

			return TRUE;
		}
	default:
		{
			szMessage = m_strLastErrorMessage;
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Check solidcore status after send command/password failed
	szMessage += _T("Check solidcore status: ");
	iSend = GetSolidcoreStatus();
	switch (iSend)
	{
	case 0:
		{
			// solidcore status in eu mode, cannot install
			szMessage += m_strLastErrorMessage;
			break;
			//return FALSE;
		}
	case 1:
		{
			// solidcore status in bu mode, continue Installation package
			szMessage += m_strLastErrorMessage;
			break;
			//return TRUE;
		}
	default:
		{
			szMessage += m_strLastErrorMessage;
			break;
			//return FALSE;
		}
	}

	// get console text
	m_console.GetArrayData(vConsoleData);

	return FALSE;
}


BOOL CConsoleSolidcoreInteract::RunCommandLine(CString& szMessage, CString& szResult)
{
	//////////////////////////////////////////////////////////////////////////
	// start console command window
	// 	if (!StartConsole())
	// 	{
	// 		szMessage = m_strLastErrorMessage;
	// 		return FALSE;
	// 	}

	//////////////////////////////////////////////////////////////////////////
	//BOOL bSend = SendCommandLine(szResult);	
	//szMessage = m_strLastErrorMessage;
	//
	//return bSend;

	//////////////////////////////////////////////////////////////////////////
	// Reset return
	szResult = _T("");
	szMessage = _T("");

	CString sLog(_T(""));

	CString processToken(_T(""));
	BOOL bFind = FALSE;

	CString sCommandParam = m_strCommand;

	CStringArray vParams;
	ParseParameters(sCommandParam, _T(" "), vParams);
	int nSize = (int)vParams.GetSize();
	if (nSize > 0)
	{
		for (int i = 0; i < nSize; i++)
		{
			if (vParams[i].Find(_T("--token_process=")) >= 0)
			{
				processToken = vParams[i];
				processToken.Replace(_T("--token_process="), _T(""));
				processToken.Trim();

				// update command
				sCommandParam.Replace(vParams[i], _T(""));
				sCommandParam.Trim();
								
				sLog.Format(_T("process token: %s. command: %s"), processToken, sCommandParam);
				LOGNULL(sLog);

				bFind = TRUE;
				break;
			}
		}
	}

	// parser token process list: --token_process=applicationcore.exe|winlogon.exe
	CStringArray vTokens;
	ParseParameters(processToken, _T("|"), vTokens);
	int nTokens = (int)vTokens.GetSize();

	if (bFind == FALSE || nTokens <= 0)
	{
		// Run command with no Token
		CString strCommandRun;
		strCommandRun = m_strShell + _T(" /c ") + m_strCommand;

		sLog.Format(_T("Command execute: %s"), strCommandRun);
		LOGNULL(sLog);

		// create pipes to write and read data
		SECURITY_ATTRIBUTES secattr;
		ZeroMemory(&secattr, sizeof(secattr));
		secattr.nLength = sizeof(secattr);
		secattr.bInheritHandle = TRUE;

		HANDLE rPipe, wPipe;
		if (!CreatePipe(&rPipe, &wPipe, &secattr, 0))
		{			
			szMessage = _T("Cannot createpipe to read write console data");
			LOGNULL(szMessage);
			return FALSE;
		}

		// create process
		STARTUPINFO sInfo;
		ZeroMemory(&sInfo, sizeof(sInfo));
		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi));
		sInfo.cb = sizeof(sInfo);
		sInfo.dwFlags = STARTF_USESTDHANDLES;
		sInfo.hStdInput = NULL;
		sInfo.hStdOutput = wPipe;
		sInfo.hStdError = wPipe;

		//Create the process here.
		BOOL bCreate = CreateProcess((LPWSTR)(LPCTSTR)m_strShell,
			(LPWSTR)(LPCTSTR)strCommandRun,
			0,
			0,
			TRUE,
			0,
			0,
			0,
			&sInfo,
			&pi);

		if (!bCreate)
		{
			szMessage.Format(_T("Cannot execute command: %s"), strCommandRun);
			LOGNULL(szMessage);
			return FALSE;
		}

		//szMessage.Format(_T("Execute command successfully: %s."), (LPWSTR)(LPCTSTR)strFullCommand);
		//LOGNULL(szMessage);
		CloseHandle(wPipe);

		// now read the output pipe here.
// 		char buf[100];
// 		DWORD dwRead;
// 		CString m_csOutput(_T("")), csTemp;
// 		BOOL bRead;
// 		do
// 		{
// 			bRead = ::ReadFile(rPipe, buf, 100, &dwRead, 0);
// 			if (!bRead || dwRead == 0)
// 				break;
// 			csTemp = buf;
// 			m_csOutput += csTemp.Left(dwRead);
// 		} while (bRead);
		DWORD dwWait = WaitForSingleObject(pi.hProcess, m_dwTimeOut);
		if (dwWait == WAIT_TIMEOUT)
		{
			szMessage.Format(_T("Command time out: %s."), (LPWSTR)(LPCTSTR)strCommandRun);

			kill_process_tree(pi.dwProcessId);

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			return FALSE;
		}
		else
		{
			szMessage.Format(_T("Execute command successfully: %s."), (LPWSTR)(LPCTSTR)strCommandRun);

			CString m_csOutput(_T("")), csTemp;
			for (;;)
			{
				DWORD peekAvail = 0, peekLeftMessage, peekRead;
				char peekBuf[100 + 1] = { 0 };
				PeekNamedPipe(rPipe, peekBuf, 100, &peekRead, &peekAvail, &peekLeftMessage);
				if (peekAvail == 0)
				{
					break;
				}

				CHAR buf[100 + 1] = { 0 };
				DWORD dwRead;
				BOOL bRead = ::ReadFile(rPipe, buf, 100, &dwRead, 0);
				if (!bRead || dwRead == 0)
					break;
				csTemp = buf;
				m_csOutput += csTemp.Left(dwRead);
			}

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			szResult = m_csOutput;
			return TRUE;
		}
	}
	else
	{
		// Run command with Token
		if (sCommandParam.IsEmpty())
		{
			szMessage.Format(_T("Run command failed. Error: command param is null. Command=%s."), (LPWSTR)(LPCTSTR)sCommandParam);
			return FALSE;
		}

		//////////////////////////////////////////////////////////////////////////
		// change priority to access process
		ChangeProcessPriority();
		EnablePrivilege(SE_DEBUG_NAME);

		CString sErr(_T(""));
		CString sLog(_T(""));

		PROCESS_INFORMATION pi;
		STARTUPINFO si;
		BOOL bResult = FALSE;
		DWORD winlogonPid = 0;
		HANDLE hPToken, hProcess;

		// Find the WinLogon process
		PROCESSENTRY32 procEntry;

		HANDLE hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnap == INVALID_HANDLE_VALUE)
		{
			szMessage = _T("Run command failed. Error: CreateToolhelp32Snapshot failed.");
			return FALSE;
		}

		procEntry.dwSize = sizeof(PROCESSENTRY32);
		if (!::Process32First(hSnap, &procEntry))
		{
			szMessage = _T("Run command failed. Error: Process32First failed.");
			return FALSE;
		}

		BOOL bFindProcess = FALSE;
		do
		{
			for (int i = 0; i < nTokens; i++)
			{
				if (_tcsicmp(procEntry.szExeFile, vTokens[i]) == 0)	//|| _tcsicmp(procEntry.szExeFile, L"winlogon.exe") == 0)
				{
					// We found a winlogon process
					// Dont care to check sessionID, just get the process ID
					sLog.Format(_T("Found token: %s"), vTokens[i]);
					LOGNULL(sLog);

					winlogonPid = procEntry.th32ProcessID;
					bFindProcess = TRUE;
					break;
				}
			}

			if (bFindProcess)
			{
				break;
			}

		} while (::Process32Next(hSnap, &procEntry));

		// return if can not find process token
		if (!bFindProcess || winlogonPid == 0)
		{
			szMessage.Format(_T("Run command failed. Error: Cannot find process token: %s"), processToken);
			return FALSE;
		}

		// Get process token
		hProcess = ::OpenProcess(MAXIMUM_ALLOWED, FALSE, winlogonPid);
		if (!::OpenProcessToken(hProcess,
			TOKEN_ADJUST_PRIVILEGES |
			TOKEN_QUERY |
			TOKEN_DUPLICATE |
			TOKEN_ASSIGN_PRIMARY |
			TOKEN_ADJUST_SESSIONID |
			TOKEN_READ |
			TOKEN_WRITE,
			&hPToken))
		{
			// Just a sample of error message
			DWORD err = GetLastError();
			sErr = GetWin32ErrorMessage(err);
			szMessage.Format(_T("Run command failed. Error: OpenProcessToken failed. Error=%s"), sErr);
			return FALSE;
		}
		if (hProcess)
			::CloseHandle(hProcess);

		//////////////////////////////////////////////////////////////////////////
		// create pipe
		SECURITY_ATTRIBUTES secattr;
		ZeroMemory(&secattr, sizeof(secattr));
		secattr.nLength = sizeof(secattr);
		secattr.bInheritHandle = TRUE;

		HANDLE rPipe, wPipe;
		if (!CreatePipe(&rPipe, &wPipe, &secattr, 0))
		{
			szMessage = _T("Cannot createpipe to read write console data");
			LOGNULL(szMessage);
			return FALSE;
		}

		// Launch the process in the client's log on session.
		ZeroMemory(&pi, sizeof(pi));
		ZeroMemory(&si, sizeof(STARTUPINFO));

		si.cb = sizeof(STARTUPINFO);
		//si.lpDesktop = _T("winsta0\\default");
		//si.dwFlags |= STARTF_USESHOWWINDOW;
		
		si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;	// auto create cmd.exe hidden
		si.hStdInput = NULL;
		si.hStdOutput = wPipe;
		si.hStdError = wPipe;

		// Generate execute command
		CString strCommandRun(_T(""));
		strCommandRun.Format(_T("%s /c %s"), m_strShell, sCommandParam);

		bResult = ::CreateProcessAsUser(
			hPToken,        // client's access token
			(LPWSTR)(LPCTSTR)m_strShell,	// file to execute //szExeFile/
			(LPWSTR)(LPCTSTR)strCommandRun, // command line
			NULL,           // pointer to process SECURITY_ATTRIBUTES
			NULL,           // pointer to thread SECURITY_ATTRIBUTES
			TRUE, //FALSE,           // handles are not inheritable
			NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW,// creation flags
			NULL,           // pointer to new environment block 
			NULL,           // name of current directory 
			&si,			// pointer to STARTUPINFO structure
			&pi				// receives information about new process
			);

		// Error
		if (bResult == 0)
		{
			// Just a sample of error message
			DWORD err = GetLastError();
			sErr = GetWin32ErrorMessage(err);

			szMessage.Format(_T("Run command failed. Error: CreateProcessAsUser failed. Command=%s. Error=%s"), (LPWSTR)(LPCTSTR)strCommandRun, sErr);
			return FALSE;
		}
		else
		{
			CloseHandle(wPipe);

			// now read the output pipe here.
// 			char buf[100];
// 			DWORD dwRead;
// 			CString m_csOutput(_T("")), csTemp;
// 			BOOL bRead;
// 			do
// 			{
// 				bRead = ::ReadFile(rPipe, buf, 100, &dwRead, 0);
// 				if (!bRead || dwRead == 0)
// 					break;
// 				csTemp = buf;
// 				m_csOutput += csTemp.Left(dwRead);
// 			} while (bRead);

			DWORD dwWait = WaitForSingleObject(pi.hProcess, m_dwTimeOut);
			if (dwWait == WAIT_TIMEOUT)
			{
				szMessage.Format(_T("Command time out: %s."), (LPWSTR)(LPCTSTR)strCommandRun);

				kill_process_tree(pi.dwProcessId);

				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
				return FALSE;
			}
			else
			{
				szMessage.Format(_T("Execute command successfully: %s."), (LPWSTR)(LPCTSTR)strCommandRun);

				CString m_csOutput(_T("")), csTemp;
				for (;;)
				{
					DWORD peekAvail = 0, peekLeftMessage, peekRead;
					char peekBuf[100 + 1] = { 0 };
					PeekNamedPipe(rPipe, peekBuf, 100, &peekRead, &peekAvail, &peekLeftMessage);
					if (peekAvail == 0)
					{
						break;
					}

					CHAR buf[100 + 1] = { 0 };
					DWORD dwRead;
					BOOL bRead = ::ReadFile(rPipe, buf, 100, &dwRead, 0);
					if (!bRead || dwRead == 0)
						break;
					csTemp = buf;
					m_csOutput += csTemp.Left(dwRead);
				}

				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);

				szResult = m_csOutput;
				return TRUE;
			}
		}
	}
	return TRUE;
}


void CConsoleSolidcoreInteract::kill_process_tree(DWORD parentID)
{
	ChangeProcessPriority();
	EnablePrivilege(SE_DEBUG_NAME);

	CString sLog(_T(""));

	DWORD list[1024];
	int len = 0, i;

	list[len++] = parentID;

	len = get_process_tree(list, len, sizeof(list) / sizeof(*list));

	DWORD cbNeeded;
	HANDLE hProcess;
	HMODULE hMod;
	TCHAR szProcessName[MAX_PATH];
	for (i = len - 1; i >= 0; i--)
	{
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, list[i]);

		// Get the process name.
		if (hProcess)
		{
			if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
			{
				if (GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName)))
				{
					if (!TerminateProcess(hProcess, 0))
					{
						sLog.Format(_T("Cannot terminate process name %s. Error code = %i"), szProcessName, GetLastError());
						LOGNULL(sLog);
					}
					else
					{
						sLog.Format(_T("Terminated process name %s."), szProcessName);
						LOGNULL(sLog);
					}
				}
			}
		}
		CloseHandle(hProcess);
	}
}

int CConsoleSolidcoreInteract::get_process_tree(DWORD *list, int len, int max_len)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 entry;
	int i;

	memset(&entry, 0, sizeof(entry));
	entry.dwSize = sizeof(entry);

	if (!Process32First(snapshot, &entry))
		return len;

	do {
		for (i = len - 1; i >= 0; i--)
			if (list[i] == entry.th32ParentProcessID)
			{
				list[len++] = entry.th32ProcessID;
				if (len >= max_len)
				{
					// 					fprintf(stderr, "Warning: many pids "
					// 						"found, stopping...\n");
					return len;
				}
			}
	} while (Process32Next(snapshot, &entry));

	return len;
}