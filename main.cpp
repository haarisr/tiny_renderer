#include "tgaimage.h"
#include <cmath>

constexpr TGAColor white = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green = {0, 255, 0, 255};
constexpr TGAColor red = {0, 0, 255, 255};
constexpr TGAColor blue = {255, 128, 64, 255};
constexpr TGAColor yellow = {0, 200, 255, 255};

int main(int argc, char **argv) {
  constexpr int width = 64;
  constexpr int height = 64;
  TGAImage framebuffer(width, height, TGAImage::RGB);

  int ax = 7, ay = 3;
  int bx = 12, by = 37;
  int cx = 62, cy = 53;

  for (float t = 0; t < 1; t += 0.02f) {
    int x = std::round(ax * (1 - t) + bx * t);
    int y = std::round(ay * (1 - t) + by * t);
    framebuffer.set(x, y, blue);

    x = std::round(cx * (1 - t) + bx * t);
    y = std::round(cy * (1 - t) + by * t);
    framebuffer.set(x, y, green);

    x = std::round(cx * (1 - t) + ax * t);
    y = std::round(cy * (1 - t) + ay * t);
    framebuffer.set(x, y, yellow);

    x = std::round(ax * (1 - t) + cx * t);
    y = std::round(ay * (1 - t) + cy * t);
    framebuffer.set(x, y, red);
  }

  framebuffer.set(ax, ay, white);
  framebuffer.set(bx, by, white);
  framebuffer.set(cx, cy, white);

  framebuffer.write_tga_file("framebuffer.tga");
  return 0;
}
