#include <stdio.h>

int main() {
    int n;
    scanf("%d", &n);

    double x[105], y[105], r[105];

    for (int i = 0; i < n; i++) {
        scanf("%lf %lf %lf", &x[i], &y[i], &r[i]);
    }

    int p;
    scanf("%d", &p);

    for (int i = 0; i < n; i++) {
        if (i == p) continue;

        double dx = x[i] - x[p];
        double dy = y[i] - y[p];

        double dist2 = dx * dx + dy * dy;
        double sumR = r[i] + r[p];

        if (dist2 <= sumR * sumR) {
            printf("Biu\n");
            return 0;
        }
    }

    printf("Alive\n");
    return 0;
}