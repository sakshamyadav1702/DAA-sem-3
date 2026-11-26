/*
Q26. Implement the Dutch national flag algorithm and identify three
cases where it applies.

1. Sort an array containing only 0, 1, and 2.
2. Group records by low, medium, and high priority.
3. Partition a quicksort array into values below, equal to, and above a pivot.

Input: n, followed by n values from {0, 1, 2}.
Time: O(n). Extra space: O(1).
*/

#include <stdio.h>

#define MAX 1000

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main(void) {
    int a[MAX], n;

    if (scanf("%d", &n) != 1 || n < 0 || n > MAX)
        return 1;

    for (int i = 0; i < n; i++) {
        if (scanf("%d", &a[i]) != 1 || a[i] < 0 || a[i] > 2)
            return 1;
    }

    int low = 0, mid = 0, high = n - 1;

    while (mid <= high) {
        if (a[mid] == 0) {
            swap(&a[low], &a[mid]);
            low++;
            mid++;
        } else if (a[mid] == 1) {
            mid++;
        } else {
            swap(&a[mid], &a[high]);
            high--;
            /* Check mid again because the incoming value is unknown. */
        }
    }

    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);
    putchar('\n');
    return 0;
}