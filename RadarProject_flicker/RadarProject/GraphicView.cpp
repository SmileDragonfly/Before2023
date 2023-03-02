#include "stdafx.h"
#include "GraphicView.h"
#include "gdiplus.h"

CGraphicView::CGraphicView()
    :CIFICustomWnd(AfxGetInstanceHandle(),_T("CGraphicViewClassName"))
{
    m_backgroundColor = RGB(255, 255, 255);
    m_centerPoint.x = RADAR_REAL_RADIUS;
    m_centerPoint.y = RADAR_REAL_RADIUS;
    int radius = 5120;
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
    m_windowSize = RADAR_REAL_SIZE;
    m_prevWindowSize = RADAR_REAL_SIZE;
    m_windowRadius = RADAR_REAL_RADIUS;
    m_viewportSize = RADAR_DISPLAY_SIZE;
    m_viewportRadius = RADAR_DISPLAY_RADIUS;
    m_viewportOrg.SetPoint(0, 0);
}

CGraphicView::~CGraphicView()
{
}

BEGIN_MESSAGE_MAP(CGraphicView, CIFICustomWnd)
    ON_WM_ERASEBKGND()
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_PAINT()
    ON_WM_TIMER()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void CGraphicView::Draw(CDC* pDC)
{
    clock_t start, s;
    float time, totalTime;
    s = clock();
    for (int iRow = 0; iRow < GRAPHIC_VIEW_ROW; iRow++)
    {
        for (int iCol = 0; iCol < GRAPHIC_VIEW_COLUMN; iCol++)
        {
            m_graphicItemMatrix[iRow][iCol].Draw(pDC);
        }
    }
    totalTime = float(clock() - s) / CLOCKS_PER_SEC;
}

void CGraphicView::UpdateChildrenOffset(CRect & rcBound)
{
    GridGraphicsView(GRAPHIC_VIEW_ROW, GRAPHIC_VIEW_COLUMN);
    rcBound = CRect(0, 0, m_windowSize, m_windowSize);
}

BOOL CGraphicView::UpdateData(CArray<TData>* data)
{
    if (data)
    {
        MoveToScreenAxis(data);
        AssignDataToGraphicItem(data);
        //Invalidate();        
        return TRUE;
    }    
    return FALSE;
}

void CGraphicView::MoveToScreenAxis(CArray<TData>* data)
{
    TData currData;
    int dataSize = data->GetSize();
    for (int i = 0; i < dataSize; i++)
    {
        currData = data->GetAt(i);
        currData.position.x += m_windowSize / 2;
        currData.position.y = m_windowSize / 2 - currData.position.y;
        data->SetAt(i, currData);
    }
    // Check last point
    currData = data->GetAt(dataSize - 1);
    if (currData.position.x = m_windowSize)
    {
        currData.position.x -= 1;
        data->SetAt(dataSize - 1, currData);
    }
    if(currData.position.y = m_windowSize)
    {
        currData.position.y -= 1;
        data->SetAt(dataSize - 1, currData);
    }
}

void CGraphicView::AssignDataToGraphicItem(CArray<TData>* data)
{
    unsigned int itemRow, itemColumn;
    unsigned int rowSize = m_windowSize / GRAPHIC_VIEW_ROW;
    unsigned int columnSize = m_windowSize / GRAPHIC_VIEW_COLUMN;
    TData currData;
    int dataSize = data->GetSize();
    for (int i = 0; i < dataSize; i++)
    {
        currData = data->GetAt(i);
        itemRow = (int)(currData.position.y / rowSize);
        itemColumn = (int)(currData.position.x / columnSize);
        // Update data to graphic item
        currData.position.x = currData.position.x - m_graphicItemMatrix[itemRow][itemColumn].GetRect().left;
        currData.position.y = currData.position.y - m_graphicItemMatrix[itemRow][itemColumn].GetRect().top;

        m_graphicItemMatrix[itemRow][itemColumn].m_arrDataUpdate.Add(currData);
        m_graphicItemMatrix[itemRow][itemColumn].m_bIsNeedToReDraw = TRUE;
    }
}

void CGraphicView::GridGraphicsView(int row, int column)
{
    int rowSize = m_windowSize / row;
    int colSize = m_windowSize / column;
    m_graphicItemMatrix = new CGraphicItem*[row];
    for (int i = 0; i < row; i++)
    {
        m_graphicItemMatrix[i] = new CGraphicItem[column];
    }

    CRect rcBaseItem(0, 0, m_windowSize / GRAPHIC_VIEW_ROW, m_windowSize / GRAPHIC_VIEW_COLUMN);
    CRect rcItem;
    for (int iRow = 0; iRow < GRAPHIC_VIEW_ROW; iRow++)
    {
        for (int iCol = 0; iCol < GRAPHIC_VIEW_COLUMN; iCol++)
        {
            rcItem = rcBaseItem;
            rcItem.OffsetRect(iCol * m_windowSize / GRAPHIC_VIEW_ROW, iRow * m_windowSize / GRAPHIC_VIEW_COLUMN);
            m_graphicItemMatrix[iRow][iCol].SetRect(rcItem);
            m_graphicItemMatrix[iRow][iCol].SetPosition(iRow, iCol);
        }
    }
}

void CGraphicView::UpdateAzimuth(float azimuth)
{
    m_currAzumith = azimuth;
}

void CGraphicView::UpdateRadarRealRadius(int realRadius)
{
    m_prevWindowSize = m_windowSize;
    m_windowRadius = realRadius;
    m_windowSize = 2 * realRadius;
    m_centerPoint.x = m_windowRadius;
    m_centerPoint.y = m_windowRadius;
    GridGraphicsView(GRAPHIC_VIEW_ROW, GRAPHIC_VIEW_COLUMN);
    // Recalculate 
    for (int i = 0; i < 12; i++)
    {
        float angle = 30 * i * M_PI / 180;
        // m_centerPoint is origin
        m_arrBGLineEndPoint[i].x = (int)(5120 * sin(angle));
        m_arrBGLineEndPoint[i].y = (int)(5120 * cos(angle));
        // client origin
        m_arrBGLineEndPoint[i].x = m_centerPoint.x + m_arrBGLineEndPoint[i].x;
        m_arrBGLineEndPoint[i].y = -m_arrBGLineEndPoint[i].y + m_centerPoint.y;
    }
}

void CGraphicView::UpdateZoomInView()
{
    m_viewportSize = m_viewportSize + 128;
    m_viewportRadius = m_viewportRadius + 64;
    CRect rc;
    GetClientRect(&rc);
    m_viewportOrg.x = (rc.Width() - m_viewportSize) / 2;
    m_viewportOrg.y = (rc.Height() - m_viewportSize) /2;
}

void CGraphicView::UpdateZoomOutView()
{
    m_viewportSize = m_viewportSize - 128;
    m_viewportRadius = m_viewportRadius - 64;
    CRect rc;
    GetClientRect(&rc);
    m_viewportOrg.x = (rc.Width() - m_viewportSize) / 2;
    m_viewportOrg.y = (rc.Height() - m_viewportSize) / 2;
}

void CGraphicView::DrawBackGroundLine(Graphics& gdiplus, float angle, Color color)
{
    Pen pen(color);
    CPoint endPoint;
    float radius = m_windowRadius;
    angle = angle * M_PI / 180;
    // m_centerPoint is origin
    endPoint.x = (int)(radius * sin(angle));
    endPoint.y = (int)(radius * cos(angle));
    // client origin
    endPoint.x = m_centerPoint.x + endPoint.x;
    endPoint.y = - endPoint.y + m_centerPoint.y;

    gdiplus.DrawLine(&pen, m_centerPoint.x, m_centerPoint.y, endPoint.x, endPoint.y);
}

void CGraphicView::DrawBackGroundLine(Graphics& gdiplus, Color color)
{
    Pen pen(color);
    for (int i = 0; i < 12; i++)
    {
        gdiplus.DrawLine(&pen, m_centerPoint.x, m_centerPoint.y, m_arrBGLineEndPoint[i].x, m_arrBGLineEndPoint[i].y);
    }
}

void CGraphicView::DrawBackGroundCircle(Graphics& gdiplus, int radius, Color color)
{
    Pen pen(color);
    CRect rc;
    rc.top = m_centerPoint.y - radius;
    rc.left = m_centerPoint.x - radius;
    rc.bottom = m_centerPoint.y + radius;
    rc.right = m_centerPoint.x + radius;

    gdiplus.DrawEllipse(&pen, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
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


CRect CGraphicView::GetVirtualRect()
{
    CRect rc(0, 0, m_windowSize, m_windowSize);
    return rc;
}



void CGraphicView::OnPaint()
{
    CPaintDC dc(this);      // device context for painting
    CBitmap  bitmap;        // Offscreen bitmap
    CBitmap* pOldBitmap = NULL;
    CBitmap* pOldBitmapView;
    CRect rect;
    CRect clrect;
    CDC memDC;

    rect = GetVirtualRect();
    GetClientRect(clrect);

    int iSaveDC = dc.SaveDC();
    dc.SetMapMode(MM_ISOTROPIC);
    dc.SetWindowExt(rect.Width(), rect.Height());
    dc.SetViewportExt(m_viewportSize, m_viewportSize);
    dc.SetViewportOrg(m_viewportOrg);
    
    memDC.CreateCompatibleDC(&dc);
    if (m_windowBitmap.GetSafeHandle() == NULL | (m_prevWindowSize != m_windowSize))
    {
        m_windowBitmap.DeleteObject();
        m_windowBitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
        pOldBitmap = memDC.SelectObject(&m_windowBitmap);
        memDC.FillSolidRect(&rect, RGB(255, 255, 255));
        m_prevWindowSize = m_windowSize;
    }
    else
    {
        pOldBitmap = memDC.SelectObject(&m_windowBitmap);
    }

    Draw(&memDC);

    // Draw circle and line
    Graphics gdiplus(memDC.m_hDC);
    // Set smooth draw
    gdiplus.SetSmoothingMode(SmoothingModeAntiAlias);
    // Draw line
    DrawBackGroundLine(gdiplus, Color(0, 0, 255));
    // Draw circle
    int radius = m_windowRadius - m_windowRadius / 5;
    for (int i = 0; i < 5; i++)
    {
        DrawBackGroundCircle(gdiplus, radius, Color(0, 0, 255));
        radius -= m_windowRadius / 5;
    }
    // Draw current data
    if (m_currAzumith != -1)
    {
        DrawBackGroundLine(gdiplus, m_currAzumith, Color(0, 255, 0));
    }

    
    dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

    //
    memDC.SelectObject(pOldBitmap);
    memDC.DeleteDC();
}


void CGraphicView::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (nFlags == MK_LBUTTON)
    {
        m_viewportOrg.x += (point.x - m_leftMouseClickPoint.x) ;
        m_viewportOrg.y += (point.y - m_leftMouseClickPoint.y);
    }

    CIFICustomWnd::OnMouseMove(nFlags, point);
}


void CGraphicView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    m_leftMouseClickPoint = point;

    CIFICustomWnd::OnLButtonDown(nFlags, point);
}
