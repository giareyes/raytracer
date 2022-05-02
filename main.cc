#include <iostream>
#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "sphere.h"
#include "hittable_list.h"

// compile with g++ -std=c++11 main.cc -o main because for some reason the default g++ on my computer
// uses version 4.2.1 ??? TToTT
// double hit_sphere( const point3& center, double radius, const ray& r ) {
//   // r^2 = (t^2 * b . b ) + 2tb ( A - C ) + (a - c )^2
//   vec3 oc = r.origin() - center;
//   double a = r.direction().length_squared();
//   double hb = dot( r.direction(), oc );
//   double c = oc.length_squared() - radius*radius;
//
//   double discriminant = hb*hb - a*c;
//   if( discriminant < 0 ) {
//     return -1.0;
//   }
//   else {
//     return ( -hb - sqrt( discriminant ) ) / a;
//   }
// }

color ray_color( const ray& r, const hittable_list& world ) {
  // sphere s1 = sphere( point3( 0, 0, -1 ), 0.5 );
  hit_record rec;
  if( world.hit( r, 0, std::numeric_limits<double>::infinity(), rec ) )
    return 0.5*( rec.normal + color( 1.0, 1.0, 1.0 ) );

  // double contact_pt = hit_sphere( point3( 0, 0, -1 ), 0.5, r );
  // if( contact_pt > 0.0 ) {
  //   vec3 normal = unit_vector( r.at( contact_pt ) - point3( 0, 0, -1 ) ); // hit point - center of sphere
  //   return 0.5*color( normal.x() + 1.0, normal.y() + 1, normal.z() + 1 );
  // }
  vec3 unit_direction = unit_vector( r.direction() );
  auto t = 0.5*( unit_direction.y() + 1 );
  return ( 1.0 - t )*color( 1.0, 1.0, 1.0 ) + t*color( 0.5, 0.7, 1.0 );
}

int main() {
  // image
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 400;
  const int image_height = static_cast<int>( image_width / aspect_ratio );

  // world
  hittable_list world;
  world.add( make_shared<sphere>( point3( 0, 0, -1 ), 0.5 ) );
  world.add( make_shared<sphere>( point3( 0, -100.5, -1 ), 100 ) );

  //camera
  auto viewport_height = 2.0;
  auto viewport_width = aspect_ratio * viewport_height;
  auto focal_length = 1.0;

  auto origin = point3( 0,0,0 );
  auto horizontal = vec3( viewport_width, 0, 0 );
  auto vertical = vec3( 0, viewport_height, 0 );
  auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3( 0, 0, focal_length );

  // render
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for(int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for(int i = 0; i < image_width; i++) {
      auto u = double(i)/ (image_width-1);
      auto v = double(j) / (image_height-1);
      ray r( origin, lower_left_corner + u*horizontal + v*vertical - origin );
      color pixel_color = ray_color( r, world );

      write_color( std::cout, pixel_color );
    }
  }
  std::cerr << "\nDone.\n";
}
