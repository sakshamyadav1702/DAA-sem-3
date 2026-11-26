/*
Q28. A staircase has n steps. Each time you can climb 1 or 2 steps.
In how many distinct ways can you reach the top?

Input: n (0 to 92, so the answer fits in an unsigned 64-bit integer).
Example: n = 4 gives 5 ways.
Time: O(n). Extra space: O(1).
*/

#include <stdio.h>

int main(void) {
    int n;

    if (scanf("%d", &n) != 1 || n < 0 || n > 92) {
        puts("Enter a number from 0 to 92.");
        return 1;
    }

    unsigned long long previous = 1;  /* ways to climb 0 steps */
    unsigned long long current = 1;   /* ways to climb 1 step */

    for (int step = 2; step <= n; step++) {
        unsigned long long next = previous + current;
        previous = current;
        current = next;
    }

    printf("%llu\n", current);
    return 0;
}