#include <iostream>

using namespace std;

void reverse(int a[], int left, int right) {
    while (left < right) {
        int temp = a[left];
        a[left] = a[right];
        a[right] = temp;
        left++;
        right--;
    }
}


void swap(int a[], int m, int n) {
    reverse(a, 0, m - 1);
    
    reverse(a, m, m + n - 1);
    
    reverse(a, 0, m + n - 1);
}

int main() {
    int m = 3, n = 4;
    int a[] = {1, 2, 3, 4, 5, 6, 7}; // 前3个: {1,2,3}, 后4个: {4,5,6,7}

    swap(a, m, n);

    // 输出验证结果
    for (int i = 0; i < m + n; i++) {
        cout << a[i] << " ";
    }
    // 预期输出: 4 5 6 7 1 2 3
    return 0;
}