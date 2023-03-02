
// SeverIso8583AppDlg.h : header file
//

#pragma once
#include "ServerThread.h"

// CSeverIso8583AppDlg dialog
class CSeverIso8583AppDlg : public CDialogEx
{
// Construction
public:
	CSeverIso8583AppDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEVERISO8583APP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL OnSocketAccept(SOCKET socket);
	BOOL OnSocketClose(SOCKET socket);
	BOOL ReceiveDataFromSocket();
protected:
	SOCKET m_socket;
	CServerThread m_serverThread;
};
