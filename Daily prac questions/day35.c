/*
Q35. Find the third maximum number in an array in better than O(n log n)
time. Duplicate elements count as one value.

Input: n, followed by n integers.
Example: 6 5 5 4 3 3 1
Output: 3
If fewer than three distinct values exist, print a message.
Time: O(n). Extra space: O(1).
*/

#include <stdio.h>

int main(void) {
    int n, top[3], used = 0;

    if (scanf("%d", &n) != 1 || n < 0)
        return 1;

    for (int i = 0; i < n; i++) {
        int value;
        if (scanf("%d", &value) != 1)
            return 1;

        /* top holds up to three distinct values in descending order. */
        int pos = 0;
        while (pos < used && top[pos] > value)
            pos++;

        if (pos < used && top[pos] == value)
            continue;  /* Duplicate. */

        if (pos == 3)
            continue;  /* Below all three stored values. */

        if (used < 3)
            used++;

        for (int j = used - 1; j > pos; j--)
            top[j] = top[j - 1];

        top[pos] = value;
    }

    if (used == 3)
        printf("Third distinct maximum: %d\n", top[2]);
    else
        puts("Fewer than three distinct values.");

    return 0;
}