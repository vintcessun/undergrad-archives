//  complex.c
#include <stdio.h>
#include <complex.h>
void show_cmlx(complex double cv);
int main(void)
{
    complex double v1 = 4.0 + 3.0*I;
    double re, im;
    complex double v2;
    complex double sum, conj1;
    
    printf("Enter the real part of a complex number: ");
    scanf("%lf", &re);
    printf("Enter the imaginary part of a complex number: ");
    scanf("%lf", &im);
    v2 = re + im * I; // 修改：原本使用CMPLX(re, im)会在某些编译环境中隐式声明导致错误
//    v2 = CMPLX(re, im);
    show_cmlx(v1);
    putchar('\n');
    show_cmlx(v2);
    putchar('\n');

 //   sum = CMPLX(creal(v1)+creal(v2), (cimag(v1) + cimag(v2)));
    sum = creal(v1)+creal(v2) +(cimag(v1)+ cimag(v2)) * I; // 修改：替换为显式复数表达式，避免隐式声明错误
    conj1 =conj(v1);
    show_cmlx(sum);
    putchar('\n');
    show_cmlx(conj1);
    putchar('\n');
    
    return 0;
    
}

void show_cmlx(complex double cv)
{
    printf("(%f, %fi)", creal(cv), cimag(cv));
    return;
}
