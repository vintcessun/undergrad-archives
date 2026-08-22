#include <stdio.h>

int main() {
    int t;
    scanf("%d", &t);

    while (t--) {
        int a, b, l, r;
        scanf("%d %d %d %d", &a, &b, &l, &r);

        int f[1005] = {0};
        int sum[1005] = {0};

        f[0] = a % 1000;
        f[1] = b % 1000;

        sum[0] = f[0];

        if (r >= 1) {
            sum[1] = (sum[0] + f[1]) % 1000;
        }

        for (int i = 2; i <= r; i++) {
            f[i] = (f[i - 1] + f[i - 2]) % 1000;
            sum[i] = (sum[i - 1] + f[i]) % 1000;
        }

        int ans;
        if (l == 0) {
            ans = sum[r];
        } else {
            ans = (sum[r] - sum[l - 1] + 1000) % 1000;
        }

        printf("%d\n", ans);
    }

    return 0;
}