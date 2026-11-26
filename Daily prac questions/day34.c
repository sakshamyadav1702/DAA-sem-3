/*
Q34. Find the square root of a number without using a built-in
square-root function.

Input: One finite nonnegative number. Fractions are allowed.
Example: 2.25 gives 1.5.
Method: Newton's method. Time: O(t) for t iterations. Extra space: O(1).
*/

#include <stdio.h>
#include <math.h>
#include <float.h>

int main(void) {
    double number;

    if (scanf("%lf", &number) != 1 || !isfinite(number) || number < 0) {
        puts("Enter a finite nonnegative number.");
        return 1;
    }

    if (number == 0) {
        puts("0");
        return 0;
    }

    double guess = (number >= 1) ? number : 1;

    while (1) {
        double next = 0.5 * guess + 0.5 * (number / guess);
        double difference = (next > guess) ? next - guess : guess - next;

        if (difference <= DBL_EPSILON * next) {
            printf("%.12g\n", next);
            break;
        }
        guess = next;
    }
    return 0;
}