
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
}

void CRadarProjectDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_GRAPHIC_VIEW, m_graphicView);
}

BEGIN_MESSAGE_MAP(CRadarProjectDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BUTTON_TEST, &CRadarProjectDlg::OnBnClickedButtonTest)
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
    m_graphicView.MoveWindow(0, 0, GRAPHIC_VIEW_WIDTH, GRAPHIC_VIEW_HEIGHT);

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
    //TTestData* data = m_testData.CreateLineData(g_azimuth, 200);
    if (g_count == 0)
    {
        COLORREF * pColor = m_testData.CreateColorArray(200);
        TTestData* data = m_testData.CreateLineDataWithColorInput(g_azimuth, 200, pColor);
        delete[] pColor;

        m_graphicView.UpdateData(data->data, data->size);
        m_testData.FreeTestData(data);
    }
    else
    {
        COLORREF * pColor = m_testData.CreateColorArray(200);
        TTestData* data = m_testData.CreateLineDataWithColorInput(g_azimuth, 200, pColor);
        delete[] pColor;

        m_graphicView.UpdateData(data->data, data->size);
        m_testData.FreeTestData(data);
    }

    g_azimuth += 1;
    if (g_azimuth >= 360)
    {
        g_azimuth = 0;
        m_testData.m_colorCreationCount = 0;
        KillTimer(nIDEvent);
        float time = (clock() - g_startClk)/CLOCKS_PER_SEC;
        g_startClk = clock();
        TRACE("SCANNING TIME: %f\n", time);
        g_count++;
    }
    CDialogEx::OnTimer(nIDEvent);
}


void CRadarProjectDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);    
}


void CRadarProjectDlg::OnBnClickedButtonTest()
{
    g_startClk = clock();
    SetTimer(1, 10, NULL);
}
