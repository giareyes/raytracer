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

class dielectric : public material {
  public:
    dielectric( const double index_of_refraction ) : ir( index_of_refraction ) {}

    virtual bool scatter( const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered ) const override {
      double index_ratio = rec.front_face ? 1.0 / ir : ir;
      vec3 unit_dir = unit_vector( r_in.direction() );
      double cos_theta = fmin( dot( -unit_dir, rec.normal ), 1.0  );
      double sin_theta = sqrt( 1.0 - cos_theta*cos_theta );
      vec3 scatter_direction;
      if( index_ratio*sin_theta > 1 || reflectance( cos_theta, index_ratio ) > random_double() ) {
        scatter_direction = reflect( unit_dir, rec.normal );
      }
      else {
        scatter_direction = refract( unit_dir, rec.normal, index_ratio );
      }

      scattered = ray( rec.p,  scatter_direction );
      attenuation = color( 1.0, 1.0, 1.0 );
      return true;
    };

  public:
    double ir;

  private:
    static double reflectance( double cosine, double ref_idx ) {
      // schlick's approximation equation
      auto r0 = (1-ref_idx) / (1+ref_idx);
      r0 = r0*r0;
      return r0 + (1-r0)*pow((1 - cosine),5);
    }
};

#endif
