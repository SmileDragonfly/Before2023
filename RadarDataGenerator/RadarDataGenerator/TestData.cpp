#include "stdafx.h"
#include "TestData.h"
#include "CommonDefinitions.h"

CTestData::CTestData()
{
    m_colorCreationCount = 0;
}


CTestData::~CTestData()
{
}

void CTestData::CreateLineData(float azimuth, int numberOfPoint, CArray<TData>* arrData)
{
    float xOffset;
    float yOffset;
    azimuth =  PI_VALUE * azimuth / 180;

    // Create color array
    COLORREF* pColor = CreateColorArray(numberOfPoint);

    float k;
    if (cos(azimuth) == 0)
    {
        k = MAX_K;
    }
    else
    {
        if ((1 / tan(azimuth)) > MAX_K)
        {
            k = MAX_K;
        }
        else
        {
            k = 1 / tan(azimuth);
        }
    }

    //TRACE("k = %f\n", k);
    if ((k <= 1)&&(k >= -1))
    {
        float tempx;
        tempx = (float)250 / (sqrt(1 + k * k));
        xOffset = (float)tempx / numberOfPoint;
        if (azimuth > PI_VALUE)
        {
            xOffset = -xOffset;
        }
        yOffset = k * xOffset;
    }
    else
    {
        float tempy;
        tempy = (float)250 / (sqrt(1 + 1/(k * k)));
        yOffset = (float)tempy / numberOfPoint;
        if ((azimuth >= (PI_VALUE / 2)) && (azimuth < (3 * PI_VALUE / 2)))
        {
            yOffset = -yOffset;
        }
        xOffset = yOffset / k;
    }
    for (int i = 0; i < numberOfPoint; i++)
    {
        TData currData;
        if (i * xOffset >= 250)
        {
            currData.position.x = 0;
        }
        else
        {
            currData.position.x = (int)(i * xOffset);
        }
        if (i * yOffset >= 250)
        {
            currData.position.y = 0;
        }
        else
        {
            currData.position.y = (int)(i * yOffset);
        }
        currData.color = pColor[i];
        arrData->Add(currData);
    }

    delete[] pColor;
}



int g_count = 0;

COLORREF * CTestData::CreateColorArray(int numberOfPoint)
{
    COLORREF* pColor = new COLORREF[numberOfPoint];
    for (int i = 0; i < numberOfPoint; i++)
    {
        pColor[i] = rand();
    }
    return pColor;
}


