#include "stdafx.h"
#include "RadarInput.h"
#include "RadarProjectDlg.h"
#include "GraphicItem.h"
#include "RadarProject.h"


CRadarInput::CRadarInput()
{
}


CRadarInput::~CRadarInput()
{
}

BOOL CRadarInput::ReadPacketFromFile(FILE* pFile)
{
    TPacketRaw rawPacket;
    if (ReadRawPacketFromFile(pFile, rawPacket))
    {
        TransformRawPacket(rawPacket);
        return TRUE;
    }
    return FALSE;
}

BOOL CRadarInput::ReadRawPacketFromFile(FILE* pFile, TPacketRaw& packetRaw)
{
    // TODO: Check is start byte

    // Read header
    if (ReadPacketHeader(pFile, packetRaw.packetHeader))
    {
        // Read data
        packetRaw.pData = new BYTE[packetRaw.packetHeader.packetDataSize];
        fread(packetRaw.pData, sizeof(BYTE), packetRaw.packetHeader.packetDataSize, pFile);
        return TRUE;
    }
    return FALSE;

}

BOOL CRadarInput::TransformRawPacket(TPacketRaw& rawPacket)
{
    int packetDataSize = rawPacket.packetHeader.packetDataSize;
    float azimuthAngle = (float)rawPacket.packetHeader.azimuth / 10;

    // Create Points
    CreateDataForGraphicItems(azimuthAngle, packetDataSize, rawPacket.pData);

    // Free rawPacket.pData
    delete[] rawPacket.pData;

    return TRUE;
}

BOOL CRadarInput::ReadPacketHeader(FILE* pFile, TPacketHeader& packetHeader)
{
    BYTE* pHeader = new BYTE[100];
    int size = fread(pHeader, sizeof(BYTE), 100, pFile);
    if (size != 100)
    {
        return FALSE;
    }
    packetHeader.packetDataSize = pHeader[2] | (pHeader[3] << 8);
    packetHeader.azimuth = pHeader[4] | (pHeader[5] << 8);

    // Data size change....
    if (packetHeader.packetDataSize != m_radarRealRadius)
    {
        m_radarRealRadius = packetHeader.packetDataSize;
        ((CRadarProjectDlg*)((CRadarProjectApp*)AfxGetApp()->m_pMainWnd))->m_graphicView.UpdateRadarRealRadius(m_radarRealRadius);
    }

    return TRUE;
}

void CRadarInput::CreateDataForGraphicItems(float azimuth, int numberOfPoint, BYTE* pColor)
{
    float xOffset;
    float yOffset;
    int itemRow;
    int itemColumn;
    unsigned int rowSize = 2 * m_radarRealRadius / GRAPHIC_VIEW_ROW;
    unsigned int columnSize = 2 * m_radarRealRadius / GRAPHIC_VIEW_COLUMN;
    ((CRadarProjectDlg*)AfxGetApp()->m_pMainWnd)->m_graphicView.UpdateAzimuth(azimuth);

    azimuth = M_PI * azimuth / 180;
    azimuth = M_PI_2 - azimuth;
    float cosA = cos(azimuth);
    float sinA = sin(azimuth);

    float radiusOff = (float) m_radarRealRadius / numberOfPoint;
    float radius = radiusOff;
    TData currData;
    CGraphicItem** pGraphicItemMatrix = ((CRadarProjectDlg*)AfxGetApp()->m_pMainWnd)->m_graphicView.m_graphicItemMatrix;

    for (int i = 0; i < numberOfPoint; i++)
    {
        currData.position.x = round(radius * cosA);
        currData.position.y = round(radius * sinA);
        currData.color = pColor[i];

        // Move Screen Axis
        currData.position.x += m_radarRealRadius;
        currData.position.y = m_radarRealRadius - currData.position.y;

        // Check last point
        if (i == numberOfPoint - 1)
        {
            if (currData.position.x = (2*m_radarRealRadius))
            {
                currData.position.x -= 1;
            }
            if (currData.position.y = (2 * m_radarRealRadius))
            {
                currData.position.y -= 1;
            }
        }


        // Assign data to graphic item
        itemRow = (int)(currData.position.y / rowSize);
        itemColumn = (int)(currData.position.x / columnSize);

        currData.position.x = currData.position.x - pGraphicItemMatrix[itemRow][itemColumn].GetRect().left;
        currData.position.y = currData.position.y - pGraphicItemMatrix[itemRow][itemColumn].GetRect().top;

        pGraphicItemMatrix[itemRow][itemColumn].m_arrDataUpdate.Add(currData);
        pGraphicItemMatrix[itemRow][itemColumn].m_bIsNeedToReDraw = TRUE;

        radius += radiusOff;
    }
}
