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
    COLORREF*  CreateColorArrayCheckBackground(int numberOfPoint);
    TTestData* CreateLineDataWithColorInput(float azimuth, int numberOfPoint, COLORREF* pColor);
    TTestData* CreateLineRandomDataWithColorInput(float azimuth, int numberOfPoint, COLORREF* pColor);
    COLORREF*  CreateColorArrayForAmplitudeView(int numberOfPoint);
    BOOL       ReadPacketFromFile(std::string fileName, CArray<TData>* arrData);
    void       ConvertStringToData(std::string strInput, CArray<TData>* arrData, float & azimuth);

    void FreeTestData(TTestData* data);
    void CreateLineData(float azimuth, int numberOfPoint, CArray<TData>* arrData);
    // Variables
    int m_colorCreationCount;
    std::ifstream m_inputFile;
    float m_currAzumith;
    float m_oldAzumith;
    TData m_lastDataRead;
};
