#pragma once
class CRadarInput
{
public:
    // Functions
    CRadarInput();
    ~CRadarInput();
    BOOL ReadPacketFromFile(FILE* pFile);
    BOOL ReadRawPacketFromFile(FILE* pFile, TPacketRaw& packetRaw);
    BOOL TransformRawPacket(TPacketRaw& rawPacket);
    BOOL ReadPacketHeader(FILE* pFile, TPacketHeader& packetHeader);
    void CreateDataForGraphicItems(float azimuth, int numberOfPoint, BYTE* pColor);

    // Variables
    int m_radarRealRadius;
};

