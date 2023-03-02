#pragma once
#include "IFICustomWnd.h"
#include "GraphicItem.h"
#include "resource.h"
class CGraphicView :
    public CIFICustomWnd
{
public:
    // Functions
    CGraphicView();
    ~CGraphicView();
    void Draw(CDC* pDC);
    void UpdateChildrenOffset(CRect &rcBound);
    BOOL UpdateData();
    void GridGraphicsView(int row, int column);
    void UpdateAzimuth(CArray<TData>* data);

    // Variable
    int m_packetCounter;
    CGraphicItem** m_graphicItemMatrix;      // contain all graphic item

protected:
    // Variables                                
    CPoint m_centerPoint;
    CPoint m_arrBGLineEndPoint[12];
    float m_currAzumith;
    CBitmap m_graphicViewBitmap;
    CArray<TData> m_arrDataUpdate;

    // Functions
    void DrawBackGroundLine(CDC* pDC, float angle, Color color);
    void DrawBackGroundLine(CDC* pDC, Color color);
    void DrawBackGroundCircle(CDC* pDC, int radius, Color color);
    void SaveDCToBitmap(CDC* pDC);



    // Message map
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPaint();
};

