#include "Image.hh"
#include <iostream>
#include <fstream>
#include <cmath>

Image::Image(int width, int height)
: width(width)
, height(height) {}

  Image::Image(const std::string& input_filename) {
  std::ifstream file;
  file.open(input_filename);
  if (!file.is_open()) {
    throw std::invalid_argument("Could not open file " + input_filename);
  }

  std::string tmp;
  file >> tmp;
  if (tmp != "P3" && tmp != "P6") {
    std::cout << tmp << '\n';
    throw std::invalid_argument("Not P3 or P6 it is " + tmp);
  }
  file >> width;
  file >> height;
  file >> max_color_value;
  std::cout << "Width : " << width << " Height : " << height << " Max color value : " << max_color_value << '\n';
  file.ignore(256, '\n');
  for (int i = 0; i < width * height; ++i) {
    unsigned char buffer[3];
    file.read(reinterpret_cast<char *>(buffer), 3);
    pixels.emplace_back(buffer[0], buffer[1], buffer[2]);
  }
  file.close();
}

unsigned char Image::compress_value(double value) {
  return (unsigned char)(std::max(0.0, std::min(255.0, std::pow(value, 1/gamma))));
}

void Image::save_as_ppm(const std::string& filename) {
    std::ofstream file;
    file.open(filename);
    file << "P6\n";
    file << width << " " << height << '\n';
    file << max_color_value << '\n';
    for (const auto& pixel : pixels) {
      unsigned char r = compress_value(pixel.x);
      unsigned char g = compress_value(pixel.y);
      unsigned char b = compress_value(pixel.z);
      file << r << g << b;
    }
    file.close();
}




