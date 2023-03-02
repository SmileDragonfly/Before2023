#pragma once
#include <afxwin.h>
#include "MySplitterWnd.h"
class CSubFrameWnd :
    public CFrameWnd
{
public:
    CSubFrameWnd();
    DECLARE_DYNCREATE(CSubFrameWnd)
    ~CSubFrameWnd();
protected:
    CMySplitterWnd m_wndSplitter;
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};

