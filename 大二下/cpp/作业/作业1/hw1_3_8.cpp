#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

int main() {
    double pi_over_4 = 0.0;
    double term = 1.0;     // 当前项的值
    double denominator = 1.0; // 分母
    int sign = 1;          // 符号位，正负交替

    // 循环直到当前项的绝对值小于 10^-8
    while (fabs(term) >= 1e-8) {
        term = sign * (1.0 / denominator);
        pi_over_4 += term;
        
        // 更新下一项的参数
        denominator += 2.0;
        sign = -sign;
    }

    double pi = pi_over_4 * 4.0;

    // 输出结果，保留高精度以验证计算效果
    cout << "计算得到的圆周率 pi = " << fixed << setprecision(8) << pi << endl;

    return 0;
}