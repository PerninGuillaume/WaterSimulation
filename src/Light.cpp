#include "Light.hh"
#include <cmath>

Light::Light(Point3 origin, float color_intensity, bool inverse_square_fallof)
: origin(origin)
, colors(Pixel(color_intensity, color_intensity, color_intensity))
, inverse_square_fallof(inverse_square_fallof)
 {}

Light::Light(Point3 origin, Pixel colors, bool inverse_square_fallof)
: origin(origin)
, colors(colors)
, inverse_square_fallof(inverse_square_fallof){}

Point_Light::Point_Light(Point3 origin, float color_intensity)
    : Light{origin, color_intensity} {}

Point_Light::Point_Light(Point3 origin, Pixel colors)
    : Light{origin, colors} {}

Pixel Point_Light::get_intensity(const Point3& pointHit) {
  if (!this->inverse_square_fallof)
    return colors;
  double distance = (origin - pointHit).norm();
  return colors / (4 * M_PI * std::sqrt(distance));



}