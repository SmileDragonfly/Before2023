#include "stdafx.h"
#include "IFIButton.h"


CIFIButton::CIFIButton()
{
}


CIFIButton::~CIFIButton()
{
}
BEGIN_MESSAGE_MAP(CIFIButton, CMFCButton)
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

void CIFIButton::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    this->m_nFlatStyle = BUTTONSTYLE_3D;
    CMFCButton::OnMouseMove(nFlags, point);
    TRACKMOUSEEVENT mouseEvent;
    mouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
    mouseEvent.dwFlags = TME_LEAVE;
    mouseEvent.hwndTrack = this->m_hWnd;
    mouseEvent.dwHoverTime = HOVER_DEFAULT;

    TrackMouseEvent(&mouseEvent);
}

void CIFIButton::OnMouseLeave()
{
    // TODO: Add your message handler code here and/or call default

    this->m_nFlatStyle = BUTTONSTYLE_NOBORDERS;
    CMFCButton::OnMouseLeave();
    this->RedrawWindow();
}
