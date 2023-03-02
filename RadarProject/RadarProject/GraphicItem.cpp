#include "stdafx.h"
#include "GraphicItem.h"


CGraphicItem::CGraphicItem()
{
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
        TData currData;
        int dataUpdateSize = m_arrDataUpdate.GetSize();
        MemDC.CreateCompatibleDC(pDC);
        CBitmap *pOldBmp = MemDC.SelectObject(&m_Bitmap);

        for (int i = 0; i < dataUpdateSize; i++)
        {
            currData = m_arrDataUpdate.GetAt(i);
            if (currData.color != 0)
            {
                MemDC.SetPixel(currData.position, currData.color);
                MemDC.SetPixel(currData.position.x, currData.position.y - 1, currData.color);
                MemDC.SetPixel(currData.position.x - 1, currData.position.y, currData.color);
            }
            else
            {
                MemDC.SetPixel(currData.position, RGB(255, 255, 255));
            }
        }

        pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), &MemDC, 0, 0, SRCCOPY);

        MemDC.SelectObject(pOldBmp);
        MemDC.DeleteDC();
        SaveDCToBitmap(pDC);
    }

    m_bIsNeedToReDraw = FALSE;
    m_dataUpdateSize = 0;
    m_arrDataUpdate.RemoveAll();
    m_arrDataUpdate.FreeExtra();
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
    BYTE * pBuffer = new BYTE[dwCount * 4];
    DWORD a = source->GetBitmapBits(dwCount * 4, pBuffer);
    destination->CreateBitmap(bitmapProperties.bmWidth, bitmapProperties.bmHeight, bitmapProperties.bmPlanes, bitmapProperties.bmBitsPixel, pBuffer);
}
