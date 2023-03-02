#include "stdafx.h"
#include "GraphicView.h"
#include "gdiplus.h"

CGraphicView::CGraphicView()
    :CIFICustomWnd(AfxGetInstanceHandle(),_T("CGraphicViewClassName"))
{
    m_backgroundColor = RGB(255, 255, 255);
    m_centerPoint.x = RADAR_DISPLAY_SIZE / 2 + BORDER_SIZE;
    m_centerPoint.y = RADAR_DISPLAY_SIZE / 2 + BORDER_SIZE;
    int radius = RADAR_DISPLAY_SIZE / 2;
    for(int i = 0; i < 12; i++ )
    {
        float angle = 30 * i * M_PI / 180;
        // m_centerPoint is origin
        m_arrBGLineEndPoint[i].x = (int)(radius * sin(angle));
        m_arrBGLineEndPoint[i].y = (int)(radius * cos(angle));
        // client origin
        m_arrBGLineEndPoint[i].x = m_centerPoint.x + m_arrBGLineEndPoint[i].x;
        m_arrBGLineEndPoint[i].y = -m_arrBGLineEndPoint[i].y + m_centerPoint.y;
    }
    m_currAzumith = -1;
    m_packetCounter = 0;
}

CGraphicView::~CGraphicView()
{
}

BEGIN_MESSAGE_MAP(CGraphicView, CIFICustomWnd)
    ON_WM_ERASEBKGND()
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_PAINT()
END_MESSAGE_MAP()

void CGraphicView::Draw(CDC* pDC)
{
    for (int iRow = 0; iRow < GRAPHIC_VIEW_ROW; iRow++)
    {
        for (int iCol = 0; iCol < GRAPHIC_VIEW_COLUMN; iCol++)
        {
            m_graphicItemMatrix[iRow][iCol].Draw(pDC);
        }
    }
    //if (m_arrDataUpdate.GetSize())
    //{
    //    int size = m_arrDataUpdate.GetSize();
    //    TData currData;
    //    for (int i = 0; i < size; i++)
    //    {
    //        currData = m_arrDataUpdate.GetAt(i);
    //        if (currData.color != 0)
    //        {
    //            pDC->SetPixel(currData.position, currData.color);
    //        }
    //        else
    //        {
    //            pDC->SetPixel(currData.position, RGB(255, 255, 255));
    //        }
    //    }
    //}
}

void CGraphicView::UpdateChildrenOffset(CRect & rcBound)
{
    GridGraphicsView(GRAPHIC_VIEW_ROW, GRAPHIC_VIEW_COLUMN);
    rcBound = CRect(0, 0, RADAR_REAL_SIZE, RADAR_REAL_SIZE);
}

BOOL CGraphicView::UpdateData()
{
    Invalidate();
    UpdateWindow();
    return TRUE;
}

void CGraphicView::GridGraphicsView(int row, int column)
{
    int rowSize = RADAR_REAL_SIZE / row;
    int colSize = RADAR_REAL_SIZE / column;
    m_graphicItemMatrix = new CGraphicItem*[row];
    for (int i = 0; i < row; i++)
    {
        m_graphicItemMatrix[i] = new CGraphicItem[column];
    }

    CRect rcBaseItem(0, 0, RADAR_REAL_SIZE / GRAPHIC_VIEW_ROW, RADAR_REAL_SIZE / GRAPHIC_VIEW_COLUMN);
    CRect rcItem;
    for (int iRow = 0; iRow < GRAPHIC_VIEW_ROW; iRow++)
    {
        for (int iCol = 0; iCol < GRAPHIC_VIEW_COLUMN; iCol++)
        {
            rcItem = rcBaseItem;
            rcItem.OffsetRect(iCol * RADAR_REAL_SIZE / GRAPHIC_VIEW_ROW, iRow * RADAR_REAL_SIZE / GRAPHIC_VIEW_COLUMN);
            m_graphicItemMatrix[iRow][iCol].SetRect(rcItem);
            m_graphicItemMatrix[iRow][iCol].SetPosition(iRow, iCol);
        }
    }
}

void CGraphicView::UpdateAzimuth(CArray<TData>* data)
{
    int dataSize = data->GetSize();
    TData currData = data->GetAt(dataSize - 1);
    float diagonal = sqrt(pow(currData.position.x, 2) + pow(currData.position.y, 2));
    if (currData.position.x > 0)
    {
        m_currAzumith = M_PI_2 - asin((float)currData.position.y / diagonal);
    }
    else
    {
        m_currAzumith = M_PI_2 + M_PI + asin((float)currData.position.y / diagonal);
    }
    m_currAzumith = m_currAzumith * 180 / M_PI;
}

void CGraphicView::DrawBackGroundLine(CDC * pDC, float angle, Color color)
{
    Graphics gdiplus(pDC->m_hDC);
    // Set smooth draw
    gdiplus.SetSmoothingMode(SmoothingModeAntiAlias);
    Pen pen(color);
    CPoint endPoint;
    float radius = RADAR_DISPLAY_SIZE / 2;
    angle = angle * M_PI / 180;
    // m_centerPoint is origin
    endPoint.x = (int)(radius * sin(angle));
    endPoint.y = (int)(radius * cos(angle));
    // client origin
    endPoint.x = m_centerPoint.x + endPoint.x;
    endPoint.y = - endPoint.y + m_centerPoint.y;

    gdiplus.DrawLine(&pen, m_centerPoint.x, m_centerPoint.y, endPoint.x, endPoint.y);
}

void CGraphicView::DrawBackGroundLine(CDC* pDC, Color color)
{
    Graphics gdiplus(pDC->m_hDC);
    // Set smooth draw
    gdiplus.SetSmoothingMode(SmoothingModeAntiAlias);
    Pen pen(color);
    for (int i = 0; i < 12; i++)
    {
        gdiplus.DrawLine(&pen, m_centerPoint.x, m_centerPoint.y, m_arrBGLineEndPoint[i].x, m_arrBGLineEndPoint[i].y);
    }
}

void CGraphicView::DrawBackGroundCircle(CDC * pDC, int radius, Color color)
{
    Graphics gdiplus(pDC->m_hDC);
    // Set smooth draw
    gdiplus.SetSmoothingMode(SmoothingModeAntiAlias);
    Pen pen(color);
    CRect rc;
    rc.top = m_centerPoint.y - radius;
    rc.left = m_centerPoint.x - radius;
    rc.bottom = m_centerPoint.y + radius;
    rc.right = m_centerPoint.x + radius;

    gdiplus.DrawEllipse(&pen, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
}

void CGraphicView::SaveDCToBitmap(CDC * pDC)
{
    if (m_graphicViewBitmap.GetSafeHandle() == NULL)
    {
        m_graphicViewBitmap.CreateCompatibleBitmap(pDC, RADAR_REAL_SIZE, RADAR_REAL_SIZE);
    }
    //copy what's on the parents at this point
    CDC MemDC;
    MemDC.CreateCompatibleDC(pDC);
    CBitmap *pOldBmp = MemDC.SelectObject(&m_graphicViewBitmap);
    BOOL success = MemDC.BitBlt(0, 0, RADAR_REAL_SIZE, RADAR_REAL_SIZE, pDC, 0,
        0, SRCCOPY);

    MemDC.SelectObject(pOldBmp);
    MemDC.DeleteDC();
}

void CGraphicView::OnSize(UINT nType, int cx, int cy)
{

    if (!::IsWindow(m_hWnd))
        return;
    CWnd::OnSize(nType, cx, cx);

    CRect rc;
    GetWindowRect(rc);
    CRect rcCt(0, 0, rc.Width() + 1, rc.Height() + 1);
    CRgn rgn;
    rgn.CreateEllipticRgnIndirect(CRect(0,0,cx,cy));
    SetWindowRgn(static_cast<HRGN>(rgn.GetSafeHandle()), TRUE);
    rgn.DeleteObject();

    UpdateOffset();
}




void CGraphicView::OnPaint()
{
    CPaintDC dc(this);      // device context for painting
    CBitmap  bitmap;        // Offscreen bitmap
    CBitmap* pOldBitmap = NULL;
    CBitmap* pOldBitmapView;
    CRect rect;
    CRect clrect;
    CDC MemDC;

    clock_t start = clock();
    rect = GetVirtualRect();
    GetClientRect(clrect);

    //dc.SetMapMode(MM_HIENGLISH);
    //dc.SetViewportExt(1000, 1000);
    //dc.SetWindowExt(rect.Width(), rect.Height());

    MemDC.CreateCompatibleDC(&dc);
    MemDC.FillSolidRect(clrect, m_backgroundColor);

    if (m_graphicViewBitmap.GetSafeHandle() == NULL)
    {
        m_graphicViewBitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
        pOldBitmap = MemDC.SelectObject(&m_graphicViewBitmap);
        MemDC.FillSolidRect(rect, m_backgroundColor);
    }
    else
    {
        pOldBitmap = MemDC.SelectObject(&m_graphicViewBitmap);
    }


    Draw(&MemDC);
    MemDC.SelectObject(pOldBitmap);
    SaveDCToBitmap(&MemDC);
    pOldBitmap = MemDC.SelectObject(&m_graphicViewBitmap);

    // Draw circle and line
    CDC memDC;
    CBitmap bgBitmap;
    memDC.CreateCompatibleDC(&dc);

    bgBitmap.CreateCompatibleBitmap(&dc, clrect.Width(), clrect.Height());

    pOldBitmapView = memDC.SelectObject(&bgBitmap);
    memDC.FillSolidRect(&clrect, m_backgroundColor);

    memDC.StretchBlt(BORDER_SIZE,
                    BORDER_SIZE,
                    clrect.Width() - 2 * BORDER_SIZE,
                    clrect.Height() - 2 * BORDER_SIZE,
                    &MemDC,
                    0, 0,
                    rect.Width(),
                    rect.Height(),
                    SRCCOPY);

    // Draw line
    DrawBackGroundLine(&memDC, Color(255, 0, 0));
    // Draw circle
    int radius = RADAR_DISPLAY_RADIUS;
    for (int i = 0; i < 5; i++)
    {
        DrawBackGroundCircle(&memDC, radius, Color(255, 0, 0));
        radius -= 100;
    }
    // Draw current line
    if (m_currAzumith != -1)
    {
        DrawBackGroundLine(&memDC, m_currAzumith, Color(0, 255, 0));
    }

    dc.BitBlt(0,
        0,
        clrect.Width(),
        clrect.Height(),
        &memDC,
        0,
        0,
        SRCCOPY);

    // Swap back the original bitmap.
    memDC.SelectObject(pOldBitmapView);
    memDC.DeleteDC();
    // Swap back the original bitmap.
    MemDC.SelectObject(pOldBitmap);
    MemDC.DeleteDC();
}
