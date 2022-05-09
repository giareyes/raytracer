#ifndef CAMERA_H
#define CAMERA_H

#include "gen_library.h"

class camera {
  public:
    camera( double vfov, double aspect_ratio ) {
      auto theta = degrees_to_radians( vfov );
      auto h = tan( theta/2 );
//      const auto aspect_ratio = 16.0 / 9.0;
      auto viewport_height = 2.0*h;
      auto viewport_width = aspect_ratio * viewport_height;
      auto focal_length = 1.0;

      origin = point3( 0,0,0 );
      horizontal = vec3( viewport_width, 0, 0 );
      vertical = vec3( 0, viewport_height, 0 );
      lower_left_corner = origin - horizontal/2 - vertical/2 - vec3( 0, 0, focal_length );
    }
    ray get_ray( double u, double v ) const {
      return ray( origin, lower_left_corner + u*horizontal + v*vertical - origin );
    }

  private:
    point3 origin;
    point3 lower_left_corner;
    vec3 vertical;
    vec3 horizontal;
};
#endif
