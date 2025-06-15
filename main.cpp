#include "tgaimage.h"
#include <cmath>

constexpr TGAColor white = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green = {0, 255, 0, 255};
constexpr TGAColor red = {0, 0, 255, 255};
constexpr TGAColor blue = {255, 128, 64, 255};
constexpr TGAColor yellow = {0, 200, 255, 255};

void line(int ax, int ay, int bx, int by, TGAImage &image,
          const TGAColor &color) {

  bool steep = std::abs(by - ay) > std::abs(bx - ax);
  if (steep) {
    std::swap(ax, ay);
    std::swap(bx, by);
  }

  if (ax > bx) {
    std::swap(ax, bx);
    std::swap(ay, by);
  }

  for (int start = ax; start <= bx; ++start) {
    float t = (start - ax) / static_cast<float>(bx - ax);
    int x = std::round(ax + (bx - ax) * t);
    int y = std::round(ay + (by - ay) * t);
    if (steep) {
      image.set(y, x, color);
    } else {
      image.set(x, y, color);
    }
  }
}

int main(int argc, char **argv) {
  constexpr int width = 64;
  constexpr int height = 64;
  TGAImage framebuffer(width, height, TGAImage::RGB);

  int ax = 7, ay = 3;
  int bx = 12, by = 37;
  int cx = 62, cy = 53;

  line(ax, ay, bx, by, framebuffer, blue);
  line(cx, cy, bx, by, framebuffer, green);
  line(cx, cy, ax, ay, framebuffer, yellow);
  line(ax, ay, cx, cy, framebuffer, red);

  framebuffer.set(ax, ay, white);
  framebuffer.set(bx, by, white);
  framebuffer.set(cx, cy, white);

  framebuffer.write_tga_file("framebuffer.tga");
  return 0;
}
