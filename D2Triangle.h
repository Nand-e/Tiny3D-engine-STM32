
#ifndef D2TRIANGLE
#define D2TRIANGLE

#include <Arduino.h>


class RenderPoint
{
public:
    RenderPoint ( uint16_t x=0, uint16_t y=0, uint16_t deep = 1 );
    uint16_t x,y,deep;
};

class D2Triangle
{
public:
    D2Triangle();
    D2Triangle( RenderPoint pp1, RenderPoint pp2, RenderPoint pp3 );

    // retutn the left edge point and right edge point in given line
    // return false if it has not any part of the given line
    bool scanLine ( uint16_t line, RenderPoint & pl, RenderPoint & pr );
    void calculate();

    RenderPoint p0, p1, p2;
    float    dxy01,  dxy02, dxy12;
    
    int16_t  dy02, dy01, dy12;
    int16_t  dd01, dd02, dd12;
    uint16_t color;
};

#endif
