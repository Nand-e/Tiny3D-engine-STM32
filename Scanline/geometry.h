#ifndef POINT_H
#define POINT_H

#include <Arduino.h>

#ifdef  FIXEDPOINT
#define FIXED * 128
#else
#define FIXED * 128
#endif



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

#define FIXEDPOINT


template <class T>
class Matrix  {
public:  
  friend Vertex<T> operator* (const Vertex<T> & vec, const Matrix<T> & m);
  /**************************************************************/
  void createRotM ( long x, long y, long z=0 ) {
     float x2 = x * M_PI / 180;
     float y2 = y * M_PI / 180;
     float z2 = z * M_PI / 180;  

     float cosz = cos ( z2 );     float sinz = sin ( z2 );
     float siny = sin ( y2 );     float cosy = cos ( y2 );
     float sinx = sin ( x2 );     float cosx = cos ( x2 );

     m00 = cosy*cosz*128;  m10 = ( cosz*sinx*siny-cosx*sinz )* 128;  m20 = ( cosx*cosz*siny + sinx*sinz )  * 128;
     m01 = cosy*sinz*128;  m11 = ( cosx*cosz+sinx*siny*sinz )* 128;  m21 = ( cosx*siny*sinz - cosz * sinx )* 128;
     m02 = -siny    *128;     m12 =  cosy*sinx * 128;                m22 = cosx*cosy *128;
  }
  
  T m00, m10, m20;  
  T m01, m11, m21;  
  T m02, m12, m22;
};


template < class T >  
inline Vertex<T>  operator*( const Vertex<T> & ver, const Matrix<T> & m) {

    T x = m.m00 *ver.x + m.m10 * ver.y + m.m20 * ver.z;
    T y = m.m01 *ver.x + m.m11 * ver.y + m.m21 * ver.z;
    T z = m.m02 *ver.x + m.m12 * ver.y + m.m22 * ver.z;
    Vertex<T> v(x,y,z);
    return v;
}



#endif

