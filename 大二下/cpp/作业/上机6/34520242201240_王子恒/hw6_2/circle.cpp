#include "circle.h"

Circle::Circle(double r) : r(r) {}

double Circle::perimeter() const
{
    return 2.0 * PI * r;
}

double Circle::area() const
{
    return PI * r * r;
}
