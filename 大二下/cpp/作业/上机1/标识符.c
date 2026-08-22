#include <stdio.h>
#include <string.h>

int isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

int main() {
    int n;
    scanf("%d", &n);

    while (n--) {
        char s[105];
        scanf("%s", s);

        int ok = 1;

        if (!(isLetter(s[0]) || s[0] == '_')) {
            ok = 0;
        }

        for (int i = 1; s[i] != '\0'; i++) {
            if (!(isLetter(s[i]) || isDigit(s[i]) || s[i] == '_')) {
                ok = 0;
                break;
            }
        }

        if (ok) {
            printf("Great\n");
        } else {
            printf("Bad\n");
        }
    }

    return 0;
}