#include "Vector3.hh"
#include <cmath>
#include <optional>
#include <iostream>

Vector3::Vector3()
    : x(0)
    , y(0)
    , z(0)
{}

Vector3::Vector3(double x, double y, double z)
    : x(x)
    , y(y)
    , z(z)
{}

Vector3::Vector3(const Point3& src, const Point3& dst)
    : Vector3{dst.x - src.x, dst.y - src.y, dst.z - src.z}
{}

Vector3 Vector3::operator*(const double &mul) const {
    return Vector3(x * mul, y * mul, z * mul);
}

Vector3 Vector3::operator*(const Vector3 &v) const {
  return Vector3(this->x * v.x, this->y * v.y, this->z * v.z);
}

Vector3& Vector3::operator*=(const double t) {
  *this = *this * t;
  return *this;
}

Vector3& Vector3::operator*=(const Vector3 &v) {
  *this = *this * v;
  return *this;
}

Vector3 operator*(const double &mul, const Vector3 &v) {
  return v * mul;
}

Vector3 Vector3::operator+(const Vector3 &v) const {
    return Vector3(this->x + v.x, this->y + v.y, this->z + v.z);
}

Vector3 Vector3::operator-(const Vector3 &v) const {
    return Vector3(this->x - v.x, this->y - v.y, this->z - v.z);
}

Vector3 Vector3::operator-() const {
  return Vector3(-this->x, -this->y, -this->z);
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

Vector3& Vector3::operator+=(const Vector3& v) {
  *this = *this + v;
  return *this;
}

Vector3 Vector3::operator/(double t) {
  return (1/t) * *this;
}

Vector3& Vector3::operator/=(const double t) {
  return *this *= 1/t;
}

double Vector3::norm() const {
    return std::sqrt(x * x + y * y + z * z);
}

Vector3& Vector3::normalize() {
  double norm = this->norm();
  x /= norm;
  y /= norm;
  z /= norm;
  return *this;
}

double Vector3::scalar_product(const Vector3 &v, bool only_positive) const {
    double tmp = this->x * v.x + this->y * v.y + this->z * v.z;
    if (only_positive && tmp < 0) {
        return 0.;
    }
    else return tmp;
}

Vector3 Vector3::vector_product(const Vector3 &v) const {
    return Vector3(this->y * v.z - v.y * this->z, v.x * this->z - this->x * v.z, this->x * v.y - v.x * this->y);
}

std::optional<Vector3> refraction_vector(const Vector3& incident, Vector3 normal, double index_refraction) {
  double ratio_refraction;
  double cosinus_incident_angle = normal.scalar_product(incident);
  if (cosinus_incident_angle > 1 || cosinus_incident_angle < -1) {
    std::cout << cosinus_incident_angle << '\n';
  }
  if (cosinus_incident_angle < 0) { //Air to Water, outside the surface
    cosinus_incident_angle = -1.0 * cosinus_incident_angle;
    ratio_refraction = 1.0 / index_refraction;
  }
  else { //Water to Air, inside the surface
    ratio_refraction = index_refraction / 1.0;
    normal = -1.0 * normal;
  }
  double k = 1.0 - std::pow(ratio_refraction, 2) * (1.0 - std::pow(cosinus_incident_angle, 2));
  if (k < 0) { //Total internal reflection, no refraction (aquarium)
    return std::optional<Vector3>();
  }
  return ratio_refraction * incident + (ratio_refraction * cosinus_incident_angle - std::sqrt(k)) * normal;
}

Vector3 reflection_vector(const Vector3& incident, const Vector3& normal) {
  return (incident - 2 * (incident.scalar_product(normal)) * normal).normalize();
}

std::ostream& operator<<(std::ostream &out, const Vector3& v) {
  return out << "[" << v.x << ", " << v.y << ", " << v.z << "]";
}
