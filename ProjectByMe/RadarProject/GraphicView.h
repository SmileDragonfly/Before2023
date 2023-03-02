#pragma once
#include "IFICustomWnd.h"
#include "GraphicItem.h"
class CGraphicView :
    public CIFICustomWnd
{
public:
    // Functions
    CGraphicView();
    ~CGraphicView();
    void Draw(CDC* pDC);
    void UpdateChildrenOffset(CRect &rcBound);
    void DrawBackGround(COLORREF backGroundColor, COLORREF circleColor, CDC* pDC);
    void DrawBackGroundCircle(CDC * pDC, COLORREF circleColor, COLORREF backGroundColor, CRect rc, unsigned int borderSize);
    BOOL UpdateData(TData * data, unsigned int size);
    void MoveToScreenAxis(TData * data, unsigned int size);
    void AssignDataToGraphicItem(TData * data, unsigned int size);

    // Variables                                
    CGraphicItem m_graphicItemMatrix[4][4];      // contain all graphic item
    BOOL m_bIsDrawBackGround;
 
    // Message map
    DECLARE_MESSAGE_MAP()    
};

