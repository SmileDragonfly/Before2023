
// Swing_calculatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Swing_calculator.h"
#include "Swing_calculatorDlg.h"
#include "afxdialogex.h"

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


// CSwingcalculatorDlg dialog



CSwingcalculatorDlg::CSwingcalculatorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SWING_CALCULATOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSwingcalculatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSwingcalculatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSwingcalculatorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDABOUT, &CSwingcalculatorDlg::OnBnClickedAbout)
END_MESSAGE_MAP()


// CSwingcalculatorDlg message handlers

BOOL CSwingcalculatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Set Default to High
	CButton* pHighRadio = (CButton*)GetDlgItem(IDC_HIGH_RADIO);
	pHighRadio->SetCheck(TRUE);
	// Set title
	CWnd * pWnd = AfxGetMainWnd();
	pWnd->SetWindowTextW(L"Swing Calculator");
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSwingcalculatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSwingcalculatorDlg::OnPaint()
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
HCURSOR CSwingcalculatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSwingcalculatorDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	// CDialogEx::OnOK();
	CButton* pHighRadio = (CButton*) GetDlgItem(IDC_HIGH_RADIO);
	CButton* pLowRadio = (CButton*) GetDlgItem(IDC_LOW_RADIO);
	CEdit* pEditSwing = (CEdit*) GetDlgItem(IDC_SWING);
	CEdit* pEditFactor = (CEdit*) GetDlgItem(IDC_FACTOR);
	CEdit* pEditResult[8];
	pEditResult[0] = (CEdit*) GetDlgItem(IDC_RESULT1);
	pEditResult[1] = (CEdit*) GetDlgItem(IDC_RESULT2);
	pEditResult[2] = (CEdit*) GetDlgItem(IDC_RESULT3);
	pEditResult[3] = (CEdit*) GetDlgItem(IDC_RESULT4);
	pEditResult[4] = (CEdit*) GetDlgItem(IDC_RESULT5);
	pEditResult[5] = (CEdit*) GetDlgItem(IDC_RESULT6);
	pEditResult[6] = (CEdit*) GetDlgItem(IDC_RESULT7);
	pEditResult[7] = (CEdit*) GetDlgItem(IDC_RESULT8);

	CString sEditSwing;
	pEditSwing->GetWindowTextW(sEditSwing);
	unsigned int iEditSwing = _wtoi(sEditSwing);
	CString sEditFactor;
	pEditFactor->GetWindowTextW(sEditFactor);
	unsigned int iEditFactor = _wtoi(sEditFactor);

	float fEditResult[8];
	std::wstring sEditResult[8];

	if (pHighRadio->GetCheck())
	{
		fEditResult[0] = pow(sqrt(iEditSwing) - iEditFactor, 2);
		sEditResult[0] = std::to_wstring(fEditResult[0]);
		pEditResult[0]->SetWindowTextW(sEditResult[0].c_str());
		for(int i = 0; i < 7; i++)
		{
			fEditResult[i + 1] = pow(sqrt(fEditResult[i]) - iEditFactor, 2);
			sEditResult[i + 1] = std::to_wstring(fEditResult[i + 1]);
			pEditResult[i + 1]->SetWindowTextW(sEditResult[i + 1].c_str());
		}
	}
	else
	{
		fEditResult[0] = pow(sqrt(iEditSwing) + iEditFactor, 2);
		sEditResult[0] = std::to_wstring(fEditResult[0]);
		pEditResult[0]->SetWindowTextW(sEditResult[0].c_str());
		for (int i = 0; i < 7; i++)
		{
			fEditResult[i + 1] = pow(sqrt(fEditResult[i]) + iEditFactor, 2);
			sEditResult[i + 1] = std::to_wstring(fEditResult[i + 1]);
			pEditResult[i + 1]->SetWindowTextW(sEditResult[i + 1].c_str());
		};
	}
}


void CSwingcalculatorDlg::OnBnClickedAbout()
{
	// TODO: Add your control notification handler code here
	CAboutDlg cAboutDlg;
	cAboutDlg.DoModal();
}
