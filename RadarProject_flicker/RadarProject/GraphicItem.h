#pragma once
class CGraphicItem
{
public:
    CGraphicItem();
    ~CGraphicItem();
private:
    // Variable
    CPoint m_point;
    CRect m_rect;
    int m_iRow;
    int m_iCol;
    CBitmap m_Bitmap;
    COLORREF* m_pArrBackGroundColor;     // Use to reload background if a point have no infor
protected:
    void SaveDCToBitmap(CDC* pDC);  
    void CopyBitmap(CBitmap* source, CBitmap* destination);
public:
    // Functions
    void Draw(CDC* pDC);
    void SetRect(CRect rc);
    void SetPosition(int iRow, int iCol);
    CRect GetRect();
    
    // Variables
    BOOL m_bIsNeedToReDraw;
    CArray<TData> m_arrDataUpdate;
};

