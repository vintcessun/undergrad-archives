#include <stdio.h>
#include <string.h>

int isValley(char s[]) {
    int len = strlen(s);
    int trend = 0;

    for (int i = 1; i < len; i++) {
        if (s[i] == s[i - 1]) {
            continue;
        }

        if (s[i] > s[i - 1]) {
            trend = 1;
        } else {
            if (trend == 1) {
                return 0;
            }
            trend = -1;
        }
    }

    return 1;
}

int main() {
    char s[20];

    while (scanf("%s", s) != EOF) {
        if (isValley(s)) {
            printf("Yes\n");
        } else {
            printf("No\n");
        }
    }

    return 0;
}