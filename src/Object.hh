#pragma once

#include <memory>
#include <optional>
#include "Rayon.hh"
#include "Vector3.hh"
#include "Texture_Material.hh"

class Object
{
public:
  Object(std::shared_ptr<Texture_Material> texture_material);

    virtual std::optional<double> is_intersecting(const Rayon& ray, double &u, double &v) = 0;
    virtual Vector3 normal_at_point(const Point3& point, double u, double v, bool interpolate) = 0;
    virtual Caracteristics texture_at_point(const Point3& point, double u, double v) = 0;

  std::shared_ptr<Texture_Material> texture_material;
  double epsilon = 0.000001;
};

class Sphere : public Object
{
public:
    Sphere(std::shared_ptr<Texture_Material> texture_material, Point3 origin, double radius);

    std::optional<double> is_intersecting(const Rayon& ray, double &u, double &v) override;

    Vector3 normal_at_point(const Point3& point, double u, double v, bool interpolate) override;

    Caracteristics texture_at_point(const Point3& point, double u, double v) override;

    Point3 origin;
    double radius;
};

class Plane : public Object {
public:
    Plane(std::shared_ptr<Texture_Material> texture_material, Point3 point, Vector3 normal);
  std::optional<double> is_intersecting(const Rayon& ray, double &u, double &v) override;

  Vector3 normal_at_point(const Point3& point, double u, double v, bool interpolate) override;

  Caracteristics texture_at_point(const Point3& point, double u, double v) override;

  Point3 point;
    Vector3 normal;
};

class Triangle : public Object {
public:
    Triangle(std::shared_ptr<Texture_Material> texture_material, Point3 A, Point3 B, Point3 C);

  std::optional<double> is_intersecting(const Rayon& ray, double &u, double &v) override;

  Vector3 normal_at_point(const Point3& point, double u, double v, bool interpolate) override;

  Caracteristics texture_at_point(const Point3& point, double u, double v) override;

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
                   Vector3 normB, Vector3 normC, std::optional<Point3> A_text_coord = {}, std::optional<Point3> B_text_coord = {},
                   std::optional<Point3> C_text_coord = {});

  std::optional<double> is_intersecting(const Rayon& ray, double &u, double &v) override;

  Vector3 normal_at_point(const Point3& point, double u, double v, bool interpolate) override;

  Caracteristics texture_at_point(const Point3& point, double u, double v) override;

  Point3 A;
  Point3 B;
  Point3 C;
  Vector3 normA;
  Vector3 normB;
  Vector3 normC;
  std::optional<Point3> A_text_coord; //TODO Actually it is a Point2
  std::optional<Point3> B_text_coord;
  std::optional<Point3> C_text_coord;
  Vector3 normal;
};

std::ostream& operator<<(std::ostream& ost, const Triangle& triangle);