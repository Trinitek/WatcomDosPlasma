#include <stdint.h>
#include <graph.h>
#include <math.h>
#include <stdbool.h>
#include <i86.h>

uint8_t character(double d)
{
    int i = (int)round(d * 2) + 2;

    switch (i)
    {
        case 0:
            return ' ';     // 0%
        case 1:
            return 0xB0;    // 25%
        case 2:
            return 0xB1;    // 50%
        case 3:
            return 0xB2;    // 75%
        case 4:
            return 0xDB;    // 100%
        default:
            return '?';
    }
}

void put(uint16_t* buffer, uint8_t c)
{
    uint16_t color = 1;
    *buffer = (uint16_t)c | color << 8;
}

void frame(int tick)
{
    const short width = 80;
    const short height = 25;

    uint16_t* video = (uint16_t*)MK_FP(0xB000, 0x8000);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            tick = tick == 0 ? 1 : tick;

            double a = x / (height / 16.0);
            double b = y / (width / 128.0);
            
            double c = (x - width / 2.0) * (x - width / 2.0);
            double d = (y - height / 2.0) * (y - height / 2.0);
            
            double t = sin(tick / 8.0);
            
            double e = sin(((tick * 2.0) + ((t - 0.5) * a)) / 8.0);
            double f = sin(((tick / 2.0) + b) / 8.0);
            double g = sin(sqrt(c + d * 1.1) / (t + 4.0) * 0.1);
            
            double intensity = (e + f + g) / 3.0;

            put(video, character(intensity));

            video++;
        }
    }
}

int main()
{
    _setvideomode(_TEXTC80);
    _displaycursor(_GCURSOROFF);

    int tick = 0;

    while(true)
    {
        frame(tick);
        tick++;
    }
    
    return 0;
}
