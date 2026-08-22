#include "polygon.h"

#include <cmath>

Polygon::Polygon(double n, double s) : n(n), s(s) {}

double Polygon::perimeter() const
{
    return n * s;
}

double Polygon::area() const
{
    return (n * s * s) / (4.0 * std::tan(PI / n));
}
