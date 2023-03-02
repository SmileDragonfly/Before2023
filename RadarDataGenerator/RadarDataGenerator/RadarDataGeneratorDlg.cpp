
// RadarDataGeneratorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RadarDataGenerator.h"
#include "RadarDataGeneratorDlg.h"
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


// CRadarDataGeneratorDlg dialog



CRadarDataGeneratorDlg::CRadarDataGeneratorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RADARDATAGENERATOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_currentDrawPoint = 0;
}

void CRadarDataGeneratorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRadarDataGeneratorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_LBUTTONDOWN()
    ON_BN_CLICKED(IDOK, &CRadarDataGeneratorDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRadarDataGeneratorDlg message handlers

BOOL CRadarDataGeneratorDlg::OnInitDialog()
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRadarDataGeneratorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRadarDataGeneratorDlg::OnPaint()
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
        CClientDC dc(this);
        CRect rc;
        GetClientRect(&rc);
        TData point;

        // Create memory device context
        CDC memDC;
        CBitmap bitmap;
        memDC.CreateCompatibleDC(&dc);
        bitmap.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
        CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

        m_circleCenter.x = 250;
        m_circleCenter.y = 250;
        rc.left = 0;
        rc.top = 0;
        rc.right = 500;
        rc.bottom = 500;
        if (m_data.GetSize() > 0)
        {
            memDC.Ellipse(&rc);
            memDC.SetPixel(m_circleCenter.x, m_circleCenter.y, RGB(0, 0, 0));
            for (int i = 0; i < m_data.GetSize(); i++)
            {
                point = m_data.GetAt(i);
                memDC.SetPixel(point.position.x + m_circleCenter.x, m_circleCenter.y - point.position.y, point.color);
            }
        }
        else
        {
            memDC.Ellipse(&rc);
            memDC.SetPixel(m_circleCenter.x, m_circleCenter.y, RGB(0, 0, 0));
        }
        dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
        memDC.SelectObject(pOldBitmap);
        memDC.DeleteDC();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRadarDataGeneratorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRadarDataGeneratorDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CDialogEx::OnLButtonDown(nFlags, point);
    point.x = point.x - m_circleCenter.x;
    point.y = m_circleCenter.y - point.y;

    m_arrPoint.Add(point);

    m_createData.GenerateData(&m_arrPoint, 200, &m_data);
    Invalidate();
    UpdateWindow();
    m_arrPoint.RemoveAll();
    m_arrPoint.FreeExtra();
}


void CRadarDataGeneratorDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    FILE* pFile = fopen("RADARDATA.csv", "wb");
    m_createData.SaveDataToCSV(pFile, &m_data);
    fclose(pFile);
    AfxMessageBox(L"Data is stored to RADARDATA.csv");
    m_data.RemoveAll();
    m_data.FreeExtra();
}
