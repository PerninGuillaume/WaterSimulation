#pragma once

#include <ostream>
#include "Constants.hh"

class Point3
{
public:
    Point3();
    Point3(float x, float y, float z);
    Point3 operator+=(const Point3& p);
  Point3 operator+(const Point3& p) const;
  Point3 operator-(const Point3& p) const;


    float x;
    float y;
    float z;
};

std::ostream& operator<<(std::ostream &out, const Point3& point);

