#pragma once

#include "Vector3.hh"

class Rayon
{
public:
    Rayon(Vector3 direction, Point3 origin);

    Vector3 direction;
    Point3 origin;
};
