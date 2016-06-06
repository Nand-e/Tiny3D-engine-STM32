#ifndef POINT_H
#define POINT_H

#include <Arduino.h>

struct Face {
  uint16_t v1;
  uint16_t v2; 
  uint16_t v3; 
};


struct VertexInt {
  int16_t x;
  int16_t y; 
  int16_t z; 
};

template 
< class T>
struct Vertex {
  T x;
  T y;
  T z;
  
  /*int16_t x;
  int16_t y;
  int16_t z;*/
  //Vertex (){};
  Vertex<T> ( float x1 =0, float y1=0, float z1=0 ) {
    x = x1; y = y1; z = z1;
  }
  
  Vertex<T> operator-( const Vertex & rhs ) const {
    Vertex p;
    p.x = this->x - rhs.x;
    p.y = this->y - rhs.y;
    p.z = this->z - rhs.z;
    return p;
  }

  Vertex<T> operator+( const Vertex & rhs ) const {
    Vertex p;
    p.x = this->x + rhs.x;
    p.y = this->y + rhs.y;
    p.z = this->z + rhs.z;
    return p;
  }

  
  Vertex<T> operator+=( const Vertex & rhs ) {
    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;
    return *this;
  }

 
  Vertex<T> operator/( const float & rhs ) const {
    Vertex p;
    p.x = this->x / rhs;
    p.y = this->y / rhs;
    p.z = this->z / rhs;
    return p;
  }

  
  Vertex<T> operator*( const float & rhs ) const {
    Vertex p;
    p.x = this->x * rhs;
    p.y = this->y * rhs;
    p.z = this->z * rhs;
    return p;
  }
  
  float dot ( const Vertex<T> & rhs ) const {
    return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
  }

  float amplitude () {
    return sqrt ( x*x + y*y + z*z);
  }  
};



class Matrix  {
public:  

  template <class T>
  friend Vertex<T> operator* (const Vertex<T> & vec, const Matrix& m);
  /**************************************************************/
  void createRotM ( int16_t x, int16_t y, int16_t z=0 ) {
     float x2 = x * M_PI / 180;
     float y2 = y * M_PI / 180;
     float z2 = z * M_PI / 180;  

     float cosz = cos ( z2 );     float sinz = sin ( z2 );
     float siny = sin ( y2 );     float cosy = cos ( y2 );
     float sinx = sin ( x2 );     float cosx = cos ( x2 );

     m00 = cosy * cosz;  m10 = cosz * sinx*siny -cosx* sinz;     m20 = cosx*cosz*siny + sinx*sinz;
     m01 = cosy * sinz;  m11 = cosx*cosz + sinx* siny * sinz;    m21 = cosx*siny*sinz - cosz * sinx;
     m02 = -siny;        m12 =  cosy*sinx;                       m22 = cosx*cosy;
  }
  
  float m00, m10, m20;  
  float m01, m11, m21;  
  float m02, m12, m22;
};

template < class T >  
inline Vertex<T>  operator*( const Vertex<T> & ver, const Matrix & m) {

    float x = m.m00 *ver.x + m.m10 * ver.y + m.m20 * ver.z;
    float y = m.m01 *ver.x + m.m11 * ver.y + m.m21 * ver.z;
    float z = m.m02 *ver.x + m.m12 * ver.y + m.m22 * ver.z;
    Vertex<T> v(x,y,z);
    return v;
}


#endif

