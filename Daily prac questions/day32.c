/*
Q32. Find all unique triplets in an array whose sum is zero.

Input: n, followed by n integers.
For the screenshot's array (2 0 3 -2 -1 0 1), there are three answers:
-2 -1 3
-2  0 2
-1  0 1

Time: O(n^2), using insertion sort followed by a two-pointer scan.
Extra space: O(1).
*/

#include <stdio.h>

#define MAX 1000

void sort(int a[], int n) {
    for (int i = 1; i < n; i++) {
        int value = a[i];
        int j = i;

        while (j > 0 && a[j - 1] > value) {
            a[j] = a[j - 1];
            j--;
        }
        a[j] = value;
    }
}

int main(void) {
    int a[MAX], n, found = 0;

    if (scanf("%d", &n) != 1 || n < 0 || n > MAX)
        return 1;

    for (int i = 0; i < n; i++)
        if (scanf("%d", &a[i]) != 1)
            return 1;

    sort(a, n);

    for (int i = 0; i + 2 < n; i++) {
        if (i > 0 && a[i] == a[i - 1])
            continue;

        int left = i + 1, right = n - 1;

        while (left < right) {
            long long sum = (long long)a[i] + a[left] + a[right];

            if (sum < 0) {
                left++;
            } else if (sum > 0) {
                right--;
            } else {
                printf("%d %d %d\n", a[i], a[left], a[right]);
                found = 1;

                int usedLeft = a[left], usedRight = a[right];
                while (left < right && a[left] == usedLeft)
                    left++;
                while (left < right && a[right] == usedRight)
                    right--;
            }
        }
    }

    if (!found)
        puts("No triplets found.");
    return 0;
}