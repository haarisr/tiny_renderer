#include "tgaimage.h"
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

  int y = ay;
  int ierror = 0;
  for (int x = ax; x <= bx; ++x) {
    if (steep) {
      image.set(y, x, color);
    } else {
      image.set(x, y, color);
    }
    ierror += 2 * std::abs(by - ay);
    int correction = (ierror > (bx - ax));
    y += ((by > ay) ? 1 : -1) * correction;
    ierror -= 2 * (bx - ax) * correction;
  }
}

struct Vertex {
  float x, y, z;
};

struct Obj {
  std::vector<Vertex> vertices;
  std::vector<int> faces;
};

Obj parseObjFile(const std::filesystem::path &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file: " + filename.string());
  }
  std::string line;
  Obj obj;
  auto &vertices = obj.vertices;
  auto &faces = obj.faces;
  while (std::getline(file, line)) {
    std::istringstream iss(line);

    std::string prefix;
    prefix.reserve(2);

    iss >> prefix;
    if (prefix == "v") {
      Vertex v;
      iss >> v.x >> v.y >> v.z;
      vertices.push_back(v);
    } else if (prefix == "f") {
      for (int i = 0; i < 3; i++) {
        int vertex_index;
        int texture_index;
        int normal_index;
        char slash;
        iss >> vertex_index >> slash >> texture_index >> slash >> normal_index;
        faces.push_back(--vertex_index); // OBJ indices are 1-based
      }
    }
  }
  return obj;
}

int main(int argc, char **argv) {
  constexpr int width = 800;
  constexpr int height = 800;
  TGAImage framebuffer(width, height, TGAImage::RGB);

  auto obj = parseObjFile("./obj/diablo3_pose/diablo3_pose.obj");

  for (size_t i = 0; i < obj.faces.size(); i += 3) {
    auto point1 = obj.vertices[obj.faces[i]];
    auto point2 = obj.vertices[obj.faces[i + 1]];
    auto point3 = obj.vertices[obj.faces[i + 2]];
    line((point1.x + 1) * width / 2.0f, (point1.y + 1) * height / 2.0f,
         (point2.x + 1) * width / 2.0f, (point2.y + 1) * height / 2.0f,
         framebuffer, red);
    line((point2.x + 1) * width / 2.0f, (point2.y + 1) * height / 2.0f,
         (point3.x + 1) * width / 2.0f, (point3.y + 1) * height / 2.0f,
         framebuffer, red);
    line((point3.x + 1) * width / 2.0f, (point3.y + 1) * height / 2.0f,
         (point1.x + 1) * width / 2.0f, (point1.y + 1) * height / 2.0f,
         framebuffer, red);
    framebuffer.set((point1.x + 1) * width / 2.0f,
                    (point1.y + 1) * height / 2.0f, white);
    framebuffer.set((point2.x + 1) * width / 2.0f,
                    (point2.y + 1) * height / 2.0f, white);
    framebuffer.set((point3.x + 1) * width / 2.0f,
                    (point3.y + 1) * height / 2.0f, white);
  }

  framebuffer.write_tga_file("framebuffer.tga");
  return 0;
}
