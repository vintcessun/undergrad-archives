#include <stdio.h>

int main() {
    int n;
    scanf("%d", &n);

    int a[105];

    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    int m;
    scanf("%d", &m);

    for (int i = 0; i < m; i++) {
        int x, y;
        scanf("%d %d", &x, &y);

        int give = a[x] / 2;
        a[x] -= give;
        a[y] += give;
    }

    int maxVal = a[0];
    int minVal = a[0];

    for (int i = 1; i < n; i++) {
        if (a[i] > maxVal) {
            maxVal = a[i];
        }
        if (a[i] < minVal) {
            minVal = a[i];
        }
    }

    printf("%d\n", maxVal - minVal);

    return 0;
}