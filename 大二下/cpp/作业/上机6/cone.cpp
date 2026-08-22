#include "cone.h"
#include <cmath>

Cone::Cone(double r, double h) : r(r), h(h) {}

double Cone::volume() const
{
    return (1.0 / 3.0) * PI * r * r * h;
}

double Cone::area() const
{
    double slantHeight = sqrt(r * r + h * h);
    return PI * r * r + PI * r * slantHeight;
}
