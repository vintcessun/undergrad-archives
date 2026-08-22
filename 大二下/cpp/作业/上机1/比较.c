#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmp(const void *a, const void *b) {
    long long x = *(long long *)a;
    long long y = *(long long *)b;

    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

long long parseNumber(char *s) {
    int len = strlen(s);

    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';
        len--;
    }

    if (len >= 2 && s[0] == '0' && s[1] == 'x') {
        return strtoll(s + 2, NULL, 16);
    } else if (len >= 2 && s[0] == '0' && s[1] == 'b') {
        long long num = 0;
        for (int i = 2; s[i] != '\0'; i++) {
            num = num * 2 + (s[i] - '0');
        }
        return num;
    } else if (len >= 2 && s[0] == '0') {
        return strtoll(s + 1, NULL, 8);
    } else {
        return strtoll(s, NULL, 10);
    }
}

int main() {
    char line[10005];
    long long nums[105];
    int n = 0;

    fgets(line, sizeof(line), stdin);

    char *token = strtok(line, ",");

    while (token != NULL) {
        nums[n++] = parseNumber(token);
        token = strtok(NULL, ",");
    }

    qsort(nums, n, sizeof(long long), cmp);

    for (int i = 0; i < n; i++) {
        if (i > 0) {
            printf(",");
        }
        printf("%lld", nums[i]);
    }

    return 0;
}