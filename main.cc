#include "ray.h"
#include "color.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "material.h"

// compile with g++ -std=c++11 main.cc -o main because for some reason the default g++ on my computer
// uses version 4.2.1 ??? TToTT

// depth param used to prevent recursion from lasting too long
color ray_color( const ray& r, const hittable_list& world, int depth ) {
  // if we have recursed for too long, there is no more color to accumulate and we return black
  if( depth <= 0 ) return color( 0, 0, 0 );
  hit_record rec;

  // use 0.001 to prevent near hits from being factored in
  if( world.hit( r, 0.001, infinity, rec ) )
  {
    ray scattered;
    color attenuation;
    if( rec.mat_ptr->scatter( r, rec, attenuation, scattered ) )
      return attenuation*ray_color( scattered, world, depth - 1 );

    return color( 0, 0, 0 );
  }

  // if we do not hit any surfaces
  vec3 unit_direction = unit_vector( r.direction() );
  auto t = 0.5*( unit_direction.y() + 1 );
  return ( 1.0 - t )*color( 1.0, 1.0, 1.0 ) + t*color( 0.5, 0.7, 1.0 );
}

// from textbook
hittable_list world1() {
  hittable_list world;
  auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
  auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
  auto material_left   = make_shared<dielectric>(1.5);
  auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0 );

  world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
  world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
  world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
  world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

  return world;
}

// from textbook
hittable_list random_scene() {
  hittable_list world;
  auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

  for(int a = -11; a < 11; a++ ) {
    for(int b = -11; b < 11; b++ ) {
      auto choose_mat = random_double();
      point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<material> sphere_material;

        if (choose_mat < 0.8) {
            // diffuse
            auto albedo = random_vec() * random_vec();
            sphere_material = make_shared<lambertian>(albedo);
            world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
            // metal
            auto albedo = random_vec( 0.5, 1.0 );
            auto fuzz = random_double(0, 0.5);
            sphere_material = make_shared<metal>(albedo, fuzz);
            world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else {
            // glass
            sphere_material = make_shared<dielectric>(1.5);
            world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  return world;
}

int main() {
  // image
  const auto aspect_ratio = 3.0 / 2.0;
  const int image_width = 1200;
  const int image_height = static_cast<int>( image_width / aspect_ratio );
  const int samples_per_pixel = 500;
  const int max_depth = 50;
  // world
  hittable_list world = random_scene();

  //camera
  point3 lookfrom(12,2,3);
  point3 lookat(0,0,0);
  vec3 vup(0,1,0);
  auto dist_to_focus = 10.0;
  auto aperture = 0.1;
  camera c( lookfrom, lookat, vup, 20.0, aspect_ratio, aperture, dist_to_focus );

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
