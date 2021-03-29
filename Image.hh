#pragma once
#include "Pixel.hh"
#include <string>
#include <vector>

class Image
{
public:
    Image(int width, int height);
    Image(std::string input_filename);

    void save_as_ppm(std::string filename);
    int width;
    int height;
    std::vector<Pixel> pixels;
    int max_color_value = 255;
};
