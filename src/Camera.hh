#pragma once

#include <vector>
#include "Point3.hh"
#include "Vector3.hh"

class Camera
{
public:
    Camera(Point3 center, Point3 spotted_point, Vector3 up, float alpha, float beta, float zmin);
    std::vector<Point3> pixels_location(int width, int heigth);

    Point3 center;
    Point3 spotted_point;
    Vector3 up; //normalized vector
    float alpha;
    float beta;
    float zmin; //distance from the center to the image plane
    Vector3 center_to_spotted_point;
    Point3 center_image_plane;
    Vector3 side; //normalized vector

    Vector3 unit_x_vector = Vector3(0,0,0);
    Vector3 unit_y_vector = Vector3(0,0,0);
};
