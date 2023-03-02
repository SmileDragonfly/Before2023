// IFICustomWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IFICustomWnd.h"
#include "IFIGraphicUtils.h"

// CIFICustomWnd

//IMPLEMENT_DYNAMIC(CIFICustomWnd, CWnd)
#define ID_CHK_MOUSEOUT_TIMER	1

UINT CIFICustomWnd::GetMouseScrollLines()
{
    int nScrollLines = 3;            // reasonable default

    SystemParametersInfo (SPI_GETWHEELSCROLLLINES, 0, &nScrollLines, 0);

    return nScrollLines;
}

UINT CIFICustomWnd::GetItemFromPoint(CPoint point)
{
    return 0;
}

CIFICustomWnd::CIFICustomWnd(HINSTANCE hInstance,CString strClassName)
: m_strClassName(strClassName)
, m_bEnableAutoScroll(FALSE)
, m_bTimerOn(FALSE)
{
	RegisterWindowClass(hInstance);
    _AFX_THREAD_STATE* pState = AfxGetThreadState();
    if (!pState->m_bNeedTerm && !AfxOleInit())
        AfxMessageBox(_T("OLE initialization failed. Make sure that the OLE libraries are the correct version"));

	m_nBarState = FCSB_NONE;
    m_nVScrollMax         = 0;          // Scroll position
    m_nHScrollMax         = 0;
	m_nRowsPerWheelNotch  = GetMouseScrollLines();
	m_VirtualRect=CRect(0,0,0,0);
	m_roundsize.cx = 3;
	m_roundsize.cy = 3;
}

CIFICustomWnd::~CIFICustomWnd()
{
	
}

BOOL CIFICustomWnd::RegisterWindowClass(HINSTANCE hInstance)
{
    WNDCLASS wndcls;
	HINSTANCE hInst = (HINSTANCE)hInstance;

    if (!(::GetClassInfo(hInst, m_strClassName, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_HAND);
        wndcls.hbrBackground    = ::GetSysColorBrush(COLOR_WINDOW);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = m_strClassName;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CIFICustomWnd::Create(CWnd* pParentWnd, const RECT& rect, UINT nID, DWORD dwStyle /*=WS_VISIBLE*/)
{
	return CWnd::Create(m_strClassName, _T(""), dwStyle, rect, pParentWnd, nID);
}

BEGIN_MESSAGE_MAP(CIFICustomWnd, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
	//ON_WM_SETCURSOR()
	ON_WM_SIZE()
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

void CIFICustomWnd::PreSubclassWindow()
{
	CRect rc;
	GetWindowRect(rc);
	CRect rcCt(0,0,rc.Width()+1,rc.Height()+1);
	CRgn rgn;
	rgn.CreateRoundRectRgn(rcCt.left,rcCt.top,rcCt.right,rcCt.bottom,m_roundsize.cx,m_roundsize.cy);
	SetWindowRgn(static_cast<HRGN>(rgn.GetSafeHandle()),TRUE);
	rgn.DeleteObject();
	CWnd::PreSubclassWindow();
	if(m_bEnableAutoScroll)
	{
		ShowScrollBar(SB_VERT);
	}
	UpdateOffset();
}

int CIFICustomWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{		
	CWnd::OnCreate(lpCreateStruct);	

	UpdateOffset();

	return 1;
}
BOOL CIFICustomWnd::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
void CIFICustomWnd::OnNcPaint()
{
	DWORD dwStyle = GetWindowLong(m_hWnd,GWL_EXSTYLE);
	if(dwStyle & WS_EX_CLIENTEDGE)
	{
		CDC* pDC = GetWindowDC();
		CRect rc;
		GetWindowRect(rc);
		CRect rcCt(0,0,rc.Width()+1,rc.Height()+1);
		pDC->ExcludeClipRect(&rcCt);
		CRgn rgn;
		CWnd::OnNcPaint();
		rgn.CreateRoundRectRgn(rcCt.left,rcCt.top,rcCt.right,rcCt.bottom,m_roundsize.cx,m_roundsize.cy);
		pDC->SelectClipRgn(&rgn,RGN_COPY);
		pDC->FrameRgn(&rgn,&CBrush(RGB(200,200,200)),2,2);
		pDC->FrameRgn(&rgn,&CBrush(RGB(100,100,100)),1,1);
		
		rgn.DeleteObject();

		pDC->SelectClipRgn(NULL);
		ReleaseDC(pDC);
	}
	else
	{
		CWnd::OnNcPaint();
	}
}
void CIFICustomWnd::EnableAutoScroll(BOOL bEnable)
{
	m_bEnableAutoScroll = bEnable;
	ResetScrollBars();
}
void CIFICustomWnd::TransformPoint(CPoint& point)
{
	point.x+=GetScrollPos32(SB_HORZ);
	point.y+=GetScrollPos32(SB_VERT);
}

CRect CIFICustomWnd::TransformRect(CRect& rect)
{
	CRect resRect(rect);
	resRect.OffsetRect(-GetScrollPos32(SB_HORZ), -GetScrollPos32(SB_VERT));
	return resRect;
}

int CIFICustomWnd::GetScrollPos32(int nBar, BOOL bGetTrackPos /* = FALSE */)
{
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);

    if (bGetTrackPos)
    {
        if (GetScrollInfo(nBar, &si, SIF_TRACKPOS))
            return si.nTrackPos;
    }
    else
    {
        if (GetScrollInfo(nBar, &si, SIF_POS))
            return si.nPos;
    }

    return 0;
}


BOOL CIFICustomWnd::SetScrollPos32(int nBar, int nPos, BOOL bRedraw /* = TRUE */)
{
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_POS;
    si.nPos   = nPos;
    return SetScrollInfo(nBar, &si, bRedraw);
}

// Handle horz scrollbar notifications
void CIFICustomWnd::OnHScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar*/)
{
	if(!m_bEnableAutoScroll)
		return;
	int scrollPos = GetScrollPos32(SB_HORZ);

	CRect rect;
	GetClientRect(rect);

	switch (nSBCode)
	{
	case SB_LINERIGHT:
		{
			SetScrollPos32(SB_HORZ, (scrollPos<m_nHScrollMax-10) ? (scrollPos+10) :m_nHScrollMax );
			Invalidate();
		}
		break;
	case SB_PAGERIGHT:
		{
			SetScrollPos32(SB_HORZ, (scrollPos<m_nHScrollMax-rect.Width()) ? (scrollPos+rect.Width()) :m_nHScrollMax );
			Invalidate();
		}
		break;
	case SB_LINELEFT:
		{
			SetScrollPos32(SB_HORZ, (scrollPos>10) ? (scrollPos-10) :0 );
			Invalidate();
		}
		break;        
	case SB_PAGELEFT:
		{
			SetScrollPos32(SB_HORZ, (scrollPos>rect.Width()) ? (scrollPos-rect.Width()) :0 );
			Invalidate();
		}
		break;        
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		{
			SetScrollPos32(SB_HORZ, GetScrollPos32(SB_HORZ, TRUE));
			Invalidate();
		}
		break;

	case SB_LEFT:
		if (scrollPos > 0)
		{
			SetScrollPos32(SB_HORZ, 0);
			Invalidate();
		}
		break;

	case SB_RIGHT:
		if (scrollPos < m_nHScrollMax)
		{
			SetScrollPos32(SB_HORZ, m_nHScrollMax);
			Invalidate();
		}
		break;

	default: 
		break;
	}
}

// Handle vert scrollbar notifications
void CIFICustomWnd::OnVScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar*/)
{
	if(!m_bEnableAutoScroll)
		return;

	int scrollPos = GetScrollPos32(SB_VERT);
	CRect rect;
	GetClientRect(rect);

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		{
			SetScrollPos32(SB_VERT, (scrollPos<m_nVScrollMax-10) ? (scrollPos+10) : m_nVScrollMax );
			Invalidate();
		}
		break;
	case SB_PAGEDOWN:
		{
			SetScrollPos32(SB_VERT, (scrollPos<m_nVScrollMax-rect.Height()) ? (scrollPos+rect.Height()) : m_nVScrollMax );
			Invalidate();
		}
		break;
	case SB_LINEUP:
		{
			SetScrollPos32(SB_VERT, (scrollPos>10) ? (scrollPos-10) :0 );
			Invalidate();
		}
		break;
	case SB_PAGEUP:
		{
			SetScrollPos32(SB_VERT, (scrollPos>rect.Height()) ? (scrollPos-rect.Height()) :0 );
			Invalidate();
		}
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		{
			SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT, TRUE));
			Invalidate();
		}
		break;

	case SB_TOP:
		if (scrollPos > 0)
		{
			SetScrollPos32(SB_VERT, 0);
			Invalidate();
		}
		break;

	case SB_BOTTOM:
		if (scrollPos < m_nVScrollMax)
		{
			SetScrollPos32(SB_VERT, m_nVScrollMax);
			Invalidate();
		}

	default: 
		break;
	}
}

CRect CIFICustomWnd::GetVirtualRect()
{
	CRect rc(m_VirtualRect);
	if(IsVisibleHScroll())
		rc.bottom += GetSystemMetrics(SM_CYHSCROLL);
	
	return rc;
}

// If resizing or changes occur, call this - it'll fix up the scroll bars
void CIFICustomWnd::ResetScrollBars()
{
    if (!::IsWindow(GetSafeHwnd())) 
        return;
    
	if(!m_bEnableAutoScroll)
	{
		ShowScrollBar(SB_VERT,0);
		EnableScrollBar(SB_VERT,ESB_DISABLE_BOTH); 
		EnableScrollBarCtrl(SB_VERT,FALSE);		
		return;
	}

    CRect rect;
    GetClientRect(rect);
    
    if (rect.left == rect.right || rect.top == rect.bottom)
	{
		return;
	}
    
    if (IsVisibleVScroll())
        rect.right += GetSystemMetrics(SM_CXVSCROLL) + GetSystemMetrics(SM_CXBORDER);
    
    if (IsVisibleHScroll())
        rect.bottom += GetSystemMetrics(SM_CYHSCROLL) + GetSystemMetrics(SM_CYBORDER);
    
    if (rect.left >= rect.right || rect.top >= rect.bottom)
    {
        return;
    }

    CRect VisibleRect(0, 0, rect.right, rect.bottom);
    CRect VirtualRect(GetVirtualRect());
    
    // If vertical scroll bar, horizontal space is reduced
    if (VisibleRect.Height() < VirtualRect.Height())
        VisibleRect.right -= ::GetSystemMetrics(SM_CXVSCROLL);
    
    // Recheck vertical scroll bar
    if (VisibleRect.Height() < VirtualRect.Height())
    {
		EnableScrollBar(SB_VERT); 
		m_nBarState = FCSB_VERT;
        m_nVScrollMax = VirtualRect.Height() - 1;
    }
    else
    {
		m_nBarState = FCSB_NONE;
		EnableScrollBar(SB_VERT,ESB_DISABLE_BOTH); 
        m_nVScrollMax = 0;
    }


    ASSERT(m_nVScrollMax < INT_MAX); // This should be fine

    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_PAGE;
    si.nPage = (m_nVScrollMax>0)? VisibleRect.Height() : 0;
    SetScrollInfo(SB_VERT, &si, FALSE);

    SetScrollRange(SB_VERT, 0, m_nVScrollMax, TRUE);
}

BOOL CIFICustomWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if(!m_bEnableAutoScroll)
		return FALSE;
    // A m_nRowsPerWheelNotch value less than 0 indicates that the mouse
    // wheel scrolls whole pages, not just lines.
    if (m_nRowsPerWheelNotch == -1)
    {
        int nPagesScrolled = zDelta / 120;

        if (nPagesScrolled > 0)
            for (int n = 0; n < nPagesScrolled; n++)
                PostMessage(WM_VSCROLL, SB_PAGEUP, 0);
        else
            for (int n = 0; n > nPagesScrolled; n--)
                PostMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
    }
    else
    {
        int nRowsScrolled = m_nRowsPerWheelNotch * zDelta / 120;

        if (nRowsScrolled > 0)
            for (int i = 0; i < nRowsScrolled; i++)
                PostMessage(WM_VSCROLL, SB_LINEUP, 0);
        else
            for (int i = 0; i > nRowsScrolled; i--)
                PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);
    }
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL CIFICustomWnd::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
		switch ( (int)pMsg->wParam )
		{
			case VK_RIGHT:
			case VK_LEFT:
			case VK_UP:
			case VK_DOWN:
			case VK_DELETE:
				// dipatch keyboard messages directly (skip main accelerators)
				::TranslateMessage(pMsg);
				::DispatchMessage(pMsg);
				return TRUE;
		}
	
	return CWnd::PreTranslateMessage(pMsg);
}

void CIFICustomWnd::OnSize(UINT nType, int nCx, int nCy) 
{
    if (!::IsWindow(m_hWnd))
        return;
	CWnd::OnSize(nType, nCx, nCy);

	CRect rc;
	GetWindowRect(rc);
	CRect rcCt(0,0,rc.Width()+1,rc.Height()+1);
	CRgn rgn;
	rgn.CreateRoundRectRgn(rcCt.left,rcCt.top,rcCt.right,rcCt.bottom,m_roundsize.cx,m_roundsize.cy);
	SetWindowRgn(static_cast<HRGN>(rgn.GetSafeHandle()),TRUE);
	rgn.DeleteObject();

	UpdateOffset();	
}
void CIFICustomWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
    CBitmap  bitmap;      // Offscreen bitmap
	CBitmap* pOldBitmap;
	CRect rect;
	CRect clrect;
	CRect destrect;
    CDC MemDC;
    
	rect=GetVirtualRect();
	GetClientRect(clrect);
	destrect=clrect | rect;

	MemDC.CreateCompatibleDC(&dc);	
	MemDC.FillSolidRect(rect,::GetSysColor(COLOR_WINDOW));

	bitmap.CreateCompatibleBitmap(&dc, destrect.Width(), destrect.Height());
 	pOldBitmap = MemDC.SelectObject(&bitmap);

	MemDC.SetWindowOrg(0,0);
	MemDC.FillSolidRect(destrect, ::GetSysColor(COLOR_WINDOW));

	Draw(&MemDC);

	dc.BitBlt(rect.left, 
		rect.top, 
		clrect.Width(), 
		clrect.Height(),
		&MemDC, 
		GetScrollPos32(SB_HORZ),
		GetScrollPos32(SB_VERT),
		SRCCOPY);

	// Swap back the original bitmap.
	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
}

void CIFICustomWnd::UpdateOffset()
{
	if(IsWindow(m_hWnd))
	{
		UpdateChildrenOffset(m_VirtualRect);
	}
	
	ResetScrollBars();
}
void CIFICustomWnd::UpdateChildrenOffset(CRect &rcBound)
{
	if(IsWindow(m_hWnd))
	{
		CRect rcCL;
		GetClientRect(rcCL);
		CRect rc(rcCL);
		rc.DeflateRect(4,4,4,4);
		rcBound = rc;
	}
}
void CIFICustomWnd::Draw(CDC* pDC)
{
	pDC->SetBkMode(TRANSPARENT);	
}

void CIFICustomWnd::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == ID_CHK_MOUSEOUT_TIMER)
	{
		CPoint point;
		GetCursorPos(&point);
		CRect rc;
		GetWindowRect(rc);
		if(!rc.PtInRect(point))
		{
			InformMouseMove(0,CPoint(-100,-100));
			KillTimer(ID_CHK_MOUSEOUT_TIMER);
			m_bTimerOn = FALSE;
		}
	}
}
void CIFICustomWnd::OnMouseMove(UINT nFlags, CPoint point)
{	
	if(m_bTimerOn == FALSE)
	{
		SetTimer(ID_CHK_MOUSEOUT_TIMER,500,NULL);
		m_bTimerOn = TRUE;
	}

	CWnd::OnMouseMove(nFlags, point);

	TransformPoint(point);
	//CWnd *pWndFocused = GetFocus();
	//if(pWndFocused && pWndFocused->m_hWnd != m_hWnd)
	//{
		//SetFocus();
	//}

	InformMouseMove(nFlags,point);
}
void CIFICustomWnd::InformMouseMove(UINT nFlags, CPoint point)
{
}
void CIFICustomWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);
	
	TransformPoint(point);
	SetFocus();

	InformLButtonDown(nFlags,point);

	if(GetCapture() != this)
		SetCapture();
}
void CIFICustomWnd::InformLButtonDown(UINT nFlags, CPoint point)
{
}
void CIFICustomWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonUp(nFlags, point);
	
	TransformPoint(point);
	SetFocus();
	if(GetCapture() == this)
		::ReleaseCapture();

	InformLButtonUp(nFlags,point);
}
void CIFICustomWnd::InformLButtonUp(UINT nFlags, CPoint point)
{
}
void CIFICustomWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	//CWnd::OnRButtonDown(nFlags, point);
	
	TransformPoint(point);
	SetFocus();

	InformRButtonDown(nFlags,point);

	if(GetCapture() != this)
		SetCapture();
}
void CIFICustomWnd::InformRButtonDown(UINT nFlags, CPoint point)
{
}
void CIFICustomWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	//CWnd::OnRButtonUp(nFlags, point);
	
	TransformPoint(point);
	SetFocus();
	if(GetCapture() == this)
		::ReleaseCapture();

	InformRButtonUp(nFlags,point);
}
void CIFICustomWnd::InformRButtonUp(UINT nFlags, CPoint point)
{
}
//BOOL CIFICustomWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
//{	
//	return TRUE;
//}


