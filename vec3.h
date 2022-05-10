#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include "gen_library.h"

using std::sqrt;

class vec3 {
  public:
    vec3() : e{0,0,0} {}
    vec3(double e0, double e1, double e2) : e{e0,e1,e2} {};
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3( -e[0], -e[1], -e[2]); };
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }
    vec3& operator+=(const vec3 &v ){
      e[0] += v.e[0];
      e[1] += v.e[1];
      e[2] += v.e[2];
      return *this;
    }

    vec3& operator*=(const double t){
      e[0] *= t;
      e[1] *= t;
      e[2] *= t;
      return *this;
    }

    vec3& operator/=(const double t){
      return *this *= 1/t;
    }

    double length_squared() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; };
    double length() const { return sqrt( length_squared() ); };

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }

  public:
    double e[3];
};

using point3 = vec3;
using color = vec3;

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
  return out << v[0] << ' ' << v[1] << ' ' << v[2];
}
inline vec3 operator+(const vec3 &u, const vec3 &v) {
  return vec3( u[0] + v[0], u[1] + v[1], u[2] + v[2] );
}
inline vec3 operator-(const vec3 &u, const vec3 &v) {
  return vec3( u[0] - v[0], u[1] - v[1], u[2] - v[2] );
}
inline vec3 operator*(const vec3 &u, const vec3 &v) {
  return vec3( u[0]*v[0], u[1]*v[1], u[2]*v[2] );
}
inline vec3 operator*(double t, const vec3 &v) {
  return vec3( t*v[0], t*v[1], t*v[2]);
}
inline vec3 operator*(const vec3 &v, double t) {
  return t*v;
}
inline vec3 operator/(vec3 v, double t) {
  return vec3( v[0]/t, v[1]/t, v[2]/t);
}
inline double dot(const vec3 &u, const vec3 &v) {
  return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
}
inline vec3 cross(const vec3 &u, const vec3 &v) {
  return vec3( u[1]*v[2] - u[2]*v[1], u[2]*v[0] - u[0]*v[2], u[0]*v[1] - u[1]*v[0]);
}
inline vec3 unit_vector(vec3 v) {
  return v / v.length();
}
inline vec3 random_vec() {
    return vec3( random_double(), random_double(), random_double() );
}
inline vec3 random_vec( double min, double max ) {
    return vec3( random_double( min, max ), random_double( min, max ), random_double( min, max ) );
}

vec3 random_in_unit_sphere() {
  while( true )
  {
    vec3 random_pt = random_vec( -1, 1 );
    if( random_pt.length_squared() >= 1 ) continue;
    return random_pt;
  }
}

vec3 random_unit_vector() { return( unit_vector( random_in_unit_sphere() ) ); }

vec3 random_in_unit_disk() {
  while( true )
  {
    vec3 random_pt = vec3( random_double( -1, 1 ), random_double( -1, 1 ), 0 );
    if( random_pt.length_squared() >= 1 ) continue;
    return random_pt;
  }
}
// n is normal to a surface, v is the entering ray. if the angle between v and n is theta,
// then the angle the reflected v makes with the surface is 90-theta. the magnitude of the vector
// B, which is perpendicular to the surface and touches the end of v, is || v || sin( 90 - theta )
// = || v || cos( theta ) = || v || * 1 * cos( theta ) = || v || || n || cos( theta ) = v dot n
vec3 reflect( const vec3& v, const vec3& n ) {
  return v - 2*dot( v, n )*n;
}

vec3 refract( const vec3& uv, const vec3& n, const double etai_over_etat ) {
  // check why we have a min
  vec3 r_perpendicular = etai_over_etat * ( uv + fmin( dot( -uv, n ), 1.0 )*n );
  vec3 r_parallel = -1*sqrt( fabs( 1 - r_perpendicular.length_squared() ) )*n;
  return r_perpendicular + r_parallel;
}

#endif
