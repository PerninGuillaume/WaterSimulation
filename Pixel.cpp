#include "Pixel.hh"
#include <algorithm>

Pixel::Pixel()
    : red(0)
    , green(0)
    , blue(0)
    {}

Pixel::Pixel(color red, color green, color blue)
    : red(red)
    , green(green)
    , blue(blue)
{}

color add(color a, color b) {
  int sum = (int)a + (int)b;
  sum = std::min(255, sum);
  sum = std::max(0, sum);
  return sum;
}

color mul(color a, float coeff) {
  float mul = (float)a * coeff;
  mul = std::min(255.f, mul);
  mul = std::max(0.f, mul);
  return mul;
}
Pixel& Pixel::operator+=(const Pixel& lhs) {
    *this = *this + lhs;
    return *this;
}

Pixel& Pixel::operator+=(float coeff) {
    *this = *this + coeff;
    return *this;
}

Pixel& Pixel::operator*=(float coeff) {
*this = *this * coeff;
return *this;
}

Pixel& Pixel::operator*=(const Pixel& b) {
    *this = *this * b;
    return *this;
}

std::ostream& operator<<(std::ostream &out, const Pixel& pixel) {
    out << "rgb : (" << +pixel.red << ", " << +pixel.green << ", " << +pixel.blue << ")\n";
    return out;
}

Pixel operator+(const Pixel& a, float b) {
    return Pixel(add(a.red, b), add(a.green, b), add(a.blue, b));
}

Pixel operator+(float a, const Pixel& b) {
    return b + a;
}

Pixel operator+(const Pixel& a, const Pixel& b) {
  return Pixel(add(a.red, b.red), add(a.green, b.green), add(a.blue, b.blue));
}

Pixel operator+=(Pixel a, Pixel b) {
    return a + b;
}

Pixel operator*(const Pixel& a, float coeff) {
  return Pixel(mul(a.red, coeff), mul(a.green, coeff), mul(a.blue, coeff));
}

Pixel operator*(float coeff, const Pixel& a) {
    return a * coeff;
}

Pixel operator*(const Pixel& a, const Pixel& b) {
    return Pixel(mul(a.red, b.red), mul(a.green, b.green), mul(a.blue, b.blue));
}

Pixel operator/(const Pixel& a, float div) {
  return a * (1.0/div);
}