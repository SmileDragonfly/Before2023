#include "stdafx.h"
#include "TestData.h"
#define MAX_K                       500

CTestData::CTestData()
{
    m_colorCreationCount = 0;
    m_currAzumith = 0;
    m_oldAzumith = 0;
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
    azimuth =  M_PI * azimuth / 180;

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
        if (azimuth > M_PI)
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
        if ((azimuth >= (M_PI / 2)) && (azimuth < (3 * M_PI / 2)))
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
    for (int i = 0; i < numberOfPoint; i++)
    {
        pColor[i] = rand();
    }
    return pColor;
}

COLORREF * CTestData::CreateColorArrayCheckBackground(int numberOfPoint)
{
    COLORREF* pColor = new COLORREF[numberOfPoint];

    // Create a object from 45->60
    if (g_count == 0)
    {
        if ((m_colorCreationCount >= 45) && (m_colorCreationCount < 60))
        {
            for (int i = 0; i < numberOfPoint; i++)
            {
                if ((i >= 50) && (i < 150))
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
        if ((m_colorCreationCount >= 45) && (m_colorCreationCount < 60))
        {
            for (int i = 0; i < numberOfPoint; i++)
            {
                if ((i >= 50) && (i < 150))
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
    azimuth = M_PI * azimuth / 180;

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
        tempx = (float)5000 / (sqrt(1 + k * k));
        xOffset = (float)tempx / numberOfPoint;
        if (azimuth > M_PI)
        {
            xOffset = -xOffset;
        }
        yOffset = k * xOffset;
    }
    else
    {
        float tempy;
        tempy = (float)5000 / (sqrt(1 + 1 / (k * k)));
        yOffset = (float)tempy / numberOfPoint;
        if ((azimuth >= (M_PI / 2)) && (azimuth < (3 * M_PI / 2)))
        {
            yOffset = -yOffset;
        }
        xOffset = yOffset / k;
    }
    for (int i = 0; i < numberOfPoint; i++)
    {
        if (i * xOffset >= 5000)
        {
            pTestData->data[i].position.x = 0;
        }
        else
        {
            pTestData->data[i].position.x = (int)(i * xOffset);
        }
        if (i * yOffset >= 5000)
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

COLORREF * CTestData::CreateColorArrayForAmplitudeView(int numberOfPoint)
{
    COLORREF* pColor = new COLORREF[numberOfPoint];    
    for (int i = 0; i < numberOfPoint; i++)
    {
        pColor[i] = rand() % 255;
    }
    return pColor;
}

BOOL CTestData::ReadPacketFromFile(std::string fileName, CArray<TData>* arrData)
{
    float azimuth;
    if (!m_inputFile.is_open())
    {
        m_inputFile.open(fileName);
    }
    if ((m_lastDataRead.position.x != 0) || (m_lastDataRead.position.y != 0) || (m_lastDataRead.color != 0))
    {
        arrData->Add(m_lastDataRead);
    }
    std::string strFileLine;
    std::getline(m_inputFile, strFileLine);
    ConvertStringToData(strFileLine, arrData, m_currAzumith);
    m_oldAzumith = m_currAzumith;
    BOOL check = TRUE;
    while (m_currAzumith == m_oldAzumith)
    {
        if (std::getline(m_inputFile, strFileLine))
        {
            ConvertStringToData(strFileLine, arrData, m_currAzumith);
        }
        else
        {
            m_inputFile.close();
            return FALSE;
        }
    }
    m_lastDataRead = arrData->GetAt(arrData->GetSize() - 1);
    arrData->RemoveAt(arrData->GetSize() - 1);
    return TRUE;
}

void CTestData::ConvertStringToData(std::string strInput, CArray<TData>* arrData, float & azimuth)
{
    TData data;
    int start = 0;
    int end = strInput.find(",", 0);
    std::string subStr = strInput.substr(0, end);
    // get azimuth
    azimuth = atof(subStr.c_str());
    // get position x
    start = end + 1;
    end = strInput.find(",", start);
    subStr = strInput.substr(start, end - start);
    data.position.x = atoi(subStr.c_str());
    // get position y
    start = end + 1;
    end = strInput.find(",", start);
    subStr = strInput.substr(start, end - start);
    data.position.y = atoi(subStr.c_str());
    // get color
    start = end + 1;
    end = strInput.find(",", start);
    subStr = strInput.substr(start, end - start);
    data.color = atoi(subStr.c_str());
    // add to arr
    arrData->Add(data);
}

void CTestData::FreeTestData(TTestData * testData)
{
    delete[] testData->data;
    delete testData;
}

void CTestData::CreateLineData(float azimuth, int numberOfPoint, CArray<TData>* arrData)
{
    float xOffset;
    float yOffset;
    TData currData;
    azimuth = M_PI * azimuth / 180;

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
    if ((k <= 1) && (k >= -1))
    {
        float tempx;
        tempx = (float)RADAR_REAL_RADIUS / (sqrt(1 + k * k));
        xOffset = (float)tempx / numberOfPoint;
        if (azimuth > M_PI)
        {
            xOffset = -xOffset;
        }
        yOffset = k * xOffset;
    }
    else
    {
        float tempy;
        tempy = (float)RADAR_REAL_RADIUS / (sqrt(1 + 1 / (k * k)));
        yOffset = (float)tempy / numberOfPoint;
        if ((azimuth >= (M_PI / 2)) && (azimuth < (3 * M_PI / 2)))
        {
            yOffset = -yOffset;
        }
        xOffset = yOffset / k;
    }
    for (int i = 0; i < numberOfPoint; i++)
    {
        if (i * xOffset >= RADAR_REAL_RADIUS)
        {
            currData.position.x = 0;
        }
        else
        {
            currData.position.x = (int)(i * xOffset);
        }
        if (i * yOffset >= RADAR_REAL_RADIUS)
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
