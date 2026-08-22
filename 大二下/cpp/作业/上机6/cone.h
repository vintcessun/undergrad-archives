#ifndef CONE_H
#define CONE_H

#include "shape.h"

class Cone : public Shape3D
{
private:
    double r, h;
public:
    Cone(double r, double h);
    double volume() const override;
    double area() const override;
};

#endif
