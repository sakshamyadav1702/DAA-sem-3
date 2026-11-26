/*
Q31. Find all anagrams of a pattern in a string. Print the 0-based
starting index of every match using normal and optimized methods.

Input: Text on the first line and a nonempty pattern on the second.
Each line may contain up to 1000 characters. Matching is case-sensitive.
Example: text = cbaebabacd, pattern = abc
Output from both methods: 0 6

For text length n and pattern length m, with a fixed byte alphabet:
Normal: O(n*m) time, O(1) extra space.
Optimized sliding window: O(n) time for m <= n, O(1) extra space.
*/

#include <stdio.h>
#include <string.h>

void normal(const char text[], const char pattern[], int n, int m) {
    int target[256] = {0};
    int found = 0;

    for (int i = 0; i < m; i++)
        target[(unsigned char)pattern[i]]++;

    for (int start = 0; start + m <= n; start++) {
        int count[256] = {0};

        for (int j = 0; j < m; j++)
            count[(unsigned char)text[start + j]]++;

        int ch = 0;
        while (ch < 256 && count[ch] == target[ch])
            ch++;

        if (ch == 256) {
            printf("%d ", start);
            found = 1;
        }
    }

    if (!found)
        printf("None");
    putchar('\n');
}

void optimized(const char text[], const char pattern[], int n, int m) {
    int need[256] = {0};
    int missing = m, found = 0;

    for (int i = 0; i < m; i++)
        need[(unsigned char)pattern[i]]++;

    for (int right = 0; right < n; right++) {
        unsigned char in = (unsigned char)text[right];

        if (need[in] > 0)
            missing--;
        need[in]--;

        if (right >= m) {
            unsigned char out = (unsigned char)text[right - m];
            need[out]++;
            if (need[out] > 0)
                missing++;
        }

        if (right >= m - 1 && missing == 0) {
            printf("%d ", right - m + 1);
            found = 1;
        }
    }

    if (!found)
        printf("None");
    putchar('\n');
}

int main(void) {
    char text[1002], pattern[1002];

    if (!fgets(text, sizeof text, stdin) ||
        !fgets(pattern, sizeof pattern, stdin))
        return 1;

    text[strcspn(text, "\r\n")] = '\0';
    pattern[strcspn(pattern, "\r\n")] = '\0';

    int n = (int)strlen(text);
    int m = (int)strlen(pattern);
    if (m == 0)
        return 1;

    printf("Normal indices: ");
    normal(text, pattern, n, m);

    printf("Optimized indices: ");
    optimized(text, pattern, n, m);
    return 0;
}