
// RadarProjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RadarProject.h"
#include "RadarProjectDlg.h"
#include "afxdialogex.h"
#include "GraphicView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

clock_t g_startClk= 0;

CRadarProjectDlg::CRadarProjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RADARPROJECT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pFile = NULL;
}

void CRadarProjectDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_GRAPHIC_VIEW, m_graphicView);
    DDX_Control(pDX, IDC_AMPLITUDE_VIEW, m_amplitudeView);
    DDX_Control(pDX, IDC_BUTTON_ZOOM_IN, m_buttonZoomIn);
    DDX_Control(pDX, IDC_BUTTON_ZOOM_OUT, m_buttonZoomOut);
}

BEGIN_MESSAGE_MAP(CRadarProjectDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BUTTON_TEST, &CRadarProjectDlg::OnBnClickedButtonTest)
    ON_BN_CLICKED(IDC_BUTTON_ZOOM_IN, &CRadarProjectDlg::OnBnClickedButtonZoomIn)
    ON_BN_CLICKED(IDC_BUTTON_ZOOM_OUT, &CRadarProjectDlg::OnBnClickedButtonZoomOut)
END_MESSAGE_MAP()


// CRadarProjectDlg message handlers

BOOL CRadarProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    MoveWindow(0, 0, 1920, 1038);
    CRect rc;
    GetClientRect(&rc);
    m_graphicView.MoveWindow(0, 0, 1024, 1024);
    m_amplitudeView.MoveWindow(1004, 700, 900, 300);
    m_buttonZoomIn.MoveWindow(968, 930, 35, 35);
    m_buttonZoomOut.MoveWindow(968, 965, 35, 35);

    m_buttonZoomIn.SetImage(IDB_ZOOM_IN);
    m_buttonZoomIn.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
    m_buttonZoomIn.SizeToContent();
    m_buttonZoomIn.SetMouseCursorHand();
    m_buttonZoomIn.m_bDrawFocus = FALSE;

    m_buttonZoomOut.SetImage(IDB_ZOOM_OUT);
    m_buttonZoomOut.m_nFlatStyle = CMFCButton::BUTTONSTYLE_NOBORDERS;
    m_buttonZoomOut.SizeToContent();
    m_buttonZoomOut.SetMouseCursorHand();
    m_buttonZoomOut.m_bDrawFocus = FALSE;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRadarProjectDlg::OnPaint()
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
HCURSOR CRadarProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

float g_azimuth = 0;

void CRadarProjectDlg::OnTimer(UINT_PTR nIDEvent)
{    
    // [start] 28/5/2019: Read data from csv file
    if (nIDEvent == 1)
    {
        BOOL check = m_radarInput.ReadPacketFromFile(m_pFile);
        if (!check)
        {
            KillTimer(nIDEvent);
            AfxMessageBox(L"Done");
        }
    }
    else if (nIDEvent == 2)
    {
        m_graphicView.Invalidate();
    }

}


void CRadarProjectDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);    
}


void CRadarProjectDlg::OnBnClickedButtonTest()
{
    g_startClk = clock();
    m_pFile = fopen("data15_03_2019.txt", "rb");
    SetTimer(1, 1, NULL);
    SetTimer(2, 1000, NULL);
}


void CRadarProjectDlg::OnBnClickedButtonZoomIn()
{
    // TODO: Add your control notification handler code here
    m_graphicView.UpdateZoomInView();
}


void CRadarProjectDlg::OnBnClickedButtonZoomOut()
{
    // TODO: Add your control notification handler code here
    m_graphicView.UpdateZoomOutView();
}
