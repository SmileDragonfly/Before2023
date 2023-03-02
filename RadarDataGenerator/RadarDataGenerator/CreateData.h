#pragma once
#include "TestData.h"

class CCreateData
{
public:
    // Functions
    CCreateData();
    ~CCreateData();
    void GenerateData(CArray<CPoint>* arr, int numberOfPoint, CArray<TData>* arrData);
    void SaveDataToCSV(FILE* pFile, CArray<TData>* arrData);

    // Variables
    CTestData m_testData;
};

