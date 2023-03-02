
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MFCApplication1.h"

#include "MainFrm.h"
#include "MFCApplication1View.h"
#include "SubFrameWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame() noexcept
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	EnableDocking(CBRS_ALIGN_ANY);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
    RECT screenWorkArea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &screenWorkArea, 0);
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_SYSMENU;
    cs.cx = screenWorkArea.right;
    cs.cy = 1042;
    cs.x = 0;
    cs.y = 0;
    // Delete Menu bar
    if (cs.hMenu)
    {
        ::DestroyMenu(cs.hMenu);      // delete menu if loaded
        cs.hMenu = NULL;              // no menu for this window
    }

	return TRUE;
}

// CMainFrame diagnostics
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif

CRect CMainFrame::GetLeftFrameSize()
{
    CRect rc;
    m_leftFrameWnd->GetClientRect(&rc);
    return rc;
}
CRect CMainFrame::GetRightFrameSize()
{
    CRect rc;
    m_rightFrameWnd->GetWindowRect(&rc);
    return rc;
}


// CMainFrame message handlers



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
    // TODO: Add your specialized code here and/or call the base class
    // create splitter window
    CRect crWnd, crClient;
    GetWindowRect(&crWnd);
    GetClientRect(&crClient);
    if (!m_wndSplitter.CreateStatic(this, 1, 2, WS_CHILD | WS_VISIBLE ))
        return FALSE;
    // Get current pane(0,0) position


    if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CMFCApplication1View), CSize(1000, 1000), pContext) ||
        !m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CSubFrameWnd), CSize(crClient.Width(), crClient.Height()), pContext))
    {
        m_wndSplitter.DestroyWindow();
        return FALSE;
    }
    m_leftFrameWnd = m_wndSplitter.GetPane(0, 0);
    m_rightFrameWnd = m_wndSplitter.GetPane(0, 1);
    return TRUE;
}
