#pragma once
#include "Vector3.hh"
#include "Image.hh"
#include <optional>

struct Caracteristics {
  Caracteristics();
  Caracteristics(Pixel pixel, double kd, double ks, double ns, std::optional<double> index_refraction = std::optional<double>());
  Pixel pixel;
  double kd; //For diffusion
  double ks; //For specularity
  double ns;
  std::optional<double> index_refraction;
};

class Texture_Material {
 public:
  explicit Texture_Material(Caracteristics caracteristics);
  virtual Caracteristics caracteristics_point(const Point3& point) = 0;

  Caracteristics caracteristics;
};

class Uniform_Texture : public Texture_Material {
 public:
  explicit Uniform_Texture(Caracteristics caracteristics);
  Caracteristics caracteristics_point(const Point3& point) override;
};

class Procedural_Texture : public Texture_Material {
 public:
  explicit Procedural_Texture(Caracteristics caracteristics);
  Caracteristics caracteristics_point(const Point3& point) override;
};

class Image_Texture : public Texture_Material {
 public:
  explicit Image_Texture(Caracteristics caracteristics, const std::string& filename);
  //I think the point ought to have 2 dimension and be the u and v coordinates
  //u and v between 0 and 1
  Caracteristics caracteristics_point(const Point3& point) override;

  Image image;
};

