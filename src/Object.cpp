#include "Object.hh"
#include <cmath>
#include <utility>

Object::Object(std::shared_ptr<Texture_Material> texture_material)
  : texture_material(std::move(texture_material))
 {}

Sphere::Sphere(std::shared_ptr<Texture_Material> texture_material, Point3 origin, double radius)
    : Object{std::move(texture_material)}
    , origin(origin)
    , radius(radius)
{}

Plane::Plane(std::shared_ptr<Texture_Material> texture_material, Point3 point, Vector3 normal)
    : Object{std::move(texture_material)}
    , point(point)
    , normal(normal.normalize())
{}

  Triangle::Triangle(std::shared_ptr<Texture_Material> texture_material, Point3 A, Point3 B, Point3 C)
    : Object{std::move(texture_material)}
    , A(A)
    , B(B)
    , C(C)
    , AB(Vector3(A, B))
    , AC(Vector3(A, C))
    , normal(AB.vector_product(AC).normalize())
{}

SmoothTriangle::SmoothTriangle(std::shared_ptr<Texture_Material> texture_material, Point3 A, Point3 B, Point3 C,
                               Vector3 normA, Vector3 normB, Vector3 normC, std::optional<Point3> A_text_coord,
                               std::optional<Point3> B_text_coord, std::optional<Point3> C_text_coord)
    : Object{std::move(texture_material)}
    , A(A)
    , B(B)
    , C(C)
    , normA(normA)
    , normB(normB)
    , normC(normC)
    , A_text_coord(std::move(A_text_coord))
    , B_text_coord(std::move(B_text_coord))
    , C_text_coord(std::move(C_text_coord))
{}
//-----------------------------------------------SPHERE------------------------------------------------------------//

std::optional<double> Sphere::is_intersecting(const Rayon& ray)
{
    // Formulas from wikipedia, verified on paper
    double a = std::pow(ray.direction.norm(), 2.0);
    Vector3 sphere_to_point(this->origin, ray.origin);
    double b = 2.0 * (ray.direction.scalar_product(sphere_to_point));
    double c = std::pow(sphere_to_point.norm(), 2.0) - std::pow(this->radius, 2.0);
    double delta = std::pow(b, 2.0) - 4.0 * a * c;
    if (delta < 0.0) {
        return std::optional<double>();
    } else if (delta == 0.0) {
        return std::optional<double>(-b / (2.0 * a));
    }
    double t0 = (-b - std::sqrt(delta)) / (2.0 * a);
    double t1 = (-b + std::sqrt(delta)) / (2.0 * a);
    if (t0 < 0 && t1 < 0) {
      return std::optional<double>();
    } else if (t0 < 0) {
      return t1;
    } else if (t1 < 0) {
      return t0;
    }
    return std::min(t0, t1);
}

Vector3 Sphere::normal_at_point(const Point3& point, const Rayon&)
{
    return Vector3(point.x - origin.x, point.y - origin.y, point.z - origin.z);
}

Caracteristics Sphere::texture_at_point(const Point3&) {
    return texture_material->caracteristics;
}

//-----------------------------------------------PLANE--------------------------------------------------------------//

std::optional<double> Plane::is_intersecting(const Rayon& ray) {
    auto scalar = ray.direction.scalar_product(normal);
    if (scalar == 0.0) {
        return std::optional<double>(); //The line could be inside the plane but I don't take into account this case
    }
    auto tmp = Vector3(ray.origin, this->point).scalar_product(this->normal);
    return tmp / scalar;
}

Vector3 Plane::normal_at_point(const Point3&, const Rayon& ray) {
  if (ray.direction.scalar_product(this->normal) > 0) {
    return -1.0 * this->normal;
  }
  return this->normal;
}

Caracteristics Plane::texture_at_point(const Point3&) {
    return texture_material->caracteristics;
}

//-----------------------------------------------TRIANGLE--------------------------------------------------------------//
std::optional<double> Triangle::is_intersecting(const Rayon &ray) {
  Vector3 D = ray.direction;
  Vector3 P = D.vector_product(AC);
  double determinant = P.scalar_product(AB);
  if (std::fabs(determinant) < epsilon) // ray too much parallel to triangle
    return std::optional<double>();

  double invDeterminant = 1.0 / determinant;
  Vector3 AO(A, ray.origin);
  double u = invDeterminant * AO.scalar_product(P);
  if (u < 0 || u > 1) // outside of triangle
    return std::optional<double>();

  Vector3 Q = AO.vector_product(AB);
  double v = invDeterminant * D.scalar_product(Q);
  //double w = 1.0 - u - v;
  if (v < 0 || u + v > 1) // outside of triangle
    return std::optional<double>();

  double t = invDeterminant * AC.scalar_product(Q);
  return t;
}
/*
//ScratchPixel formulas for edges
std::optional<double> Triangle::is_intersecting(const Rayon& ray) {
  auto ray_dir_dot_product_normal = ray.direction.scalar_product(normal);
  if (std::fabs(ray_dir_dot_product_normal) < epsilon) {
    return std::optional<double>();
  }
  double t = (normal.scalar_product(Vector3(ray.origin, this->A))) / ray_dir_dot_product_normal;

  Point3 P = ray.origin + t * ray.direction;

  Vector3 BC = Vector3(B, C);
  Vector3 CA = Vector3(C, A);

  Vector3 AP(A, P);
  Vector3 BP(B, P);
  Vector3 CP(C, P);
  if (normal.scalar_product(AB.vector_product(AP)) < 0 || normal.scalar_product(BC.vector_product(BP)) < 0
    || normal.scalar_product(CA.vector_product(CP)) < 0) {
    return std::optional<double>();
  }

  return t;
}
 */

Vector3 Triangle::normal_at_point(const Point3&, const Rayon& ray) {
  if (ray.direction.scalar_product(this->normal) > 0) {
    return -1.0 * this->normal;
  }
  return this->normal;
}

Caracteristics Triangle::texture_at_point(const Point3&) {
  return texture_material->caracteristics;
}

std::ostream& operator<<(std::ostream& ost, const Triangle& triangle) {
  //ost << "{" << triangle.A << ", " << triangle.B << ", " << triangle.C << "}";
  return ost;
  //TODO why doesn't it work
}

//-----------------------------------------------SMOOTHTRIANGLE-------------------------------------------------------//

//Moller Trumbore
// P = (D * AC)   Q = (OA * AB)
// determinant = (D * AC) . AB or P . AB
// u = ((D * AC) . AO) / determinant or (P. AO) / determinant
// v = ((AO * AB) . D) / determinant or (Q . D) / determinant
// t = ((AO * AB) . AC) / determinant or (Q. AC) / determinant
std::optional<double> SmoothTriangle::is_intersecting(const Rayon &ray) {
  Vector3 D = ray.direction;
  Vector3 AC = Vector3(A, C);
  Vector3 AB = Vector3(A, B);
  Vector3 P = D.vector_product(AC);
  double determinant = P.scalar_product(AB);
  if (std::fabs(determinant) < epsilon) // ray too much parallel to triangle
    return std::optional<double>();

  double invDeterminant = 1.0 / determinant;
  Vector3 AO(A, ray.origin);
  u = invDeterminant * AO.scalar_product(P);
  if (u < 0 || u > 1) // outside of triangle
    return std::optional<double>();

  Vector3 Q = AO.vector_product(AB);
  v = invDeterminant * D.scalar_product(Q);
  w = 1.0 - u - v;
  if (v < 0 || u + v > 1) // outside of triangle
    return std::optional<double>();

  double t = invDeterminant * AC.scalar_product(Q);
  return t;
}

//This function should be called only after is_intersecting has been called in order for u,v and w to be initialized
Vector3 SmoothTriangle::normal_at_point(const Point3&, const Rayon& ray) {
  Vector3 interpolatedVector = u * normA + v * normB + w * normC;
  if (ray.direction.scalar_product(interpolatedVector) > 0) {
    return -1.0 * interpolatedVector;
  }
  return interpolatedVector;
}

Caracteristics SmoothTriangle::texture_at_point(const Point3& point) {
  if (A_text_coord) {//We have texture coordinates and we compute the interpolated texture coordinate
    Point3 coordinate = A_text_coord.value() * w + B_text_coord.value() * u + C_text_coord.value() * v;
    return texture_material->caracteristics_point(coordinate);
  }
  return texture_material->caracteristics;
}

std::ostream& operator<<(std::ostream& ost, const SmoothTriangle& triangle) {
  //ost << "{" << triangle.A << ", " << triangle.B << ", " << triangle.C << "}";
  //TODO why doesn't it work
  return ost;
}