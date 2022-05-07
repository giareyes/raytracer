#ifndef GEN_LIBRARY_H
#define GEN_LIBRARY_H

// Common Headers
#include <cstdlib>
#include <iostream>

// Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians( double degrees ) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    // Returns a random real in [0,1)
    return rand() / ( RAND_MAX + 1.0) ;
}

inline double random_double( double min, double max ) {
    // Returns a random real in [min,max)
    return min + ( max - min )*random_double();
}

inline double clamp( double x, double min, double max ) {
  if( x > max ) return max;
  if( x < min ) return min;
  return x;
}

#endif
