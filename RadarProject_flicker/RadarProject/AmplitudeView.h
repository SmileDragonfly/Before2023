#pragma once
#include "IFICustomWnd.h"
class CAmplitudeView :
    public CIFICustomWnd
{
public:
    // Functions
    CAmplitudeView();
    ~CAmplitudeView();
    void Draw(CDC* pDC);
    void UpdateData(CArray<TData>* data);
    void UpdateChildrenOffset(CRect & rcBound);

protected:
    // Variables
    CArray<TData> m_arrDataToUpdate;
};

