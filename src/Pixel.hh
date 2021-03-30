#pragma once

#include "Constants.hh"
#include "Point3.hh"

class Pixel
{
public:
    Pixel();
    Pixel(color red, color green, color blue);
    Pixel& operator+=(const Pixel& lhs);
    Pixel& operator+=(float coeff);
    Pixel& operator*=(float coeff);
    Pixel& operator*=(const Pixel& b);

    color red;
    color green;
    color blue;
};

std::ostream& operator<<(std::ostream &out, const Pixel& pixel);
Pixel operator+(float a, const Pixel& b);
Pixel operator+(const Pixel& a, float b);
Pixel operator+(const Pixel& a, const Pixel& b);
Pixel operator*(const Pixel& a, float coeff);
Pixel operator*(float coeff, const Pixel& a);
Pixel operator*(const Pixel& a, const Pixel& b);
Pixel operator/(const Pixel& a, float div);
