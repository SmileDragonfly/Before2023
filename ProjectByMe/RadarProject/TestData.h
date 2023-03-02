#pragma once
#include "CommonStructures.h"

struct TTestData
{
    TData* data;
    int size;
};

extern int g_count;

class CTestData
{
public:
    CTestData();
    ~CTestData();
    // Functions
    TTestData* CreateLineData(float azimuth, int numberOfPoint);
    TTestData* CreateLineRandomData(float azimuth, int numberOfPoint);
    COLORREF*  CreateColorArray(int numberOfPoint);
    TTestData* CreateLineDataWithColorInput(float azimuth, int numberOfPoint, COLORREF* pColor);
    TTestData* CreateLineRandomDataWithColorInput(float azimuth, int numberOfPoint, COLORREF* pColor);
    void FreeTestData(TTestData* data);
    // Variables
    int m_colorCreationCount;
};
