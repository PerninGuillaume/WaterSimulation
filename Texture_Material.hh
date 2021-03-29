#pragma once
#include "Pixel.hh"
#include "Point3.hh"
#include <optional>

struct Caracteristics
{
    Caracteristics();
    Caracteristics(Pixel pixel, float kd, float ks, float ns, std::optional<float> index_refraction = std::optional<float>());
    Pixel pixel;
    float kd;
    float ks;
    float ns;
    std::optional<float> index_refraction;
};

class Texture_Material
{
public:
    Texture_Material(Caracteristics caracteristics);
    virtual Caracteristics caracteristics_point(Point3 point) = 0;

    Caracteristics caracteristics;
};

class Uniform_Texture : public Texture_Material
{
        public:
    Uniform_Texture(Caracteristics caracteristics);
    Caracteristics caracteristics_point(Point3 point) override;
};

