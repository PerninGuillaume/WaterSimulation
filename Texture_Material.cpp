#include "Texture_Material.hh"

Caracteristics::Caracteristics()
  : pixel(Pixel())
  , kd(0)
  , ks(0)
  , ns(0)
  , index_refraction(std::optional<float>())
{}

Caracteristics::Caracteristics(Pixel pixel, float kd, float ks, float ns, std::optional<float> index_refraction)
  : pixel(pixel)
  , kd(kd)
  , ks(ks)
  , ns(ns)
  , index_refraction(index_refraction)
  {}

Texture_Material::Texture_Material(Caracteristics caracteristics)
    : caracteristics(caracteristics) {}

Uniform_Texture::Uniform_Texture(Caracteristics caracteristics)
    : Texture_Material{caracteristics} {}


Caracteristics Uniform_Texture::caracteristics_point(Point3 point) {
  if (point.x == 0) {
    return caracteristics;
  }
  return caracteristics;
}
