#pragma once
#include <optional>

class Vector3;

using Point3 = Vector3;
class Vector3
{
 public:
  Vector3();
  Vector3(double x, double y, double z);
  Vector3(const Point3& src, const Point3& dst);

  double x;
  double y;
  double z;

  Vector3 operator*(const double &mul) const;
  Vector3 operator*(const Vector3 &v) const;
  Vector3& operator*=(double t);
  Vector3& operator*=(const Vector3 &v);

  Vector3 operator+(const Vector3 &v) const;
  Vector3 operator+(double t) const;
  Vector3& operator+=(const Vector3& v);

  Vector3 operator-(const Vector3 &v) const;
  Vector3 operator-() const;

  Vector3 operator/(double t);
  Vector3& operator/=(double t);
  bool operator==(const Vector3& v);

  [[nodiscard]] double norm() const;
  Vector3& normalize();
  [[nodiscard]] double scalar_product(const Vector3 &v, bool only_positive = false) const;
  [[nodiscard]] Vector3 vector_product(const Vector3 &v) const;

};

using Pixel = Vector3;

std::ostream& operator<<(std::ostream &out, const Vector3& v);
Vector3 operator*(const double &mul, const Vector3 &v);

std::optional<Vector3> refraction_vector(const Vector3& incident, Vector3 normal, double index_refraction);
Vector3 reflection_vector(const Vector3& incident, const Vector3& normal);
