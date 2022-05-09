#ifndef CAMERA_H
#define CAMERA_H

#include "gen_library.h"

class camera {
  public:
    // look from is the where the camera is at, look at is the point we are staring at, vup is the
    // up direction for the camera
    camera( point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspect_ratio ) {
      auto theta = degrees_to_radians( vfov );
      auto h = tan( theta/2 );
      auto viewport_height = 2.0*h;
      auto viewport_width = aspect_ratio * viewport_height;

      auto w = unit_vector( lookfrom - lookat );
      auto u = unit_vector( cross( vup, w ) );
      auto v = cross( w, u );

      origin = lookfrom;
      horizontal = viewport_width*u;
      vertical = viewport_height*v;
      lower_left_corner = origin - horizontal/2 - vertical/2 - w;
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
