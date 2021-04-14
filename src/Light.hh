#pragma once
#include "Vector3.hh"

class Light
{
 public:
  Light(Point3 origin, float color_intensity, bool inverse_square_fallof = true);
  Light(Point3 origin, Pixel colors, bool inverse_square_fallof = true);

  virtual Pixel get_intensity(const Point3& pointHit) = 0;
  Point3 origin;

 protected:
  //Determine if an object faraway get the same light intensity as a one closer
  Pixel colors;
  bool inverse_square_fallof;
};

class Point_Light : public Light
{
 public:
  Point_Light(Point3 origin, float color_intensity);
  Point_Light(Point3 origin, Pixel colors);
  Pixel get_intensity(const Point3& pointHit) override;
};

