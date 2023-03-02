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
    BOOL UpdateData(CArray<TData>* data);
    void MoveToScreenAxis(CArray<TData>* data);
    void AssignDataToGraphicItem(CArray<TData>* data);
    void GridGraphicsView(int row, int column);
    void UpdateAzimuth(float azimuth);
    void UpdateRadarRealRadius(int realSize);
    void UpdateZoomInView();
    void UpdateZoomOutView();
    // Variables
    CGraphicItem** m_graphicItemMatrix;      // contain all graphic item

protected:
    // Variables                                
    CPoint m_centerPoint;
    CPoint m_arrBGLineEndPoint[12];
    float m_currAzumith;
    int m_prevWindowSize;
    int m_windowSize;
    int m_windowRadius;
    int m_viewportSize;
    int m_viewportRadius;
    CPoint m_viewportOrg;
    CBitmap m_windowBitmap;
    // Move view
    CPoint m_leftMouseClickPoint;

    // Functions
    void DrawBackGroundLine(Graphics& gdiplus, float angle, Color color);
    void DrawBackGroundLine(Graphics& gdiplus, Color color);
    void DrawBackGroundCircle(Graphics& gdiplus, int radius, Color color);
    CRect GetVirtualRect();



    // Message map
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

