#include <stdio.h>
#include <stdint.h>
uint8_t x=134,y=246,z1,z2;
int8_t m,n,k1,k2;
int main(){
    m=x,n=y;
    z1=x-y,z2=x+y;
    k1=m-n,k2=m+n;
    printf("R1(x)=%02X, R5(z1)=%02X, R6(z2)=%02X\n",x,z1,z2);
    printf("m=%d, k1=%d\n",m,k1);
    return 0;
}