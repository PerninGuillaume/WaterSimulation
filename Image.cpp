#include "Image.hh"
#include <iostream>
#include <fstream>

Image::Image(int width, int height)
: width(width)
, height(height) {}

  Image::Image(std::string input_filename) {
  std::ifstream file;
  file.open(input_filename);
  if (!file.is_open()) {
    throw std::invalid_argument("Could not open file " + input_filename);
  }

  std::string tmp;
  file >> tmp;
  if (tmp != "P3") {
    std::cout << tmp << '\n';
    throw std::invalid_argument("Not P3 it is " + tmp);
  }
  file >> width;
  file >> height;
  file >> max_color_value;
  std::cout << "Width : " << width << " Height : " << height << " Max color value : " << max_color_value << '\n';
  int color_value_red;
  int color_value_green;
  int color_value_blue;
  while (file >> color_value_red && file >> color_value_green && file >> color_value_blue) {
    pixels.push_back(Pixel(color_value_red, color_value_green, color_value_blue));
  }
}

void Image::save_as_ppm(std::string filename) {
    std::ofstream file;
    file.open(filename);
    file << "P3\n";
    file << width << " " << height << '\n';
    file << max_color_value << '\n';
    for (const auto& pixel : pixels) {
        file << (int)pixel.red << ' ' << (int)pixel.green << ' ' << (int)pixel.blue << '\n';
    }
    file.close();
}




