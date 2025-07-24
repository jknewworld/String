#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXNODE 100005
#define MAXLEN 1000005
#define ALPHABET 26
#define MAXWORDS 105

int next[MAXNODE][ALPHABET];
int fail[MAXNODE];
int outputLength[MAXNODE];
int nodeCount;
int queue[MAXNODE];
int matchStart[MAXLEN];
int matchEnd[MAXLEN];
int matchIndex[MAXLEN];
int matchCount;
char words[MAXWORDS][105];


int newNode() {
    for (int i = 0; i < ALPHABET; i++)
        next[nodeCount][i] = 0;
    fail[nodeCount] = 0;
    outputLength[nodeCount] = 0;
    return nodeCount++;
}

void insert(int wordIndex) {
    int u = 0;
    int i = 0;
    while (words[wordIndex][i] != '\0') {
        int c = words[wordIndex][i] - 'a';
        if (!next[u][c]) {
            next[u][c] = newNode();
        }
        u = next[u][c];
        i++;
    }
    outputLength[u] = i; 
}

void build() {
    int nodes[MAXNODE];
    int size = 0;

    for (int c = 0; c < ALPHABET; c++) {
        if (next[0][c]) {
            fail[next[0][c]] = 0;
            nodes[size++] = next[0][c];
        }
    }

    for (int i = 0; i < size; i++) {
        int r = nodes[i];
        for (int c = 0; c < ALPHABET; c++) {
            int u = next[r][c];
            if (!u) continue;
            int f = fail[r];
            while (f && !next[f][c]) f = fail[f];
            if (next[f][c]) f = next[f][c];
            fail[u] = f;

            if (!outputLength[u])
                outputLength[u] = outputLength[f];

            nodes[size++] = u;
        }
    }
}


int compare(const void *a, const void *b) {
    int i = *(int*)a;
    int j = *(int*)b;
    return matchEnd[i] - matchEnd[j];
}

int minDeletions(char s[]) {
    int len = 0;
    while (s[len] != '\0') len++;

    int u = 0;
    matchCount = 0;

    for (int i = 0; i < len; i++) {
        int c = s[i] - 'a';
        while (u && !next[u][c])
            u = fail[u];
        if (next[u][c]) u = next[u][c];

        int temp = u;
        while (temp) {
            if (outputLength[temp]) {
                matchStart[matchCount] = i - outputLength[temp] + 1;
                matchEnd[matchCount] = i;
                matchIndex[matchCount] = matchCount;
                matchCount++;
            }
            temp = fail[temp];
        }
    }

    if (matchCount == 0) return 0;

    qsort(matchIndex, matchCount, sizeof(int), compare);

    int deletions = 0;
    int lastDeleted = -1;
    for (int i = 0; i < matchCount; i++) {
        int idx = matchIndex[i];
        if (matchStart[idx] > lastDeleted) {
            deletions++;
            lastDeleted = matchEnd[idx];
        }
    }

    return deletions;
}

int main() {
    int n, m;
    scanf("%d", &n); 
    scanf("%d", &m); 

    nodeCount = 1;

    for (int i = 0; i < m; i++) {
        scanf("%s", words[i]);
        insert(i);
    }

    build();

    char s[MAXLEN];
    for (int i = 0; i < n; i++) {
        scanf("%s", s);
        int ans = minDeletions(s);
        printf("%d\n", ans);
    }

    return 0;
}
