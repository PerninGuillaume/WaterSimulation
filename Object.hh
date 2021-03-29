#pragma once

#include <memory>
#include <optional>
#include "Point3.hh"
#include "Rayon.hh"
#include "Vector3.hh"
#include "Texture_Material.hh"

class Object
{
public:
  Object(std::shared_ptr<Texture_Material> texture_material);

    virtual std::optional<float> is_intersecting(const Rayon& ray) = 0;
    virtual Vector3 normal_at_point(const Point3& point, const Rayon& ray) = 0;
    virtual Caracteristics texture_at_point(const Point3& point) = 0;

  std::shared_ptr<Texture_Material> texture_material;
  float epsilon = 0.000001;
};

class Sphere : public Object
{
public:
    Sphere(std::shared_ptr<Texture_Material> texture_material, Point3 origin, float radius);

    std::optional<float> is_intersecting(const Rayon& ray) override;

    Vector3 normal_at_point(const Point3& point, const Rayon& ray) override;

    Caracteristics texture_at_point(const Point3& point) override;

    Point3 origin;
    float radius;
};

class Plane : public Object {
public:
    Plane(std::shared_ptr<Texture_Material> texture_material, Point3 point, Vector3 normal);
    std::optional<float> is_intersecting(const Rayon& ray) override;

    Vector3 normal_at_point(const Point3& point, const Rayon& ray) override;

    Caracteristics texture_at_point(const Point3& point) override;

    Point3 point;
    Vector3 normal;
};

class Triangle : public Object {
public:
    Triangle(std::shared_ptr<Texture_Material> texture_material, Point3 A, Point3 B, Point3 C);

    std::optional<float> is_intersecting(const Rayon& ray) override;

    Vector3 normal_at_point(const Point3& point, const Rayon& ray) override;

    Caracteristics texture_at_point(const Point3& point) override;

    Point3 A;
    Point3 B;
    Point3 C;
    Vector3 AB;
    Vector3 AC;
    Vector3 normal;
};

class SmoothTriangle : public Object {
public:
    SmoothTriangle(std::shared_ptr<Texture_Material> texture_material, Point3 A, Point3 B, Point3 C, Vector3 normA,
                   Vector3 normB, Vector3 normC);

  std::optional<float> is_intersecting(const Rayon& ray) override;

  Vector3 normal_at_point(const Point3& point, const Rayon& ray) override;

  Caracteristics texture_at_point(const Point3& point) override;

  Point3 A;
  Point3 B;
  Point3 C;
  Vector3 normA;
  Vector3 normB;
  Vector3 normC;
  float u;
  float v;
  float w;
};

std::ostream& operator<<(std::ostream& ost, const Triangle& triangle);