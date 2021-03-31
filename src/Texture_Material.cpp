#include <utility>
#include <iostream>

#include "Texture_Material.hh"
#include "Image.hh"

Caracteristics::Caracteristics()
  : pixel(Vector3())
  , kd(0)
  , ks(0)
  , ns(0)
  , index_refraction(std::optional<double>())
{}

Caracteristics::Caracteristics(Pixel pixel, double kd, double ks, double ns, std::optional<double> index_refraction)
  : pixel(pixel)
  , kd(kd)
  , ks(ks)
  , ns(ns)
  , index_refraction(std::move(index_refraction))
  {}

Texture_Material::Texture_Material(Caracteristics caracteristics)
    : caracteristics(std::move(caracteristics)) {}

Uniform_Texture::Uniform_Texture(Caracteristics caracteristics)
    : Texture_Material{std::move(caracteristics)} {}

Procedural_Texture::Procedural_Texture(Caracteristics caracteristics)
    : Texture_Material(std::move(caracteristics)) {}

Image_Texture::Image_Texture(Caracteristics caracteristics, const std::string& filename)
    : Texture_Material(std::move(caracteristics))
    , image(Image(filename)) {}

Caracteristics Uniform_Texture::caracteristics_point(const Point3&) {
  return caracteristics;
}

Caracteristics Procedural_Texture::caracteristics_point(const Point3 &point) {
  return caracteristics;
}

Caracteristics Image_Texture::caracteristics_point(const Point3 &point) {
  int x = point.x * image.width;
  int y = point.y * image.height;
  Pixel value = image.pixels[x + y * image.width];
  Caracteristics res = caracteristics;
  res.pixel = value;
  return res;
}