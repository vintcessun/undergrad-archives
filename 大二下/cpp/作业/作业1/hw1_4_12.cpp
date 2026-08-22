#include <iostream>

using namespace std;

// Ackermann 递归函数
int ack(int m, int n) {
    if (m == 0) {
        return n + 1;
    } else if (n == 0) {
        return ack(m - 1, 1);
    } else {
        return ack(m - 1, ack(m, n - 1));
    }
}

int main() {
    int m = 2, n = 2;
    cout << "ack(" << m << ", " << n << ") = " << ack(m, n) << endl;
    return 0;
}