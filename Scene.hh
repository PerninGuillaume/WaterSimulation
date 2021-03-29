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
    
    Scene add_object(std::shared_ptr<Object> object);
    Scene add_object(std::initializer_list<std::shared_ptr<Object>> objects_to_add);
    Scene add_light(std::shared_ptr<Light> light);

    bool is_hidden(const Rayon& ray, float point_to_light_norm);

    Pixel diffuse_light(const Point3& intersection_point, const Vector3& normal, const Caracteristics& caracteristics);

    Pixel specular_light(const Point3& intersection_point, const Vector3& reflected_vector, const Caracteristics& caracteristics);

    std::optional<Vector3> find_refraction(const Vector3& incident, Vector3 normal, float index_refraction);

    //This function returns the ratio of energy that is reflected, between 0 and 1
    float fresnel(const Vector3& incident, const Vector3& normal, float index_refraction);

    PointIntersection find_intersection(Rayon ray);

    Image raycasting();

    Pixel raycast(const Rayon& ray, unsigned int bounces);

    void set_epsilon(float epsilon);

    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;
    Camera camera;
    unsigned int max_bounces;
    float epsilon = 0.01; //We discard intersecting object with a t inferior to epsilon
    bool diffusion = true;
    bool specularity = true;
    bool reflection = true;
    bool shadow = true;
    bool refraction = false;
    int msaa_samples = 1;

};

