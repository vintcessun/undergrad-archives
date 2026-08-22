#ifndef SHAPE_H
#define SHAPE_H

class Shape
{
public:
    virtual double getArea() const = 0;
    virtual ~Shape() {}
};

class Rectangle : public Shape
{
private:
    double w, h;

public:
    Rectangle(double w = 0, double h = 0) : w(w), h(h) {}

    double getArea() const override
    {
        return h * w;
    }
};

class Circle : public Shape
{
private:
    double r;

public:
    Circle(double r = 0) : r(r) {}

    double getArea() const override
    {
        return 3.14 * r * r;
    }
};

#endif
