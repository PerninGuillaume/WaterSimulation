#pragma once
#include "Point3.hh"
#include "Vector3.hh"

class Light
{
public:
    Light(Point3 origin, float color_intensity);
    Light(Point3 origin, Pixel colors);

    Point3 origin;
    Pixel colors;

};

class Point_Light : public Light
{
public:
    Point_Light(Point3 origin, float color_intensity);
    Point_Light(Point3 origin, Pixel colors);
};

