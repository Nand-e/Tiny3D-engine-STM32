#ifndef OBJECT3D_H
#define OBJECT3D_H


#include "geometry.h"

template <class T>
class Object3d {
public:
          Object3d   ( const Vertex<T> * verts1, uint8_t vertN1, const Face * faces1, uint8_t faceN1, Vertex<T> scl= { 30,30,30 } );
  void    rotate     ( Matrix<T> & rm, Vertex<T> * rotatedVerts, Vertex<T> scale = Vertex<T> ( 30,30,30 ) );         // Rotate and scale object veertices and put the result in rotated Verts
  Vertex<T>  calcNormal ( const Vertex<T> &p1, const Vertex<T> &p2, const Vertex<T> &p3 );                        // calculate normal of given face

public:
  uint16_t faceN;
  uint16_t vertN;  
  const    Face  * faces;
  const    Vertex<T> * verts;
  VertexInt    position;
  VertexInt    angle;
  
};

// because tempalte class
#include "object3d.cpp"

#endif

