#pragma once
#include "Point3.hh"

class Vector3
{
public:
    Vector3(float x, float y, float z);
    Vector3(const Point3& src, const Point3& dst);

    float x;
    float y;
    float z;

    Vector3 operator*(const float &l) const;
    Vector3 operator+(const Vector3 &v) const;
    Vector3 operator-(const Vector3 &v) const;
    Point3 operator+(const Point3 &p) const;
    Point3 operator-(const Point3 &p) const;
    Vector3 operator+=(const Vector3& v);

    float norm() const;
    Vector3 normalize();
    [[nodiscard]] float scalar_product(const Vector3 &v, bool only_positive = false) const;
    [[nodiscard]] Vector3 vector_product(const Vector3 &v) const;


};
Vector3 operator*(const float &mul, const Vector3 &v);
Point3 operator+(const Point3 &p, const Vector3 &v);
Point3 operator-(const Point3 &p, const Vector3 &v);

