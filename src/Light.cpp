#include "Light.hh"

Light::Light(Point3 origin, float color_intensity)
: origin(origin)
, colors(Pixel(color_intensity, color_intensity, color_intensity))
 {}

Light::Light(Point3 origin, Pixel colors)
: origin(origin)
, colors(colors) {}

Point_Light::Point_Light(Point3 origin, float color_intensity)
    : Light{origin, color_intensity} {}

Point_Light::Point_Light(Point3 origin, Pixel colors)
    : Light{origin, colors} {}
