#include "Complex.h"
#include <iomanip>

Complex::Complex(double Re, double Im) : Re(Re), Im(Im) {}

Complex::Complex(double d) : Re(d), Im(0) {}

Complex Complex::operator~() const {
    return Complex(Re, -Im);
}

Complex Complex::operator-() const {
    return Complex(-Re, -Im);
}

ostream & operator<<(ostream & os, const Complex & c) {
    const double eps = 1e-5;
    os << std::fixed << std::setprecision(2);

    if (std::fabs(c.Re) < eps && std::fabs(c.Im) < eps) {
        os << 0.0;
    } else if (std::fabs(c.Re) < eps) {
        os << c.Im << "i";
    } else if (std::fabs(c.Im) < eps) {
        os << c.Re;
    } else if (c.Im < 0) {
        os << c.Re << " - " << std::fabs(c.Im) << "i";
    } else {
        os << c.Re << " + " << c.Im << "i";
    }

    return os;
}

Complex operator+(Complex c1, Complex c2) {
    return Complex(c1.Re + c2.Re, c1.Im + c2.Im);
}

Complex operator-(Complex c1, Complex c2) {
    return Complex(c1.Re - c2.Re, c1.Im - c2.Im);
}

Complex operator*(Complex c1, Complex c2) {
    return Complex(c1.Re * c2.Re - c1.Im * c2.Im,
                   c1.Re * c2.Im + c1.Im * c2.Re);
}

Complex operator/(Complex c1, Complex c2) {
    double denominator = c2.Re * c2.Re + c2.Im * c2.Im;
    return Complex((c1.Re * c2.Re + c1.Im * c2.Im) / denominator,
                   (c1.Im * c2.Re - c1.Re * c2.Im) / denominator);
}

double abs(Complex & c) {
    return sqrt(c.Re * c.Re + c.Im * c.Im);
}

Complex operator""i(unsigned long long Im) {
    return Complex(0, static_cast<double>(Im));
}

Complex operator""i(long double Im) {
    return Complex(0, static_cast<double>(Im));
}
