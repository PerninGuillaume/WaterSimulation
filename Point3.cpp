#include "Point3.hh"

Point3::Point3()
  : x(0)
  , y(0)
  , z(0)
{}

Point3::Point3(float x, float y, float z)
    : x(x),
      y(y),
      z(z)
      {}

Point3 Point3::operator+=(const Point3& p) {
  *this = *this + p;
  return *this;
}

Point3 Point3::operator-(const Point3& p) const {
  return Point3(this->x - p.x, this->y - p.y, this->z - p.z);
}

Point3 Point3::operator+(const Point3& p) const {
  return Point3(this->x + p.x, this->y + p.y, this->z + p.z);
}

std::ostream& operator<<(std::ostream &out, const Point3& point) {
  out << "(" << point.x << ", " << point.y << ", " << point.z << ")";
  return out;
}
