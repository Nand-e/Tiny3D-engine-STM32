#include "renderer.h"


Renderer::Renderer( Adafruit_ILI9341_STM & tft1, uint16_t width1, uint16_t height1) :
    tft (tft1), wid ( width1 ), hei ( height1 )
{
}


void Renderer::renderWithDeep(D2Triangle * d2s, uint8_t length ) {
    RenderPoint lp, rp;
    bool needclear = true;
    for ( int y= 0; y < hei; y++) {
        if ( needclear ) {
          memset ( colorLine, 0x00, wid*2 );
          memset ( deepLine , 0xff, wid*2 );
          needclear = false;
        }
        for ( unsigned int d = 0; d < length; d++ )     
            if ( y >= d2s[d].p0.y && y <= d2s[d].p2.y ) {                   // moved here from D2.scanline to save calling cost for performance reason // public vars etc... ARghhhh
                d2s[d].scanLine( y, lp, rp) ;
                if ( lp.x <= 0  ) continue; // || rp.x >= wid ) continue;
                if ( rp.x >= wid )  rp.x = wid;
                
                int16_t dd = ( rp.deep - lp.deep ) / ( rp.x- lp.x +1);      // calcualte deep steps
                unsigned int  nc = lp.deep;
                if ( !needclear ) {                                       // first line no need deep test
                  for ( int x = lp.x; x < rp.x; x++ ) {                     
                          colorLine[x] =  d2s[d].color;
                          deepLine [x] =  nc;                      
                          nc+= dd;
                 }
               } else {
                  for ( int x = lp.x; x < rp.x; x++ ) {
                      if ( deepLine[x]  > nc  ) {
                          colorLine[x] =  d2s[d].color;
                          deepLine [x] =  nc;
                      }
                      nc+= dd;
                }
               }
               needclear = true;
            }
         tft.pushLine ( colorLine , wid );
    }
}

void Renderer::renderWithoutDeep(D2Triangle * d2s, uint8_t length ) {
    RenderPoint lp, rp;
    bool needclear = true;
    for ( int y= 0; y < hei; y++) {
        if ( needclear ) {
          memset ( colorLine, 0x00, wid*2 );
          //memset ( deepLine , 0xff, wid*2 );
          needclear = false;
        }
        for ( unsigned int d = 0; d < length; d++ )     
            if ( y >= d2s[d].p0.y && y <= d2s[d].p2.y ) {  // moved here from D2.scanline to save calling cost for performance reason // public vars etc... ARghhhh
                d2s[d].scanLine( y, lp, rp) ;
                if ( lp.x <= 0 || rp.x >= wid ) continue;                         
                for ( int x = lp.x; x < rp.x; x++ ) {                     
                          colorLine[x] =  d2s[d].color;                        
                 }     
               needclear = true;
            }
            tft.pushLine ( colorLine , wid );
       }
}
void Renderer::renderWires(D2Triangle * d2s, uint8_t length ) {
    RenderPoint lp, rp;
    for ( int y= 0; y < hei; y++) {
        memset ( colorLine , 0x00, wid *2);
       //memset ( deepLine , 0xff,  wid *2 );
        for ( unsigned int d = 0; d < length; d++ )
            if (  y > d2s[d].p0.y && y < d2s[d].p2.y  ) {            // moved here from D2.scanline to save calling cost for performance reason // public vars etc... ARghhhh
                d2s[d].scanLine( y, lp, rp);
                if ( lp.x <= 0 || rp.x >= wid ) continue;
                colorLine[rp.x] = d2s[d].color;
                colorLine[lp.x] = d2s[d].color;
            }
         tft.pushLine ( colorLine , wid );
    }
}


