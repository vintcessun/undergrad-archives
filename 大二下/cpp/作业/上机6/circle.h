#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"

class Circle : public Shape2D
{
private:
    double r;
public:
    Circle(double r);
    double perimeter() const override;
    double area() const override;
};

#endif
