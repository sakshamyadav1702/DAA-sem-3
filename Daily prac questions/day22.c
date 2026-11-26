/*
Q22. Shift all zeros to the end of an array. Write a normal and an
optimized solution and analyze both. Preserve the order of nonzero values.

Input: n, followed by n integers.
Example: 5 0 1 0 3 12
Output from both methods: 1 3 12 0 0

Normal, using a temporary array: O(n) time, O(n) extra space.
Optimized, writing in place: O(n) time, O(1) extra space.
*/

#include <stdio.h>

#define MAX 1000

void printArray(const int a[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);
    putchar('\n');
}

int main(void) {
    int a[MAX], temp[MAX], n, write = 0;

    if (scanf("%d", &n) != 1 || n < 0 || n > MAX)
        return 1;

    for (int i = 0; i < n; i++)
        if (scanf("%d", &a[i]) != 1)
            return 1;

    /* Normal method: build the result in temp. */
    for (int i = 0; i < n; i++)
        if (a[i] != 0)
            temp[write++] = a[i];

    while (write < n)
        temp[write++] = 0;

    printf("Normal: ");
    printArray(temp, n);

    /* Optimized method: compact nonzero values inside a itself. */
    write = 0;
    for (int i = 0; i < n; i++)
        if (a[i] != 0)
            a[write++] = a[i];

    while (write < n)
        a[write++] = 0;

    printf("Optimized: ");
    printArray(a, n);
    return 0;
}