#ifndef IFICUSTOMWND_H
#define IFICUSTOMWND_H
#pragma once

#define IFICUSTOMWND_CLASSNAME _T("IFICustomWnd")
// CIFICustomWnd

#ifndef FCSB_NONE
#define FCSB_NONE				0L
#define FCSB_HORZ				1L
#define FCSB_VERT				2L
#define FCSB_BOTH				3L
#endif

class CIFICustomWnd : public CWnd
{
	//DECLARE_DYNAMIC(CIFICustomWnd)
public:
	CIFICustomWnd(HINSTANCE hInstance,CString strClassName = IFICUSTOMWND_CLASSNAME);
	virtual ~CIFICustomWnd();

	BOOL Create(CWnd* pParentWnd, const RECT& rect, UINT nID, DWORD dwStyle = WS_VISIBLE);
	CSize m_roundsize;
protected:
	virtual BOOL	RegisterWindowClass(HINSTANCE hInstance);

protected:
	CString m_strClassName;
	/*Used to control display and scroll*/
	BOOL m_bEnableAutoScroll;
	int	m_nBarState;
    int m_nVScrollMax;
	int m_nHScrollMax;
	int m_nRowsPerWheelNotch;
	CRect m_VirtualRect;	
	BOOL	m_bTimerOn;
protected:
	void	EnableAutoScroll(BOOL bEnable = TRUE);
	UINT    GetMouseScrollLines();
	UINT	GetItemFromPoint(CPoint point);	
	CRect	GetVirtualRect();
	void	ResetScrollBars();
	int		GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
    BOOL	SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);
    BOOL	IsVisibleVScroll() { return ( (m_nBarState & FCSB_VERT) > 0); } 
    BOOL	IsVisibleHScroll() { return ( (m_nBarState & FCSB_HORZ) > 0); }
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:	
	void	TransformPoint(CPoint& point);
	CRect	TransformRect(CRect& rect);
	virtual void Draw(CDC* pDC);
	virtual void UpdateOffset();
	virtual void UpdateChildrenOffset(CRect &rcBound);
	virtual void PreSubclassWindow();
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int nCx, int nCy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void InformMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void InformLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void InformLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void InformRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void InformRButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};


#endif 