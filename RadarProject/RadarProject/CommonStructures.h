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
    int packetDataSize;             // bytes 2,3
    int azimuth;                    // bytes 4,5
    int angularVelocity;            // bytes 20
};

struct TPacketRaw
{
    TPacketHeader packetHeader;
    BYTE * pData;
};

struct TPacketInfor {
    TPacketHeader packetHeader;
    CArray<TData> pData;
};