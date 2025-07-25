#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

#define MAX_LEN 1001
#define MAX_CASES 1024
#define MAX_FORBIDDEN 100

char pattern[MAX_LEN];
char forbidden[MAX_FORBIDDEN][MAX_LEN];
char results[MAX_CASES][MAX_LEN];

int wildcardPos[11];
int wildcardCount = 0;
int resultCount = 0;
int forbiddenCount = 0;
int maxOnes = 0;
int minZeros = 0;

int compareBinaryDesc(const void *a, const void *b) {
    return strcmp((char *)b, (char *)a);
}

int isInForbiddenList(char candidate[MAX_LEN]) {
    int i;
    for (i = 0; i < forbiddenCount; i++) {
        if (strcmp(candidate, forbidden[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void countBits(char s[MAX_LEN], int *ones, int *zeros) {
    int i;
    *ones = 0;
    *zeros = 0;
    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] == '1') (*ones)++;
        else if (s[i] == '0') (*zeros)++;
    }
}

void generateBinaryStrings(int index, char current[MAX_LEN]) {
    if (index == wildcardCount) {
        int ones, zeros;
        countBits(current, &ones, &zeros);

        if (ones <= maxOnes && zeros >= minZeros && !isInForbiddenList(current)) {
            strcpy(results[resultCount], current);
            resultCount++;
        }
        return;
    }

    int pos = wildcardPos[index];

    current[pos] = '1';
    generateBinaryStrings(index + 1, current);

    current[pos] = '0';
    generateBinaryStrings(index + 1, current);
}

int main() {
    char current[MAX_LEN];
    int i;

    scanf("%s", pattern);
    int len = strlen(pattern);

    for (i = 0; i < len; i++) {
        if (pattern[i] == '!') {
            wildcardPos[wildcardCount++] = i;
        }
    }

    scanf("%d", &maxOnes);
    scanf("%d", &minZeros);

    scanf("%d", &forbiddenCount);
    for (i = 0; i < forbiddenCount; i++) {
        scanf("%s", forbidden[i]);
    }

    strcpy(current, pattern);
    generateBinaryStrings(0, current);

    qsort(results, resultCount, MAX_LEN, compareBinaryDesc);

    for (i = 0; i < resultCount; i++) {
        printf("%s\n", results[i]);
    }

    return 0;
}
