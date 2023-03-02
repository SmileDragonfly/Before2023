#include "stdafx.h"
#include "GraphicView.h"

CGraphicView::CGraphicView()
    :CIFICustomWnd(AfxGetInstanceHandle(),_T("CGraphicViewClassName"))
{
    m_bIsDrawBackGround = TRUE;
}

CGraphicView::~CGraphicView()
{
}

BEGIN_MESSAGE_MAP(CGraphicView, CIFICustomWnd)
    ON_WM_PAINT()
END_MESSAGE_MAP()

void CGraphicView::Draw(CDC* pDC)
{
    if (m_bIsDrawBackGround)
    {
        DrawBackGround(RGB(255, 255, 255), RGB(255, 255, 255), pDC);
        clock_t start = clock();
        for (int iRow = 0; iRow < 4; iRow++)
        {
            for (int iCol = 0; iCol < 4; iCol++)
            {
                clock_t start = clock();
                m_graphicItemMatrix[iRow][iCol].GetBackGroundData(pDC);
                float time = (clock() - start) / CLOCKS_PER_SEC;
                TRACE("Time item [%d*%d]: %0.12f\n", iRow, iCol, time);
            }
        }
        float time = (clock() - start) / CLOCKS_PER_SEC;
        TRACE("Total time:%0.9f\n", time);
        m_bIsDrawBackGround = FALSE;
    }
    else
    {
        clock_t start, s;
        float time, totalTime;
        s = clock();
        for (int iRow = 0; iRow < 4; iRow++)
        {
            for (int iCol = 0; iCol < 4; iCol++)
            {
                start = clock();
                m_graphicItemMatrix[iRow][iCol].Draw(pDC);
                time = float (clock() - start) / CLOCKS_PER_SEC;
                //TRACE("Item time: %f\n", time);
            }
        }
        totalTime = float(clock() - s) / CLOCKS_PER_SEC;
        //TRACE("Total time: %f\n\n", totalTime);
    }
}

void CGraphicView::UpdateChildrenOffset(CRect & rcBound)
{
    CRect rcBaseItem(0, 0, 250, 250);
    CRect rcItem;
    for (int iRow = 0; iRow < 4; iRow++)
    {
        for (int iCol = 0; iCol < 4; iCol++)
        {
            rcItem = rcBaseItem;
            rcItem.OffsetRect(iCol * 250, iRow * 250);
            m_graphicItemMatrix[iRow][iCol].SetRect(rcItem);
            m_graphicItemMatrix[iRow][iCol].SetPosition(iRow, iCol);
        }
    }
    rcBound = CRect(0, 0, 1000, 1000);
}
void CGraphicView::DrawBackGround(COLORREF backGroundColor, COLORREF circleColor, CDC* pDC)
{
    CRect rc;
    CRect rcOff(125, 125, 125, 125);
    GetClientRect(&rc);
    pDC->FillSolidRect(rc, backGroundColor);
    for (int i = 0; i < 5; i++)
    {
        DrawBackGroundCircle(pDC, circleColor, RGB(64, 64, 0), rc, 2);
        rc.DeflateRect(rcOff);
    }
    // Draw Axis
    CPen pen(PS_SOLID, 2, circleColor);
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->MoveTo(500, 0);
    pDC->LineTo(500, 1000);
    pDC->MoveTo(0, 500);
    pDC->LineTo(1000, 500);
}

void CGraphicView::DrawBackGroundCircle(CDC * pDC, COLORREF circleColor, COLORREF backGroundColor, CRect rc, unsigned int borderSize)
{
    CPen pen(PS_SOLID, borderSize, circleColor);
    CBrush brush(backGroundColor);
    CPen* pOldPen = pDC->SelectObject(&pen);
    CBrush* pOldBrush = pDC->SelectObject(&brush);
    pDC->Ellipse(&rc);
    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}

BOOL CGraphicView::UpdateData(TData * data, unsigned int size)
{
    if (data)
    {
        MoveToScreenAxis(data, size);
        AssignDataToGraphicItem(data, size);
        Invalidate();
        UpdateWindow();
        return TRUE;
    }
    Invalidate(FALSE);
    return FALSE;
}

void CGraphicView::MoveToScreenAxis(TData * data, unsigned int size)
{
    for (int i = 0; i < size; i++)
    {
        data[i].position.x += GRAPHIC_VIEW_WIDTH / 2;
        data[i].position.y = GRAPHIC_VIEW_HEIGHT / 2 - data[i].position.y;
    }
}

void CGraphicView::AssignDataToGraphicItem(TData * data, unsigned int size)
{
    unsigned int itemRow, itemColumn, pointRow, pointColumn;
    unsigned int rowSize = GRAPHIC_VIEW_HEIGHT / GRAPHIC_VIEW_ROW;
    unsigned int columnSize = GRAPHIC_VIEW_WIDTH / GRAPHIC_VIEW_COLUMN;
    for (int i = 0; i < size; i++)
    {
        itemRow = (int)(data[i].position.y / rowSize);
        itemColumn = (int)(data[i].position.x / columnSize);
        // Update data to graphic item
        pointRow = data[i].position.y - itemRow * rowSize;
        pointColumn = data[i].position.x - itemColumn * columnSize;
        m_graphicItemMatrix[itemRow][itemColumn].m_dataUpdate[m_graphicItemMatrix[itemRow][itemColumn].m_dataUpdateSize].color = data[i].color;
        m_graphicItemMatrix[itemRow][itemColumn].m_dataUpdate[m_graphicItemMatrix[itemRow][itemColumn].m_dataUpdateSize].position.x = data[i].position.x - m_graphicItemMatrix[itemRow][itemColumn].GetRect().left;
        m_graphicItemMatrix[itemRow][itemColumn].m_dataUpdate[m_graphicItemMatrix[itemRow][itemColumn].m_dataUpdateSize].position.y = data[i].position.y - m_graphicItemMatrix[itemRow][itemColumn].GetRect().top;
        m_graphicItemMatrix[itemRow][itemColumn].m_dataUpdateSize++;
        m_graphicItemMatrix[itemRow][itemColumn].m_bIsNeedToReDraw = TRUE;
    }
}
