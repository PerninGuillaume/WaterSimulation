#pragma once
#include "Vector3.hh"
#include "Point3.hh"
#include <optional>

struct Caracteristics
{
  Caracteristics();
  Caracteristics(Pixel pixel, double kd, double ks, double ns, std::optional<double> index_refraction = std::optional<double>());
  Pixel pixel;
  double kd; //For diffusion
  double ks; //For specularity
  double ns;
  std::optional<double> index_refraction;
};

class Texture_Material
{
 public:
  explicit Texture_Material(Caracteristics caracteristics);
  virtual Caracteristics caracteristics_point(const Point3& point) = 0;

  Caracteristics caracteristics;
};

class Uniform_Texture : public Texture_Material
{
 public:
  explicit Uniform_Texture(Caracteristics caracteristics);
  Caracteristics caracteristics_point(const Point3& point) override;
};

