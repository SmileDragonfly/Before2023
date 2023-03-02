#include "stdafx.h"
#include "MySplitterWnd.h"


CMySplitterWnd::CMySplitterWnd()
{
}


CMySplitterWnd::~CMySplitterWnd()
{
}
BEGIN_MESSAGE_MAP(CMySplitterWnd, CSplitterWnd)
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


void CMySplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

}
