#include <iostream>
#include "vec3.h"
#include "color.h"

// compile with g++ -std=c++11 main.cc -o main because for some reason the default g++ on my computer
// uses version 4.2.1 ??? TToTT
int main() {
  // image
  const int image_width = 256;
  const int image_height = 256;

  // render
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for(int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for(int i = 0; i < image_width; i++) {
      vec3 color( double(i) / (image_width-1), double(j) / (image_height-1), 0.25);
      write_color( std::cout, color );
    }
  }
  std::cerr << "\nDone.\n";
}
