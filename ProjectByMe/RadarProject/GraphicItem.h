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
    CBitmap m_BitmapBK;
    COLORREF* m_pArrBackGroundColor;     // Use to reload background if a point have no infor
protected:
    void SaveDCToBitmap(CDC* pDC);  
    void CopyBitmap(CBitmap* source, CBitmap* destination);
public:
    // Functions
    void Draw(CDC* pDC);
    void SetRect(CRect rc);
    void SetPosition(int iRow, int iCol);
    void GetBackGroundData(CDC *pDC);
    CRect GetRect();
    
    // Variables
    BOOL m_bIsNeedToReDraw;
    TData* m_dataUpdate;
    unsigned int m_dataUpdateSize;
};

