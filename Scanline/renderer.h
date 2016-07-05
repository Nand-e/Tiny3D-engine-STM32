#ifndef RENDERER_H
#define RENDERER_H

#include "D2Triangle.h"
#include "Adafruit_ILI9341_STM.h"

#define TFTWIDTH 320




class Renderer
{
public:

    Renderer(Adafruit_ILI9341_STM & tft1 ,uint16_t width1, uint16_t height1);
    Adafruit_ILI9341_STM & tft; 
    void renderWires ( D2Triangle * d2s, uint8_t length );
    void renderWithoutDeep (D2Triangle * d2s, uint8_t length );
    void renderWithDeep ( D2Triangle * d2s, uint8_t length );
	  void renderWithTexture(D2Triangle * d2s, uint8_t length, const uint16_t * p);
    
    uint16_t colorLine[TFTWIDTH];
    uint16_t deepLine [TFTWIDTH];
    uint16_t wid, hei;

    const uint16_t * images[6];      // pointer to images

};

#endif // RENDERER_H
