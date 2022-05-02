#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
  public:
    sphere() {}
    sphere( point3 cen, double r ): center( cen ), radius ( r ) {}

    virtual bool hit( const ray& r, double t_min, double t_max, hit_record& rec ) const override;

  public:
    point3 center;
    double radius;
};

bool sphere::hit( const ray& r, double t_min, double t_max, hit_record& rec ) const {
  vec3 oc = r.origin() - center;
  double a = r.direction().length_squared();
  double hb = dot( r.direction(), oc );
  double c = oc.length_squared() - radius*radius;

  double discriminant = hb*hb - a*c;
  if( discriminant < 0 ) return false;

  auto root = ( -hb - sqrt( discriminant ) ) / a;
  if( root < t_min || root > t_max ) {
    root = ( -hb + sqrt( discriminant ) ) / a;
    if( root < t_min || root > t_max ) return false;
  };

  rec.t = root;
  rec.p = r.at( root );
  vec3 outward_normal = ( r.at( root ) - center )/ radius;
  rec.set_face_normal( r, outward_normal );

  return true;
};

#endif
