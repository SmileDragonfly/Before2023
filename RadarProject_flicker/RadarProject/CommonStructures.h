#pragma once

struct TData {
    CPoint      position;
    BYTE    color;
    TData()
    {
        position.x = 0;
        position.y = 0;
        color = 0;
    }
    TData(int a, int b, int c)
    {
        position.x = a;
        position.y = b;
        color = c;
    }
};

struct TPacketHeader {
    int packetDataSize;
    int azimuth;
};

struct TPacketRaw
{
    TPacketHeader packetHeader;
    BYTE * pData;
};

struct TPacketInfor {
    TPacketHeader packetHeader;
    TData * pData;
};