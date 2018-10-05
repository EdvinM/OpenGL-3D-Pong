// Task 2 - Implement a convolution filter
//        - Define a convolution filter
//        - Handle image boundary correctly as specified in assignment
//        - Correctly handle color manipulation

#include <ppgso/ppgso.h>

using namespace std;
using namespace ppgso;

const int kernel_height = 3;
const int kernel_width = 3;
float kernel[kernel_height][kernel_width] {
    // TODO: Define the kernel
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0}
};
float kernel_factor = 1.0f;
float kernel_bias = 0.0f;

// Clamp value to the <0, 1> range
inline float clamp(float value) {
  if(value > 1.0f) return 1.0f;
  if(value < 0) return 0;
  return value;
}

int main()
{
  // We will use ppgso::Image for the source and destination framebuffer this time
  ppgso::Image source = image::loadBMP("lena.bmp");
  ppgso::Image destination(source.width, source.height);

  // Traverse the source image, use Image::getPixel to get source samples
  for(int y = 0; y < source.height; ++y) {
    for(int x = 0; x < source.width; ++x) {
      // Store sum of colors
      float r = 0, g = 0, b = 0;

      // TODO: Compute convolution
      for(int k_y = 0; k_y < kernel_height; k_y++) {

          if(y == 0)
              k_y += 1;

          int cur_pixel_y = (y - (kernel_height / 2) + k_y) % source.height;

        for(int k_x = 0; k_x < kernel_width; k_x++) {

            if(x == 0)
                k_x += 1;

            int cur_pixel_x = (x - (kernel_width / 2) + k_x) % source.width;

            r = (r + source.getPixel(cur_pixel_x, cur_pixel_y).r * kernel[k_y][k_x]);
            g = (g + source.getPixel(cur_pixel_x, cur_pixel_y).g * kernel[k_y][k_x]);
            b = (b + source.getPixel(cur_pixel_x, cur_pixel_y).b * kernel[k_y][k_x]);
        }
      }

      // Store the result divided by factor
      destination.setPixel(x, y, r / 256, g / 256, b / 256);
    }
  }

  // Save as result
  cout << "Generating task2_convolution.bmp file ..." << endl;
  image::saveBMP(destination, "task3_convolution.bmp");

  cout << "Done." << endl;
  return EXIT_SUCCESS;
}
