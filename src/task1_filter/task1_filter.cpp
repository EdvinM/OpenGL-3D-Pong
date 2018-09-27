// Task 1 - Load a 512x512 image lena.raw
//        - Apply specified per-pixel transformation to each pixel
//        - Save as result.raw
#include <fstream>
#include <iostream>

using namespace std;

// Size of the framebuffer
const unsigned int SIZE = 512;

// A simple RGB struct will represent a pixel in the framebuffer
struct Pixel {
  // TODO: Define correct data type for r, g, b channel
  unsigned char r, g, b;
};

int main()
{
  // Initialize a framebuffer
  auto framebuffer = new Pixel[SIZE][SIZE];

  // TODO: Open file lena.raw (this is 512x512 RAW GRB format)
  FILE *lena;

  lena = fopen("lena.raw", "rb");
  if(lena == NULL) {
      cout << "Failed to load lena.raw from input" << endl;
      return EXIT_FAILURE;
  }

  // TODO: Read data to framebuffer and close the file
  fread(framebuffer, sizeof(Pixel) * SIZE * SIZE, 1, lena);

  fclose(lena);

  // Traverse the framebuffer
  for (unsigned int y = 0; y < SIZE; y++) {
    for (unsigned int x = 0; x < SIZE; x++) {
      // TODO: Apply pixel operation

      if((framebuffer[y][x].r > 125 && framebuffer[y][x].g > 125) ||
        (framebuffer[y][x].r > 125 && framebuffer[y][x].b > 125) ||
        (framebuffer[y][x].b > 125 && framebuffer[y][x].g > 125)) {

        framebuffer[y][x].r = 60;
        framebuffer[y][x].g = 0;
        framebuffer[y][x].b = 0;
      }
      else
      {
        framebuffer[y][x].r = 255;
        framebuffer[y][x].g = 255;
        framebuffer[y][x].b = 255;
      }
    }
  }

  // TODO: Open file result.raw
  cout << "Generating result.raw file ..." << endl;

  // TODO: Write the framebuffer to the file and close it
  lena = fopen("result.raw", "wb");

  fwrite(framebuffer, sizeof(Pixel) * SIZE * SIZE, 1, lena);

  fclose(lena);

  cout << "Done." << endl;
  delete[] framebuffer;
  return EXIT_SUCCESS;
}
