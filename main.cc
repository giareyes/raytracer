#include "ray.h"
#include "color.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"

// compile with g++ -std=c++11 main.cc -o main because for some reason the default g++ on my computer
// uses version 4.2.1 ??? TToTT

// depth param used to prevent recursion from lasting too long
color ray_color( const ray& r, const hittable_list& world, int depth ) {
  // if we have recursed for too long, there is no more color to accumulate and we return black
  if( depth <= 0 )
    return color( 0, 0, 0 );

  hit_record rec;
  // use 0.001 to prevent near hits from being factored in
  if( world.hit( r, 0.001, infinity, rec ) )
  {
    // when we hit a diffuse object, the ray is scattered
    point3 random_pt = random_in_sphere();
    point3 diffuse_target = rec.p + rec.normal + random_pt;
    return 0.5*ray_color( ray( rec.p,  diffuse_target - rec.p ), world, depth - 1 );
  }

  vec3 unit_direction = unit_vector( r.direction() );
  auto t = 0.5*( unit_direction.y() + 1 );
  return ( 1.0 - t )*color( 1.0, 1.0, 1.0 ) + t*color( 0.5, 0.7, 1.0 );
}

int main() {
  // image
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 400;
  const int image_height = static_cast<int>( image_width / aspect_ratio );
  const int samples_per_pixel = 100;
  const int max_depth = 50;
  // world
  hittable_list world;
  world.add( make_shared<sphere>( point3( 0, 0, -1 ), 0.5 ) );
  world.add( make_shared<sphere>( point3( 0, -100.5, -1 ), 100 ) );

  //camera
  camera c;

  // render
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for(int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for(int i = 0; i < image_width; ++i) {
      color pixel_color = color( 0, 0, 0 );
      for( int sample = 0; sample < samples_per_pixel; ++sample ) {
        auto u = double(i + random_double() )/ (image_width-1);
        auto v = double(j + random_double() ) / (image_height-1);
        ray r = c.get_ray( u,v );
        pixel_color += ray_color( r, world, max_depth );
      };

      write_color( std::cout, pixel_color, samples_per_pixel );
    }
  }
  std::cerr << "\nDone.\n";
}
