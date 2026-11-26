#include <stdio.h>

void selectActivities(int start[], int finish[], int n) {
    printf("Selected Activities:\n");
    
    // 1. The first activity is always picked
    int i = 0;
    printf("Activity at index %d (Start: %d, Finish: %d)\n", i, start[i], finish[i]);

    // 2. Look at all the other activities
    for (int j = 1; j < n; j++) {
        // If this activity starts AFTER or AT the same time 
        // the previous one finished, we select it!
        if (start[j] >= finish[i]) {
            printf("Activity at index %d (Start: %d, Finish: %d)\n", j, start[j], finish[j]);
            i = j; // Update our last chosen activity
        }
    }
}

int main() {
    // We use simple parallel arrays (assumed to be sorted by finish time)
    int start[]  = {1, 3, 0, 5, 8, 5};
    int finish[] = {2, 4, 6, 7, 9, 9};
    
    int n = 6; // Total number of activities

    selectActivities(start, finish, n);

    return 0;
}
