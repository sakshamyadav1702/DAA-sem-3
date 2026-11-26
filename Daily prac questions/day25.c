/*
Q25. Implement the Tower of Hanoi problem and analyze its complexity.

Input: Number of disks, n. Rod A is the source, B is the helper,
and C is the destination. Disk 1 is the smallest.
Number of moves: 2^n - 1.
Time: O(2^n). Extra space: O(n) recursion stack.
*/

#include <stdio.h>

void hanoi(int n, char source, char helper, char destination) {
    if (n == 0)
        return;

    hanoi(n - 1, source, destination, helper);
    printf("Move disk %d from %c to %c\n", n, source, destination);
    hanoi(n - 1, helper, source, destination);
}

int main(void) {
    int n;

    if (scanf("%d", &n) != 1 || n < 0)
        return 1;

    hanoi(n, 'A', 'B', 'C');
    return 0;
}