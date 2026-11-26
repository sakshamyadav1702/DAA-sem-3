/*
Q20. Print the longest substring without repeating a character.

Input: One line of text, up to 1000 characters. Spaces are allowed.
Example: abcabcbb
Output: abc
Time: O(n). Extra space: O(1) for a fixed byte alphabet.
If multiple answers have the same length, print the first.
*/

#include <stdio.h>
#include <string.h>

int main(void) {
    char text[1002];
    int last[256];
    int left = 0, bestStart = 0, bestLength = 0;

    if (fgets(text, sizeof text, stdin) == NULL)
        return 1;
    text[strcspn(text, "\r\n")] = '\0';

    for (int i = 0; i < 256; i++)
        last[i] = -1;

    for (int right = 0; text[right] != '\0'; right++) {
        unsigned char ch = (unsigned char)text[right];

        if (last[ch] >= left)
            left = last[ch] + 1;

        last[ch] = right;
        int length = right - left + 1;

        if (length > bestLength) {
            bestStart = left;
            bestLength = length;
        }
    }

    printf("%.*s\n", bestLength, text + bestStart);
    return 0;
}