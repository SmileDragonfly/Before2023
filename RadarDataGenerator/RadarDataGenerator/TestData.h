#pragma once
#include "CommonStructures.h"

extern int g_count;

class CTestData
{
public:
    CTestData();
    ~CTestData();
    // Functions
    void CreateLineData(float azimuth, int numberOfPoint, CArray<TData>* arrData);
    COLORREF* CreateColorArray(int numberOfPoint);
    // Variables
    int m_colorCreationCount;
};
