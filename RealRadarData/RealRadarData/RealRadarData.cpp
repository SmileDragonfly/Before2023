// RealRadarData.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "stdio.h"
#include "Header.h"
using namespace std;

int main()
{
    TRadarPacket radarPacket;
    int azimuth = 0;
    int packetSize = PACKET_SIZE;
    FILE* pFile = fopen("data_test.txt", "wb");
    for(int i = 0; i < NUMBER_OF_PACKETS; i++ )
    {
        radarPacket.header[2] = packetSize & 0xFF;
        radarPacket.header[3] = (packetSize >> 8) & 0xFF;
        radarPacket.header[4] = azimuth & 0xFF;
        radarPacket.header[5] = (azimuth >> 8) & 0xFF;
        for(int j = 0; j < packetSize; j++)
        {
            if ((j < (2 * packetSize / 3))&&(j> (packetSize / 3)))
            {
                radarPacket.data[j] = rand() % 255;
            }
            else
            {
                radarPacket.data[j] = 0;
            }

        }
        azimuth++;
        if (azimuth == 3600)
        {
            azimuth = 0;
        }
        fwrite(&radarPacket, sizeof(TRadarPacket), 1, pFile);
        cout << "packet:" << i << endl;
    }
    fclose(pFile);
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
