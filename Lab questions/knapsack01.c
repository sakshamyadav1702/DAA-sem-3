#include <stdio.h>

#define MAX_ITEMS 100
#define MAX_CAPACITY 1000

int dp[MAX_ITEMS + 1][MAX_CAPACITY + 1];

int main(void)
{
    int n, capacity;
    int weight[MAX_ITEMS + 1];
    int value[MAX_ITEMS + 1];

    printf("Enter number of items (1-100): ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_ITEMS) {
        printf("Invalid number of items.\n");
        return 1;
    }

    for (int i = 1; i <= n; i++) {
        printf("Enter weight and value of item %d: ", i);

        if (scanf("%d %d", &weight[i], &value[i]) != 2 ||
            weight[i] <= 0 || value[i] < 0) {
            printf("Invalid weight or value.\n");
            return 1;
        }
    }

    printf("Enter capacity (0-1000): ");
    if (scanf("%d", &capacity) != 1 ||
        capacity < 0 || capacity > MAX_CAPACITY) {
        printf("Invalid capacity.\n");
        return 1;
    }

    /* dp[i][w] = best value using first i items, capacity w. */
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {

            /* Option 1: Do not select this item. */
            dp[i][w] = dp[i - 1][w];

            /* Option 2: Select this item if it fits. */
            if (weight[i] <= w) {
                int take = value[i] +
                           dp[i - 1][w - weight[i]];

                if (take > dp[i][w])
                    dp[i][w] = take;
            }
        }
    }

    printf("\nMaximum value: %d\n", dp[n][capacity]);

    return 0;
}