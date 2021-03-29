#include "Vector3.hh"
#include <cmath>

Vector3::Vector3(float x, float y, float z)
    : x(x),
    y(y),
    z(z) 
{}

Vector3::Vector3(const Point3& src, const Point3& dst)
    : Vector3{dst.x - src.x, dst.y - src.y, dst.z - src.z}
{}

Vector3 Vector3::operator*(const float &mul) const
{
    return Vector3(x * mul, y * mul, z * mul);
}

Vector3 operator*(const float &mul, const Vector3 &v) {
  return v * mul;
}

Vector3 Vector3::operator+(const Vector3 &v) const
{
    return Vector3(this->x + v.x, this->y + v.y, this->z + v.z);
}

Vector3 Vector3::operator-(const Vector3 &v) const
{
    return Vector3(this->x - v.x, this->y - v.y, this->z - v.z);
}

Point3 Vector3::operator+(const Point3 &p) const {
    return Point3(this->x + p.x, this->y + p.y, this->z + p.z);
}

Point3 operator+(const Point3 &p, const Vector3 &v) {
  return v + p;
}

Point3 Vector3::operator-(const Point3 &p) const {
  return Point3(this->x - p.x, this->y - p.y, this->z - p.z);
}

Point3 operator-(const Point3 &p, const Vector3 &v) {
  return Point3(p.x - v.x, p.y - v.y, p.z - v.z);
}

Vector3 Vector3::operator+=(const Vector3& v) {
  *this = *this + v;
  return *this;
}

float Vector3::norm() const {
    return std::sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::normalize()
{
  float norm = this->norm();
  x /= norm;
  y /= norm;
  z /= norm;
  return *this;
}

float Vector3::scalar_product(const Vector3 &v, bool only_positive) const
{
    float tmp = this->x * v.x + this->y * v.y + this->z * v.z;
    if (only_positive && tmp < 0) {
        return 0.;
    }
    else return tmp;
}

Vector3 Vector3::vector_product(const Vector3 &v) const {
    return Vector3(this->y * v.z - v.y * this->z, v.x * this->z - this->x * v.z, this->x * v.y - v.x * this->y);
}