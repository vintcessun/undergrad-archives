#include <stdio.h>
#include <string.h>

int main() {
    char c[30];
    char line[200005];
    char decode[26];

    fgets(c, sizeof(c), stdin);

    for (int i = 0; i < 26; i++) {
        decode[c[i] - 'A'] = 'A' + i;
    }

    fgets(line, sizeof(line), stdin);

    int firstWord = 1;
    char *token = strtok(line, " \n\r");

    while (token != NULL) {
        if (!firstWord) {
            printf(" ");
        }
        firstWord = 0;

        for (int i = 0; token[i] != '\0'; i++) {
            putchar(decode[token[i] - 'A']);
        }

        token = strtok(NULL, " \n\r");
    }

    printf("\n");

    return 0;
}