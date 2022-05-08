#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"

struct hit_record;

class material {
  public:
    virtual bool scatter( const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered ) const = 0 ;
};

class lambertian : public material {
  public:
    lambertian( const color& a ) : albedo( a ) {}

    virtual bool scatter( const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered ) const override {
      vec3 scatter_direction = rec.normal + random_unit_vector();
      // if zero, will lead to problems later
      if( scatter_direction.near_zero() ) scatter_direction = rec.normal;

      scattered = ray( rec.p,  scatter_direction );
      attenuation = albedo;
      return true;
    };

  public:
    color albedo;
};

class metal : public material {
  public:
    metal( const color& a, const double f ) : albedo( a ), fuzz( f < 1 ? f : 1 ) {}

    virtual bool scatter( const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered ) const override {
      vec3 scatter_direction = reflect( unit_vector( r_in.direction() ), rec.normal );
      scatter_direction = scatter_direction + fuzz*random_in_unit_sphere();
      scattered = ray( rec.p,  scatter_direction );
      attenuation = albedo;
      return ( dot( scattered.direction(), rec.normal ) > 0 );
    };

  public:
    color albedo;
    double fuzz;
};

#endif
