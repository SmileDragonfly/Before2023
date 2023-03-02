#include "stdafx.h"
#include "SubFrameWnd.h"
#include "MFCApplication1View.h"

IMPLEMENT_DYNCREATE(CSubFrameWnd, CFrameWnd)
CSubFrameWnd::CSubFrameWnd()
{
}


CSubFrameWnd::~CSubFrameWnd()
{
}


BOOL CSubFrameWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
    // TODO: Add your specialized code here and/or call the base class
    // create splitter window
    CRect cr;
    GetWindowRect(&cr);
    if (!m_wndSplitter.CreateStatic(this, 3, 1, WS_CHILD | WS_VISIBLE))
        return FALSE;

    if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CMFCApplication1View), CSize(cr.Width(), cr.Height() / 3), pContext) ||
        !m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CMFCApplication1View), CSize(cr.Width(), cr.Height() / 3), pContext) ||
        !m_wndSplitter.CreateView(2, 0, RUNTIME_CLASS(CMFCApplication1View), CSize(cr.Width(), cr.Height() / 3), pContext))
    {
        m_wndSplitter.DestroyWindow();
        return FALSE;
    }

    return TRUE;;
}
