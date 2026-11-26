/*
Q24. Find the 0-based index of any element strictly greater than its
neighbors in O(log n) time.

Input: n, followed by n integers. Adjacent values must be different
(the standard peak-element assumption). A missing neighbor counts as
negative infinity, so an endpoint can be a peak.
Example: 4 1 2 3 1
Output: 2

Binary search: O(log n) time, O(1) extra space. Reading input takes O(n).
*/

#include <stdio.h>

#define MAX 1000

int findPeak(const int a[], int n) {
    int left = 0, right = n - 1;

    while (left < right) {
        int mid = left + (right - left) / 2;

        if (a[mid] < a[mid + 1])
            left = mid + 1;  /* A peak exists on the right. */
        else
            right = mid;     /* mid itself could be the peak. */
    }
    return left;
}

int main(void) {
    int a[MAX], n;

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX)
        return 1;

    for (int i = 0; i < n; i++)
        if (scanf("%d", &a[i]) != 1)
            return 1;

    printf("Peak index: %d\n", findPeak(a, n));
    return 0;
}