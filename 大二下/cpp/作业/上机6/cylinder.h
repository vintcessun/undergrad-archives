#ifndef CYLINDER_H
#define CYLINDER_H

#include "shape.h"

class Cylinder : public Shape3D
{
private:
    double r, h;
public:
    Cylinder(double r, double h);
    double volume() const override;
    double area() const override;
};

#endif
