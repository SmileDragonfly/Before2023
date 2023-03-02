#include "stdafx.h"
#include "GraphicItem.h"


CGraphicItem::CGraphicItem()
{
    m_dataUpdate = NULL;
    m_bIsNeedToReDraw = FALSE;
    m_dataUpdateSize = 0;
}


CGraphicItem::~CGraphicItem()
{
}
void CGraphicItem::Draw(CDC* pDC)
{
    if (m_bIsNeedToReDraw || m_Bitmap.GetSafeHandle() == NULL)
    {        
        CPoint point(m_rect.left, m_rect.top);
        CDC MemDC;
        MemDC.CreateCompatibleDC(pDC);
        CBitmap *pOldBmp = MemDC.SelectObject(&m_Bitmap);
        for (int i = 0; i < m_dataUpdateSize; i++)
        {
            if (m_dataUpdate[i].color == RGB(255, 255, 255))
            {
                MemDC.SetPixel(m_dataUpdate[i].position, m_pArrBackGroundColor[m_dataUpdate[i].position.y * 250 + m_dataUpdate[i].position.x]);
            }
            else
            {
                MemDC.SetPixel(m_dataUpdate[i].position, m_dataUpdate[i].color);
            }
        }
        pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), &MemDC, 0, 0, SRCCOPY);
        MemDC.SelectObject(pOldBmp);
        MemDC.DeleteDC();
        SaveDCToBitmap(pDC);
    }
    else
    {
        CDC MemDC;
        MemDC.CreateCompatibleDC(pDC);
        CBitmap *pOldBmp = MemDC.SelectObject(&m_Bitmap);
        pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), &MemDC, 0, 0, SRCCOPY);
        MemDC.SelectObject(pOldBmp);
        MemDC.DeleteDC();
    }
    m_bIsNeedToReDraw = FALSE;
    m_dataUpdateSize = 0;
}
void CGraphicItem::SetRect(CRect rc)
{
    m_rect = rc;
}
void CGraphicItem::SetPosition(int iRow, int iCol)
{
    m_iRow = iRow;
    m_iCol = iCol;
}

void CGraphicItem::GetBackGroundData(CDC *pDC)
{
    if (m_dataUpdate)
    {
        delete[] m_dataUpdate;
        m_dataUpdate = NULL;
    }
    m_dataUpdate = new TData[250 * 250];
    m_pArrBackGroundColor = new COLORREF[250 * 250];
    // Get background data
    CPoint point(m_rect.left, m_rect.top);
    for (int iRow = 0; iRow < 250; iRow++)
    {
        for (int iCol = 0; iCol < 250; iCol++)
        {
            m_pArrBackGroundColor[iRow * 250 + iCol] = pDC->GetPixel(point);
            point.Offset(1, 0);
        }
        point.x = m_rect.left;
        point.Offset(0, 1);
    }
    SaveDCToBitmap(pDC);
    //CopyBitmap(&m_Bitmap, &m_BitmapBK);
}
CRect CGraphicItem::GetRect()
{
    return m_rect;
}
void CGraphicItem::SaveDCToBitmap(CDC *pDC)
{
    if (m_Bitmap.GetSafeHandle() == NULL)
    {
        m_Bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
    }
    //copy what's on the parents at this point
    CDC MemDC;
    MemDC.CreateCompatibleDC(pDC);      
    CBitmap *pOldBmp = MemDC.SelectObject(&m_Bitmap);        
    MemDC.BitBlt(0, 0, m_rect.Width(), m_rect.Height(), pDC, m_rect.left,
        m_rect.top, SRCCOPY);

    MemDC.SelectObject(pOldBmp);
    MemDC.DeleteDC();  
}

void CGraphicItem::CopyBitmap(CBitmap * source, CBitmap * destination)
{
    BITMAP bitmapProperties;
    source->GetBitmap(&bitmapProperties);
    DWORD dwCount = bitmapProperties.bmWidth * bitmapProperties.bmHeight;
    BYTE* pBuffer = new BYTE[dwCount];
    source->GetBitmapBits(dwCount, pBuffer);
    destination->CreateBitmap(bitmapProperties.bmWidth, bitmapProperties.bmHeight, bitmapProperties.bmPlanes, bitmapProperties.bmBitsPixel, pBuffer);
}
