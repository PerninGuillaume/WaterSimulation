#pragma once
#include "Vector3.hh"
#include <string>
#include <vector>

class Image
{
 public:
  Image(int width, int height);
  Image(const std::string& input_filename);

  unsigned char compress_value(double max, double value) const;
  void save_as_ppm(const std::string& filename);

  int width;
  int height;
  std::vector<Pixel> pixels;
  int max_color_value = 255;
  double gamma = 2.2;
};
