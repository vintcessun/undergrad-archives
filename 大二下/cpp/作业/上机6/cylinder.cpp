#include "cylinder.h"

Cylinder::Cylinder(double r, double h) : r(r), h(h) {}

double Cylinder::volume() const
{
    return PI * r * r * h;
}

double Cylinder::area() const
{
    return 2.0 * PI * r * r + 2.0 * PI * r * h;
}
