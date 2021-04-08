#pragma once

#include <vector>
#include "Object.hh"
#include "Image.hh"
#include "Camera.hh"
#include "Light.hh"

struct PointIntersection
{
    PointIntersection();
    PointIntersection(bool is_intersecting,  std::shared_ptr<Object> intersecting_object, Point3 intersection_point,
                      Caracteristics caracteristics);

    bool is_intersecting;
    std::shared_ptr<Object> intersecting_object;
    Point3 intersection_point;
    Caracteristics caracteristics;
};

class Scene
{
public:
    Scene(Camera camera, unsigned int max_bounces);
    
    Scene& add_object(const std::shared_ptr<Object>& object);
    void add_object(const std::vector<std::shared_ptr<Object>>& objects_to_add);
    Scene& add_light(const std::shared_ptr<Light>& light);

    bool is_hidden(const Rayon& ray, double point_to_light_norm);

    Pixel diffuse_light(const Point3& intersection_point, const Caracteristics& caracteristics,
                        const std::shared_ptr<Object>& object);

    Pixel specular_light(const Point3& intersection_point, const Vector3& incident_vector,
                         const Caracteristics& caracteristics, const std::shared_ptr<Object>& object);

    //This function returns the ratio of energy that is reflected, between 0 and 1
    static double fresnel(const Vector3& incident, const Vector3& normal, double index_refraction);

    PointIntersection find_intersection(Rayon ray);

    Image raycasting();

    Pixel raycast(const Rayon& ray, unsigned int bounces);

    std::vector<std::shared_ptr<Object>> objects = {};
    std::vector<std::shared_ptr<Light>> lights = {};
    Camera camera;
    unsigned int max_bounces;
    double epsilon = 0.0001; //We discard intersecting object with a t inferior to epsilon
    bool diffusion = true;
    bool specularity = true;
    bool reflection = true;
    bool shadow = true;
    bool refraction = true;
    int msaa_samples = 1;
    int width = 800;
    int height = 450;

};

