#pragma once

enum COLOR_SPACE
{
    BLUE_MAX    = 0x00FF0000,
    GREEN_MAX   = 0x0000FF00,
    RED_MAX     = 0x000000FF,
    BLUE_MID    = 0x007F0000,
    GREEN_MID   = 0x00007F00,
    RED_MID     = 0x0000007F,
    BLUE_MIN    = 0x00200000,
    GREEN_MIN   = 0x00002000,
    RED_MIN     = 0x00000020
};

#define GRAPHIC_VIEW_WIDTH          10000
#define GRAPHIC_VIEW_HEIGHT         10000
#define GRAPHIC_VIEW_ROW            4
#define GRAPHIC_VIEW_COLUMN         4
#define MAX_K                       500
#define PI_VALUE                    3.14159265358979323846
#define RADAR_IMAGE_REAL_SIZE       10000
#define RADAR_IMAGE_DISPLAY_SIZE    1000

#define COLOR_SPACE_SIZE            255
#define DATA_SIZE_IN_PACKET         100