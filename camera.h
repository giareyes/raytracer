#ifndef CAMERA_H
#define CAMERA_H

#include "gen_library.h"

class camera {
  public:
    // look from is the where the camera is at, look at is the point we are staring at, vup is the
    // up direction for the camera
    camera( point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspect_ratio, double aperature, double focus_dist ) {
      auto theta = degrees_to_radians( vfov );
      auto h = tan( theta/2 );
      auto viewport_height = 2.0*h;
      auto viewport_width = aspect_ratio * viewport_height;

      w = unit_vector( lookfrom - lookat );
      u = unit_vector( cross( vup, w ) );
      v = cross( w, u );

      origin = lookfrom;
      horizontal = focus_dist * viewport_width * u;
      vertical = focus_dist * viewport_height * v;
      lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

      lens_radius = aperature / 2;
    }
    ray get_ray( double s, double t ) const {
      vec3 lens_pt = lens_radius * random_in_unit_disk();
      vec3 offset = u*lens_pt.x() + v*lens_pt.y();
      return ray( origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset );
    }

  private:
    point3 origin;
    point3 lower_left_corner;
    vec3 vertical, horizontal;
    vec3 w, u, v;
    double lens_radius;
};
#endif
