#pragma once
#include <afxext.h>
class CMySplitterWnd :
    public CSplitterWnd
{
public:
    CMySplitterWnd();
    ~CMySplitterWnd();
    DECLARE_MESSAGE_MAP()
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

