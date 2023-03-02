#include "stdafx.h"
#include "AmplitudeView.h"


CAmplitudeView::CAmplitudeView()
    :CIFICustomWnd(AfxGetInstanceHandle(), _T("CAmplitudeViewClassName"))
{
    m_backgroundColor = RGB(255, 255, 255);
}


CAmplitudeView::~CAmplitudeView()
{
}

void CAmplitudeView::Draw(CDC * pDC)
{
    Graphics graphics(pDC->m_hDC);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    Pen blackPen(Color(0, 0, 0));
    CRect rc;
    GetClientRect(&rc);

    float xOffset, yOffset;
    float deltaY;
    int dataUpdateSize = m_arrDataToUpdate.GetSize();

    xOffset = (float) rc.right / dataUpdateSize;
    yOffset = 1.0;

    CPoint currentPoint(rc.left, rc.bottom - 25);
    CPoint prePoint;
    TData currData;

    for (int i = 0; i < dataUpdateSize; i++)
    {
        prePoint = currentPoint;
        currData = m_arrDataToUpdate.GetAt(i);
        if (i == 0)
        {
            deltaY = yOffset * currData.color;
        }
        else
        {
            deltaY = yOffset * (float)(currData.color - m_arrDataToUpdate.GetAt(i - 1).color);
        }
        currentPoint.x = (int)(xOffset * (i + 1));
        currentPoint.y -= deltaY;
        graphics.DrawLine(&blackPen, prePoint.x, prePoint.y, currentPoint.x, currentPoint.y);
    }
    m_arrDataToUpdate.RemoveAll();
    m_arrDataToUpdate.FreeExtra();
}

void CAmplitudeView::UpdateData(CArray<TData>* data)
{
    m_arrDataToUpdate.Copy(*data);
    Invalidate();
    UpdateWindow();
}

void CAmplitudeView::UpdateChildrenOffset(CRect & rcBound)
{
    GetClientRect(&rcBound);
}
