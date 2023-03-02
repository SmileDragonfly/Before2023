#include "stdafx.h"
#include "CreateData.h"


CCreateData::CCreateData()
{
}


CCreateData::~CCreateData()
{
}

void CCreateData::GenerateData(CArray<CPoint>* arr, int numberOfPoint, CArray<TData>* arrData)
{
    for (int i = 0; i < arr->GetSize(); i++)
    {
        CArray<CArray<TData>*> shapeData;
        CPoint currPoint = arr->GetAt(i);
        int radius;

        if (abs(currPoint.x) < abs(currPoint.y))
        {
            radius = rand() % abs(currPoint.x);
        }
        else
        {
            radius = rand() % abs(currPoint.y);
        }

        double arc = atan2(currPoint.y, currPoint.x) * 180 / M_PI;

        if (arc < 0)
        {
            arc += 360;
        }
        TRACE(L"CenterPoint = (%d,%d)\n", currPoint.x, currPoint.y);
        TRACE(L"Arc = %f\n", arc);
        TRACE(L"Radius = %d\n", radius);

        double arcBound = asin(radius / sqrt(pow(currPoint.y, 2) + pow(currPoint.x, 2))) * 180 / M_PI;
        double arcOff = 1;
        TRACE(L"ArcBound = %f\n", arcBound);

        double currArc = arc - arcBound;

        TRACE(L"currArc = %f\n", currArc);

        int count = 2*(arcBound / arcOff);
        CArray<TData>* shapeLineData = new CArray<TData>[count];

        for (int i = 0; i < count; i++)
        {
            if ((90 - currArc) > 0)
            {
                m_testData.CreateLineData(90 - currArc, numberOfPoint, &shapeLineData[i]);
            }
            else
            {
                m_testData.CreateLineData(90 - currArc + 360, numberOfPoint, &shapeLineData[i]);
            }
            CString strWriteToFile;
            // Remove all point outside the circle
            for (int j = 0; j < shapeLineData[i].GetSize(); j++)
            {
                TData currData = shapeLineData[i].GetAt(j);
                float a = pow(currData.position.x - currPoint.x, 2);
                float b = pow(currData.position.y - currPoint.y, 2);
                float c = pow(radius, 2);
                if ((a + b) > c)
                {
                    TRACE(L"Remove point = (%d,%d)\n", currData.position.x, currData.position.y);
                    shapeLineData[i].RemoveAt(j);
                    TRACE(L"Line Size = %d\n", shapeLineData[i].GetSize());
                    j--;
                }
                else
                {
                    arrData->Add(currData);
                }
            }
            currArc = currArc + arcOff;
            TRACE(L"currArc = %f\n", currArc);
        }
    }
}

void CCreateData::SaveDataToCSV(FILE * pFile, CArray<TData>* arrData)
{
    for (int i = 0; i < arrData->GetSize(); i++)
    {
        TData currData = arrData->GetAt(i);
        CString strWrite;
        strWrite.Format(L"%d,%d,%d\r\n", currData.position.x, currData.position.y, currData.color);
        fwrite(strWrite, sizeof(wchar_t), strWrite.GetLength(), pFile);
    }
}
