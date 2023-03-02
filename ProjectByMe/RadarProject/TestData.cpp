#include "stdafx.h"
#include "TestData.h"

CTestData::CTestData()
{
    m_colorCreationCount = 0;
}


CTestData::~CTestData()
{
}

TTestData * CTestData::CreateLineData(float azimuth, int numberOfPoint)
{
    float xOffset;
    float yOffset;
    TTestData* pTestData = new TTestData;
    pTestData->data = new TData[numberOfPoint];
    pTestData->size = numberOfPoint;
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
        tempx = (float)500 / (sqrt(1 + k * k));
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
        tempy = (float)500 / (sqrt(1 + 1/(k * k)));
        yOffset = (float)tempy / numberOfPoint;
        if ((azimuth >= (PI_VALUE / 2)) && (azimuth < (3 * PI_VALUE / 2)))
        {
            yOffset = -yOffset;
        }
        xOffset = yOffset / k;
    }
    for (int i = 0; i < numberOfPoint; i++)
    {
        if (i * xOffset >= 500)
        {
            pTestData->data[i].position.x = 0;
        }
        else
        {
            pTestData->data[i].position.x = (int)(i * xOffset);
        }
        if (i * yOffset >= 500)
        {
            pTestData->data[i].position.y = 0;
        }
        else
        {
            pTestData->data[i].position.y = (int)(i * yOffset);
        }
        pTestData->data[i].color = pColor[i];
    }

    delete[] pColor;
    return pTestData;
}

TTestData * CTestData::CreateLineRandomData(float azimuth, int numberOfPoint)
{
    TTestData* pLineData = CreateLineData(azimuth, numberOfPoint);
    TTestData* pRetData = new TTestData;
    pRetData->size = rand() % numberOfPoint;
    pRetData->data = new TData[pRetData->size];
    for (int i = 0; i < pRetData->size; i++)
    {
        pRetData->data[i] = pLineData->data[rand() % numberOfPoint];
    }
    FreeTestData(pLineData);
    return pRetData;
}


int g_count = 0;

COLORREF * CTestData::CreateColorArray(int numberOfPoint)
{
    COLORREF* pColor = new COLORREF[numberOfPoint];

    // Create a object from 45->60
    if (g_count == 0)
    {
        if ((m_colorCreationCount >= 30) && (m_colorCreationCount < 45))
        {
            for (int i = 0; i < numberOfPoint; i++)
            {
                if ((i >= 100) && (i < 150))
                {
                    pColor[i] = RGB(0, 255, 0);
                }
                else
                {
                    pColor[i] = RGB(255, 0, 0);
                }

            }
        }
        else
        {
            for (int i = 0; i < numberOfPoint; i++)
            {
                pColor[i] = RGB(255, 0, 0);
            }

        }
    }
    else 
    {
        if ((m_colorCreationCount >= 30) && (m_colorCreationCount < 45))
        {
            for (int i = 0; i < numberOfPoint; i++)
            {
                if ((i >= 100) && (i < 150))
                {
                    pColor[i] = RGB(255, 255, 255);
                }
                else
                {
                    pColor[i] = RGB(255, 0, 0);
                }

            }
        }
        else
        {
            for (int i = 0; i < numberOfPoint; i++)
            {
                pColor[i] = RGB(255, 0, 0);
            }

        }
    }
    m_colorCreationCount++;
    return pColor;
}


TTestData * CTestData::CreateLineDataWithColorInput(float azimuth, int numberOfPoint, COLORREF * pColor)
{
    float xOffset;
    float yOffset;
    TTestData* pTestData = new TTestData;
    pTestData->data = new TData[numberOfPoint];
    pTestData->size = numberOfPoint;
    azimuth = PI_VALUE * azimuth / 180;

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
    if ((k <= 1) && (k >= -1))
    {
        float tempx;
        tempx = (float)500 / (sqrt(1 + k * k));
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
        tempy = (float)500 / (sqrt(1 + 1 / (k * k)));
        yOffset = (float)tempy / numberOfPoint;
        if ((azimuth >= (PI_VALUE / 2)) && (azimuth < (3 * PI_VALUE / 2)))
        {
            yOffset = -yOffset;
        }
        xOffset = yOffset / k;
    }
    for (int i = 0; i < numberOfPoint; i++)
    {
        if (i * xOffset >= 500)
        {
            pTestData->data[i].position.x = 0;
        }
        else
        {
            pTestData->data[i].position.x = (int)(i * xOffset);
        }
        if (i * yOffset >= 500)
        {
            pTestData->data[i].position.y = 0;
        }
        else
        {
            pTestData->data[i].position.y = (int)(i * yOffset);
        }
        pTestData->data[i].color = pColor[i];
    }
    return pTestData;
}

TTestData * CTestData::CreateLineRandomDataWithColorInput(float azimuth, int numberOfPoint, COLORREF * pColor)
{
    TTestData* pLineData = CreateLineDataWithColorInput(azimuth, numberOfPoint, pColor);
    TTestData* pRetData = new TTestData;
    pRetData->size = rand() % numberOfPoint;
    pRetData->data = new TData[pRetData->size];
    for (int i = 0; i < pRetData->size; i++)
    {
        pRetData->data[i] = pLineData->data[rand() % numberOfPoint];
    }
    FreeTestData(pLineData);
    return pRetData;
}

void CTestData::FreeTestData(TTestData * testData)
{
    delete[] testData->data;
    delete testData;
}

