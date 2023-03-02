#pragma once
#include <afxbutton.h>
class CIFIButton :
    public CMFCButton
{
public:
    CIFIButton();
    ~CIFIButton();
    DECLARE_MESSAGE_MAP()
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
};

