/*
Q21. Find the k-th largest element in an array:
1. With sorting.
2. Without fully sorting the array.
3. Using an optimized selection algorithm.

Input: n, followed by n integers, then k (1 <= k <= n).
Example: 6 3 2 1 5 6 4 2
Answer: 5. Duplicates count as separate elements.

1. Insertion sort: O(n^2) worst-case time, O(1) extra space.
2. Repeated maximum selection: O(n*k) time, O(1) extra space.
3. Median-of-medians selection: O(n) worst-case time, O(log n) stack.
The driver uses an O(n) copy to run each method on the original input.
*/

#include <stdio.h>
#include <string.h>

#define MAX 1000

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void insertionSort(int a[], int n) {
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

int repeatedMaximum(int a[], int n, int k) {
    for (int i = 0; i < k; i++) {
        int largest = i;

        for (int j = i + 1; j < n; j++)
            if (a[j] > a[largest])
                largest = j;

        swap(&a[i], &a[largest]);
    }
    return a[k - 1];
}

/* rank is a 0-based position in ascending order. */
int selectRank(int a[], int n, int rank) {
    while (n > 5) {
        int groups = 0;

        /* Collect the median of each group of at most five. */
        for (int start = 0; start < n; start += 5) {
            int size = n - start;
            if (size > 5)
                size = 5;

            insertionSort(a + start, size);
            swap(&a[groups], &a[start + size / 2]);
            groups++;
        }

        int pivot = selectRank(a, groups, groups / 2);
        int low = 0, i = 0, high = n;

        /* Partition into values less than, equal to, and above pivot. */
        while (i < high) {
            if (a[i] < pivot) {
                swap(&a[i], &a[low]);
                low++;
                i++;
            } else if (a[i] > pivot) {
                high--;
                swap(&a[i], &a[high]);
            } else {
                i++;
            }
        }

        if (rank < low) {
            n = low;
        } else if (rank >= high) {
            a += high;
            rank -= high;
            n -= high;
        } else {
            return pivot;
        }
    }

    insertionSort(a, n);
    return a[rank];
}

int main(void) {
    int a[MAX], work[MAX], n, k;

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX)
        return 1;

    for (int i = 0; i < n; i++)
        if (scanf("%d", &a[i]) != 1)
            return 1;

    if (scanf("%d", &k) != 1 || k < 1 || k > n)
        return 1;

    memcpy(work, a, n * sizeof a[0]);
    insertionSort(work, n);
    printf("With sorting: %d\n", work[n - k]);

    memcpy(work, a, n * sizeof a[0]);
    printf("Repeated maximum: %d\n", repeatedMaximum(work, n, k));

    printf("Optimized selection: %d\n", selectRank(a, n, n - k));
    return 0;
}