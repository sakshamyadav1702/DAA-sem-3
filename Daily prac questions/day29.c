/*
Q29. Find the longest contiguous palindrome within a string.

Input: One line of text, up to 1000 characters.
Example: babad
Output: bab ("aba" is also a longest palindrome).
Time: O(n^2). Extra space: O(1).
*/

#include <stdio.h>
#include <string.h>

void expand(const char s[], int n, int left, int right,
            int *bestStart, int *bestLength) {
    while (left >= 0 && right < n && s[left] == s[right]) {
        left--;
        right++;
    }

    /* The matching part starts at left + 1 and ends at right - 1. */
    int length = right - left - 1;
    if (length > *bestLength) {
        *bestStart = left + 1;
        *bestLength = length;
    }
}

int main(void) {
    char s[1002];
    int bestStart = 0, bestLength = 0;

    if (fgets(s, sizeof s, stdin) == NULL)
        return 1;
    s[strcspn(s, "\r\n")] = '\0';

    int n = (int)strlen(s);
    for (int i = 0; i < n; i++) {
        expand(s, n, i, i, &bestStart, &bestLength);     /* odd length */
        expand(s, n, i, i + 1, &bestStart, &bestLength); /* even length */
    }

    printf("%.*s\n", bestLength, s + bestStart);
    return 0;
}