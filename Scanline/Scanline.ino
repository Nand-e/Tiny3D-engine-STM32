

#include <Adafruit_GFX_AS.h>    // Core graphics library, with extra fonts.
#include "Adafruit_ILI9341_STM.h" // STM32 DMA Hardware-specific library
#include <SPI.h>

#include "geometry.h"
#include "object3d.h"

#include "models.h"       // include 3d models
#include "Renderer.h"
#include "D2Triangle.h"

#define cs   10
#define rst  9 
#define dc   8


#define MAXFACE 150
D2Triangle d2s [MAXFACE];

Vertex<long>  rotatedVerts [140];
Vertex<float>  light2( 0, 0, 1);

uint16_t  colors [64];
bool      wire = false, deepTest = true;

Adafruit_ILI9341_STM tft = Adafruit_ILI9341_STM(cs, dc, rst);       // Invoke custom library

#define WIDTH 320
#define HEIGHT 220
Renderer renderer ( tft, WIDTH, HEIGHT );


// 3Dobjets and container for them
#define MAXOBJECTS 3
Object3d<long>  obj ( verts2, numberV2, faces2, numberF2 );
//Object3d<float>  obj1( verts1, numberV1, faces1, numberF1 );
//Object3d<float>   * actual;



/*******************************************************************/
void setup() {
  //actual = &obj;
  
  Serial.begin( 57600 );
  tft.begin();
  tft.setRotation(3);
  for ( int i =0; i < 4; i++)
  tft.fillScreen( random(65000)  );
  tft.fillScreen( 0x0003 ); 
  uint8_t c=32;
  for ( int i =0; i < 32; i ++ ) {
    c+= 3;
    colors[i] = (( c/2 & 0xF8) << 8) | (( c/2 & 0xFC) << 3) | (c >> 3);    
  }
  c=128;
  colors[0] = 0x0000;
  for ( int i =32; i < 64; i ++ ) {
    c+=4;
    colors[i] = (( c/2 & 0xF8) << 8) | (( c/2 & 0xFC) << 3) | (c >> 3);
  }  
  for (int i=0; i < 64; i++ ) {
    tft.fillRect(i*2,0,2,4, colors[i]);
  }
 
  tft.setTextColor(ILI9341_WHITE);       
  tft.setTextSize(2);
}


void handleSerial () {   
  if (Serial .available () ){
    char rb = Serial.read();
    switch ( rb ) {
   
      case 'p':  
            if ( wire ) wire = false;
            else wire = true;
            break;      
      case 'o':  
            if ( deepTest ) deepTest = false;
            else deepTest = true;
            break;
      
   /*   case '1':  actual = objs[0]; break;*/
   /*  case '2':  actual = objs[1]; break;*/

  /*    case 'i':  actual->angle.x+=3; break;
      case 'm':  actual->angle.x-=3; break;
      
      case 'j':  actual->angle.y+=3; break;
      case 'k':  actual->angle.y-=3; break;
      
      case 'd':  actual->position.x+=5; break;
      case 'a':  actual->position.x-=5; break;
      case 's':  actual->position.y+=5; break;
      case 'w':  actual->position.y-=5; break;
      case 'r':  actual->position.z+=5; break;
      case 'f':  actual->position.z-=5; break;     */
    
    }    
  }
}

/* translate vertexes 
 ***************************************************************************/  
template < class T >
void translate ( Vertex<T> * verts, Vertex<T> & eltol, uint8_t number ) {
   for ( uint8_t i=0; i < number; i++ ) {    
      verts[i].x += eltol.x;
      verts[i].y += eltol.y;
      verts[i].z += eltol.z;
   }
}

/* Create 2D faces
 * project vertex, back face culling, lighting 
 **********************************************************/
template < class T >
void create2DFaces( Object3d<T> & obj, Vertex<T> * verts, uint16_t color, uint8_t & facecounter) {

  #define CENTERX 150
  #define CENTERY 100
  Vertex<float> normal; 
  float fc, fl;  
  // project all vberticies to 2D plane
  for ( int i = 0; i < obj.vertN; i++ )  {
     verts[i].x =  CENTERX + verts[i].x / ( verts[i].z / 200);  
     verts[i].y =  CENTERY + verts[i].y / ( verts[i].z / 200);   
  }
  // create faces
  int i1, i2, i3;
  const uint16_t * faces = ( uint16_t * )obj.faces;
  uint16_t x,x1,x2, y,y1,y2;
  for ( int i =0; i < obj.faceN; i++ ) {
     i1 = pgm_read_word( faces );     faces++;
     i2 = pgm_read_word( faces );     faces++;
     i3 = pgm_read_word( faces );     faces++; 
 
     x =  verts[i1].x; y  = verts[i1].y;       
     x1 = verts[i2].x; y1 = verts[i2].y;        
     x2 = verts[i3].x; y2 = verts[i3].y;       
    
    // back face culling 
    int16_t n = (x1-x)*(y1 + y) + (x2-x1) *(y2+y1) + (x - x2)*(y + y2);
    if ( n > 0 ) { 
       // no need for textureing only for lighting normal = obj.calcNormal(verts[i1], verts[i2], verts[i3] );           // MISTAKE I calculate the normals from projected verts. I have to repair this TODO
       // calculate light
       //fl = normal.dot( light2 );
       //if ( fl < 0  ) fl =0;    
       uint8_t cw  =  10+ 53 * fl;
       cw %= 64;  
       if ( facecounter < MAXFACE) {
          d2s[facecounter].p0 = RenderPoint ( x ,y,  verts[i1].z*50 , UV[i1][0], UV[i1][1]);
          d2s[facecounter].p1 = RenderPoint ( x1,y1, verts[i2].z*50 , UV[i2][0], UV[i2][1]);
          d2s[facecounter].p2 = RenderPoint ( x2,y2, verts[i3].z *50, UV[i3][0], UV[i3][1]);
          d2s[facecounter].calculate();
          d2s[facecounter].color = colors[cw] | color; 
          d2s[facecounter].textId = (i/2)%5;      // TODO move to faces
          d2s[facecounter].width  = 80;            
          facecounter++;
       }     
    }
  }   
}

/*
 * 
 *****************************************************************************/
 void drawGraph ( uint8_t rt, uint8_t ct ) {
  //rt %= 1000;
  //ct %= 1000;

 // tft.fillRect ( 5, 120, 6, 100-rt/5  , 5 );
 // tft.fillRect ( 5, 219 -rt, 6, rt , 31 << 5);
  tft.setCursor(0, 221); 
  tft.fillRect ( 0,220, 25, 18,31 << 5);
  tft.print ( rt);   
}
/************************************************************************/
Vertex<float>  o2, o3;            

Matrix<long> matrix;
Matrix<long> matrix2;    
   
long lastDraw; 
int16_t angle = 0;



void loop() {   
   lastDraw = millis();   
   handleSerial();
   handleSerial();
   handleSerial();

   angle += 2;
   angle %= 720; 

   // rotate objects and create faces
   uint8_t d2counter = 0;                                                     // number of face to draw ( after back face culling )

   int i =0; 
   matrix.createRotM ( angle ,angle*2 , angle *2 );
   obj.rotate ( matrix, rotatedVerts, Vertex<long> ( 1,1,1 ) );             // rotate obj vers and put the result in rotetdVerts array 

   Vertex<long> pos;
   pos.x = -150;
   pos.y =  100;
   pos.z =  650;   
   translate ( rotatedVerts, pos , obj.vertN );
   create2DFaces<long> ( obj, rotatedVerts,  0xf000, d2counter );           // create faces and give the created number of faces back in d2counter

   /* 
   matrix.createRotM ( -30 , -angle , angle *3 );
   obj.rotate ( matrix, rotatedVerts, Vertex<int16_t> ( 2,2,2 ) );   
   pos.x = 150;
   pos.y = -100;
   pos.z = 600;   
   translate ( rotatedVerts, pos , obj.vertN );
   create2DFaces<int16_t>  ( obj, rotatedVerts,  0xf000, d2counter );  */
  
   drawGraph ( d2counter, 0);
   
   // draw faces ----------------------------------------------
   tft.setAddrWindow ( 0, 0,   WIDTH- 1,  HEIGHT );
  // renderer.renderWithTexture ( d2s, d2counter ); 
  /// Serial.println ( "-------" );
   if ( wire ) {
      renderer.renderWires ( d2s, d2counter );                                      // d2 scanline could be optimized a lot i have to check  the original draTriangle agorithm
   }
   else {
     renderer.renderWithTexture ( d2s, d2counter, 0);  
    /*if ( deepTest )    
      renderer.renderWithDeep    (d2s, d2counter);
    else 
      renderer.renderWithoutDeep (d2s, d2counter);*/
   }
   
 }













