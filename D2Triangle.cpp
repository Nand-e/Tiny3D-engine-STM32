#include "D2Triangle.h"

#include <algorithm>
using namespace std;


RenderPoint::RenderPoint (uint16_t x, uint16_t y, uint16_t deep, uint8_t u1, uint8_t v1){
    this->x = x;    this->y = y;
    this->deep = deep;
   	this->u = u1;	this->v = v1;
}

D2Triangle::D2Triangle()
{
}

D2Triangle::D2Triangle( RenderPoint pp1, RenderPoint pp2, RenderPoint pp3) {
    p0 = pp1;
    p1 = pp2;
    p2 = pp3;
}
void D2Triangle::calculate () {
    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (p0.y > p1.y) swap(p0, p1);
    if (p1.y > p2.y) swap(p2, p1 );
    if (p0.y > p1.y) swap(p0, p1);

    int16_t dx01 = p1.x    - p0.x;
    dy01 = p1.y    - p0.y;
    dxy01 = (float)  dx01 * 1 / ( float ) dy01;
    dd01 = p1.deep - p0.deep;

    int16_t dx02 = p2.x - p0.x;
    dy02 = p2.y - p0.y;
    dxy02 = (float)  dx02 / dy02;
    dd02 = p2.deep - p0.deep;    
   
    int16_t dx12 = p2.x - p1.x;
    dy12 = p2.y - p1.y;
    dxy12 = (float)  dx12 / dy12;
    
    dd12 = p2.deep - p1.deep;


    
}

bool D2Triangle::scanLine2(uint16_t line, RenderPoint &pl, RenderPoint &pr) {
   // static bool first = true;
    //if ( line < p0.y || line > p2.y ) return false;                           // moved outside for performans reason 
    if( p0.y == p2.y) { // Handle awkward all-on-same-line case as its own thing
     // a = b = x0;
     // if(x1 < a)      a = x1;
     // else if(x1 > b) b = x1;
     // if(x2 < a)      a = x2;
     // else if(x2 > b) b = x2;
     // drawFastHLine(a, y0, b-a+1, color);
      return false;
    }
    uint16_t ypart = line - p0.y;
    // ezeket ki lehet elore szamolni
     if ( line <= p1.y) {
        pr.x    = (float) ypart * dxy01 + p0.x;        
        int16_t du = p1.u - p0.u;
        int16_t dv = p1.v - p0.v;
        pr.u = p0.u + du * ( float ) ypart / (float) dy01;
        pr.v = p0.v + dv * ( float ) ypart / (float) dy01;  
        pl.x    = (float) ypart * dxy02 + p0.x;     
        du = p2.u - p0.u;
        dv = p2.v - p0.v;   
        pl.u = p0.u + du * (float)ypart / (float)dy02;
        pl.v = p0.v + dv * (float)ypart / (float)dy02;
  //      Serial.print ( "pr.u: ");  Serial.print ( pr.u );    Serial.print ( ", pr.v: ");    Serial.print ( pr.v ); 
 //      Serial.print ( ", pl.u: " ); Serial.print ( pl.u );  Serial.print ( ", pl.v: " ); Serial.println ( pl.v );

    } else {
        pl.x    = (float) ypart *dxy02 + p0.x;     
        int16_t du = p2.u - p0.u;
        int16_t dv = p2.v - p0.v;   
        pl.u = p0.u + du * (float)ypart / (float)dy02;
        pl.v = p0.v + dv * (float)ypart / (float)dy02;        
     
        pr.x    = (float) (line - p1.y) * dxy12 + p1.x;      
        du = p2.u - p1.u;
        dv = p2.v - p1.v;   
        pr.u = p1.u + du * (float)(line - p1.y) / (float)dy12;
        pr.v = p1.v + dv * (float)(line - p1.y) / (float)dy12;        
    }
    if ( pl.x> pr.x) swap (pr, pl);
    return true;
}

bool D2Triangle::scanLine(uint16_t line, RenderPoint &pl, RenderPoint &pr) {
   // static bool first = true;
    //if ( line < p0.y || line > p2.y ) return false;                           // moved outside for performans reason 
    if( p0.y == p2.y) { // Handle awkward all-on-same-line case as its own thing
     // a = b = x0;
     // if(x1 < a)      a = x1;
     // else if(x1 > b) b = x1;
     // if(x2 < a)      a = x2;
     // else if(x2 > b) b = x2;
     // drawFastHLine(a, y0, b-a+1, color);
      return false;
    }
    uint16_t ypart = line - p0.y;
    // ezeket ki lehet elore szamolni
     if ( line <= p1.y) {
        pr.x    = (float) ypart * dxy01 + p0.x;
        pr.deep = (float) ypart / (float) dy01 * dd01 + p0.deep;
		    
		    int16_t du = p1.u - p0.u;
		    int16_t dv = p1.v - p0.v;
		    pr.u = p0.u + du * ( float ) ypart / (float) dy01;
        pr.v = p0.v + dv * ( float ) ypart / (float) dy01;  

		    pl.x    = (float) ypart * dxy02 + p0.x;
        pl.deep = (float) ypart / (float) dy02 * dd02 + p0.deep;			
		    du = p2.u - p0.u;
		    dv = p2.v - p0.v;		
  		  pl.u = p0.u + du * (float)ypart / (float)dy02;
	      pl.v = p0.v + dv * (float)ypart / (float)dy02;
  //      Serial.print ( "pr.u: ");  Serial.print ( pr.u );    Serial.print ( ", pr.v: ");    Serial.print ( pr.v ); 
 //      Serial.print ( ", pl.u: " ); Serial.print ( pl.u );  Serial.print ( ", pl.v: " ); Serial.println ( pl.v );

    } else {
        pl.x    = (float) ypart *dxy02 + p0.x;
        pl.deep = (float) ypart / (float) dy02 * dd02  + p0.deep;
        int16_t du = p2.u - p0.u;
        int16_t dv = p2.v - p0.v;   
        pl.u = p0.u + du * (float)ypart / (float)dy02;
        pl.v = p0.v + dv * (float)ypart / (float)dy02;
        
        pr.x    = (float) (line - p1.y) * dxy12 + p1.x;
        pr.deep = (float) (line - p1.y) / (float) dy12 * dd12 + p1.deep;
        du = p2.u - p1.u;
        dv = p2.v - p1.v;   
        pr.u = p1.u + du * (float)(line - p1.y) / (float)dy12;
        pr.v = p1.v + dv * (float)(line - p1.y) / (float)dy12;        
    }
    if ( pl.x> pr.x) swap (pr, pl);
    return true;
}

