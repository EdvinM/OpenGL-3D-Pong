// Task 3 - Implement Bresenham drawing alg.
//        - Draw a star using lines
//        - Make use of std::vector to define the shape
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

#include <ppgso/ppgso.h>

using namespace std;
using namespace ppgso;

// Size of the framebuffer
const unsigned int SIZE = 512;

const unsigned int STAR_SIZE = 50;

struct Point {
  int x,y;
};

void plotLineLow(ppgso::Image& framebuffer, Point& from, Point& to) {
    int dx = to.x - from.x;
    int dy = to.y - from.y;
    int yi = 1;

    if(dy < 0) {
        yi = -1;
        dy = -dy;
    }

    int D = 2 * dy - dx;
    int y = from.y;

    for(int x = from.x; x <= to.x; x++) {
        framebuffer.setPixel(x, y, 255, 255, 255);

        if(D > 0) {
            y = y + yi;

            D = D - 2 * dx;
        }

        D = D + 2 * dy;
    }
}

void plotLineHigh(ppgso::Image& framebuffer, Point& from, Point& to) {

    int dx = to.x - from.x;
    int dy = to.y - from.y;
    int xi = 1;

    if(dx < 0) {
        xi = -1;
        dx = -dx;
    }

    int D = 2*dx - dy;
    int x = from.x;

    for(int y = from.y; y <= to.y; y++) {
        framebuffer.setPixel(x, y, 255, 255, 255);

        if(D > 0) {
            x = x + xi;

            D = D - 2*dy;
        }

        D = D + 2*dx;
    }
}

// Bresenham drawing algorithm
void drawLine(ppgso::Image& framebuffer, Point& from, Point& to) {
  if(abs(to.y - from.y) < abs(to.x - from.x)) {
      plotLineLow(framebuffer, (from.x > to.x) ? to : from, (from.x > to.x) ? from : to);
  }
  else {
    plotLineHigh(framebuffer, (from.y > to.y) ? to : from, (from.y > to.y) ? from : to);
  }
}

void drawCircle(ppgso::Image& framebuffer, Point& circlePos, int r) {

    double da = (2 * 3.14) / 360;

    for(double a = 0; a < 2 * 3.14; a += da) {
        int x = (int)(circlePos.x + r * cos(a));
        int y = (int)(circlePos.y + r * sin(a));

        framebuffer.setPixel(x, y, 255, 255, 255);
    }

}

int main()
{
  // Use ppgso::Image as our framebuffer
  ppgso::Image framebuffer(SIZE, SIZE);

  // TODO: Generate star points
  vector<Point> points;
//
    Point startingPoint = {SIZE / 2, SIZE / 2};
    drawCircle(framebuffer, startingPoint, 80);

    for(int i = 0; i < 4; i++)
        drawCircle(framebuffer, startingPoint, 80 + i);

    for(int i = 0; i < 10; i++) {

        //Upper lines
        points.push_back({SIZE / 2, SIZE / 2 - 80});
        points.push_back({(int)(SIZE / 2 - i), SIZE / 2});
        points.push_back({SIZE / 2, SIZE / 2 - 80});
        points.push_back({(int)(SIZE / 2 + i), SIZE / 2});


        //Right lines
        points.push_back({(int)(SIZE / 2 + 80 * cos(PI / 6)), (int)(SIZE / 2 + 80 * sin(PI / 6))});
        points.push_back({(int)(SIZE / 2 + i), SIZE / 2});
        points.push_back({(int)(SIZE / 2 + 80 * cos(PI / 6)), (int)(SIZE / 2 + 80 * sin(PI / 6))});
        points.push_back({SIZE / 2, (int)(SIZE / 2 + i)});


        //Left lines
        points.push_back({(int)(SIZE / 2 - 80 * cos(PI / 6)), (int)(SIZE / 2 + 80 * sin(PI / 6))});
        points.push_back({(int)(SIZE / 2 - i), SIZE / 2});
        points.push_back({(int)(SIZE / 2 - 80 * cos(PI / 6)), (int)(SIZE / 2 + 80 * sin(PI / 6))});
        points.push_back({SIZE / 2, (int)(SIZE / 2 + i)});
    }

  for(unsigned int i = 0; i < points.size(); i+= 2)
    drawLine(framebuffer, points[i], points[i+1]);

  // Save the result
  cout << "Generating task3_bresenham.bmp file ..." << endl;
  image::saveBMP(framebuffer, "task3_bresenham.bmp");

  cout << "Done." << endl;
  return EXIT_SUCCESS;
}

