/*
Q30. Given an unsorted array containing positive and negative integers,
find the first missing positive number. Give normal and optimized solutions.

Input: n, followed by n integers.
Example: 4 3 4 -1 1
Answer: 2

Normal: O(n^2) time, O(1) extra space.
Optimized: O(n) time, O(1) extra space. It rearranges the array.
*/

#include <stdio.h>

#define MAX 1000

int missingNormal(const int a[], int n) {
    for (int candidate = 1; candidate <= n; candidate++) {
        int i = 0;

        while (i < n && a[i] != candidate)
            i++;

        if (i == n)
            return candidate;
    }
    return n + 1;
}

int missingOptimized(int a[], int n) {
    for (int i = 0; i < n; i++) {
        /* A value x in [1, n] belongs at index x - 1. */
        while (a[i] >= 1 && a[i] <= n) {
            int target = a[i] - 1;

            if (a[target] == a[i])
                break;  /* Already placed, or a duplicate. */

            int temp = a[target];
            a[target] = a[i];
            a[i] = temp;
        }
    }

    for (int i = 0; i < n; i++)
        if (a[i] != i + 1)
            return i + 1;

    return n + 1;
}

int main(void) {
    int a[MAX], n;

    if (scanf("%d", &n) != 1 || n < 0 || n > MAX)
        return 1;

    for (int i = 0; i < n; i++)
        if (scanf("%d", &a[i]) != 1)
            return 1;

    printf("Normal: %d\n", missingNormal(a, n));
    printf("Optimized: %d\n", missingOptimized(a, n));
    return 0;
}