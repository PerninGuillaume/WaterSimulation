#include "Rayon.hh"

Rayon::Rayon(Vector3 direction, Point3 origin)
 : direction(direction.normalize())
 , origin(origin)
{}


