/*
Q23. Given an integer array, move all even integers to the beginning,
followed by all odd integers. Analyze the complexity.

Input: n, followed by n integers.
Time: O(n). Extra space: O(1).
The order within each group may change.
*/

#include <stdio.h>

#define MAX 1000

int main(void) {
    int a[MAX], n, nextEven = 0;

    if (scanf("%d", &n) != 1 || n < 0 || n > MAX)
        return 1;

    for (int i = 0; i < n; i++)
        if (scanf("%d", &a[i]) != 1)
            return 1;

    for (int i = 0; i < n; i++) {
        if (a[i] % 2 == 0) {
            int temp = a[i];
            a[i] = a[nextEven];
            a[nextEven] = temp;
            nextEven++;
        }
    }

    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);
    putchar('\n');
    return 0;
}