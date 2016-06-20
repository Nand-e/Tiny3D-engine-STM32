
#ifndef OBJECT3D_C
#define OBJECT3D_C


#include "object3d.h"

template<class T> 
Object3d<T>::Object3d ( const Vertex<T> * verts1, uint8_t vertN1, const Face * faces1, uint8_t faceN1, Vertex<T> scl ){
    verts   = verts1;
    vertN   = vertN1;
    faces   = faces1;
    faceN   = faceN1;
    position.x = 0;
    position.y = 0;
    position.z = 300;
    angle.x = -180;
    angle.y = angle.z = 0;
}

//*********************************************************************************************************  
template <class T>
void Object3d<T>::rotate ( Matrix & rm, Vertex<T> * rotatedVerts, Vertex<T> scale ) {
   float x,y, z;
   float xv, yv, zv;
   float * verts1 = ( float * ) verts;
  // int16_t * verts1 = ( int16_t * ) verts;
   for ( int i=0; i< vertN; i++ ) {     
 
     xv = pgm_read_float( verts1 ) * scale.x;  verts1++;
     yv = pgm_read_float( verts1 ) * scale.y;  verts1++;
     zv = pgm_read_float( verts1 ) * scale.z;  verts1++;
           
     x = rm.m00 * xv + rm.m10 * yv + rm.m20 * zv;
     y = rm.m01 * xv + rm.m11 * yv + rm.m21 * zv;
     z = rm.m02 * xv + rm.m12 * yv + rm.m22 * zv;    
      
     rotatedVerts[i].x = x;// + eltol.x;
     rotatedVerts[i].y = y;// + eltol.y;
     rotatedVerts[i].z = z;// + eltol.z;
  }
} 

//*********************************************************************************************************  
template < class T >
Vertex<T> Object3d<T>::calcNormal( const Vertex<T> &p1, const Vertex<T> &p2, const Vertex<T> &p3 )
{
    Vertex<T> U = (p2 - p1);
    Vertex<T> V = (p3 - p1);
    Vertex<T> surfaceNormal;
    surfaceNormal.x = (U.y*V.z) - (U.z*V.y);     surfaceNormal.y = (U.z*V.x) - (U.x*V.z);
    surfaceNormal.z = (U.x*V.y) - (U.y*V.x);
    return surfaceNormal / surfaceNormal.amplitude();
}

//*********************************************************************************************************  
  
  /*void test () {
    for ( int i=0; i < 10; i++ ) {
      Serial.println ( pgm_read_float ( verts +i ) );
      Serial.println ( pgm_read_float ( verts +i + vertN*1 ) );
      Serial.println ( pgm_read_float ( verts +i + vertN*2 ) );
    }
  }*/

  #endif

