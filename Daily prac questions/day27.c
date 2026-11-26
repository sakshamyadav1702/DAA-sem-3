/*
Q27. A mall has a 2D parking layout. A 0 means empty and a 1 means
occupied. Find the first available spot for an arriving car and park it.

Input: rows, columns, followed by the parking grid.
Scan from left to right, starting with the top row.
Output: the spot using 1-based row and column numbers, then the updated grid.
Search time: O(rows * columns) worst case. Extra space: O(1).
*/

#include <stdio.h>

#define MAX 100

int main(void) {
    int lot[MAX][MAX], rows, cols;

    if (scanf("%d%d", &rows, &cols) != 2 ||
        rows < 1 || rows > MAX || cols < 1 || cols > MAX)
        return 1;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (scanf("%d", &lot[r][c]) != 1 ||
                (lot[r][c] != 0 && lot[r][c] != 1))
                return 1;
        }
    }

    int parked = 0;

    for (int r = 0; r < rows && !parked; r++) {
        for (int c = 0; c < cols; c++) {
            if (lot[r][c] == 0) {
                lot[r][c] = 1;
                printf("Car parked at row %d, column %d\n", r + 1, c + 1);
                parked = 1;
                break;
            }
        }
    }

    if (!parked)
        puts("Parking lot is full.");

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++)
            printf("%d ", lot[r][c]);
        putchar('\n');
    }
    return 0;
}