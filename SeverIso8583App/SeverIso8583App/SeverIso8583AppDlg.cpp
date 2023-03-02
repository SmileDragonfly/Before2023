
// SeverIso8583AppDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "SeverIso8583App.h"
#include "SeverIso8583AppDlg.h"
#include "afxdialogex.h"
#include "CommonDefinitions.h"
#include "CommonStructures.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSeverIso8583AppDlg dialog



CSeverIso8583AppDlg::CSeverIso8583AppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SEVERISO8583APP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_socket = 0;
}

void CSeverIso8583AppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSeverIso8583AppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CSeverIso8583AppDlg message handlers

BOOL CSeverIso8583AppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_serverThread.InitServerThread(1500, TPROTOCOL::TCP);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSeverIso8583AppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSeverIso8583AppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSeverIso8583AppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CSeverIso8583AppDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_SOCKET)
	{
		if (WSAGETSELECTERROR(pMsg->lParam))
		{
			AfxMessageBox(L"Socket failed with error %d", WSAGETSELECTERROR(pMsg->lParam));
		}
		else
		{
			switch (WSAGETSELECTEVENT(pMsg->lParam))
			{
			case FD_ACCEPT:
				if (OnSocketAccept(pMsg->wParam))
				{
					ReceiveDataFromSocket();
				}
				break;
			case FD_CLOSE:
				OnSocketClose(pMsg->wParam);
				break;
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CSeverIso8583AppDlg::OnSocketAccept(SOCKET socket)
{
	BOOL bRet = FALSE;
	m_socket = accept(socket, NULL, NULL);
	if (m_socket == INVALID_SOCKET)
	{
		AfxMessageBox(L"accept failed with error: %ld\n", WSAGetLastError());
	}
	else
	{
		bRet = TRUE;
	}
	return bRet;
}

BOOL CSeverIso8583AppDlg::OnSocketClose(SOCKET socket)
{
	return 0;
}

BOOL CSeverIso8583AppDlg::ReceiveDataFromSocket()
{
	if (m_socket != INVALID_SOCKET)
	{
		CHAR recvbuf[DEFAULT_BUFLEN];
		INT recvbuflen = DEFAULT_BUFLEN;
		INT iRecv = recv(m_socket, recvbuf, recvbuflen, 0);
		if (iRecv == 0)
		{
			AfxMessageBox(L"Socket was closed");
		}
	}
	return TRUE;
}
