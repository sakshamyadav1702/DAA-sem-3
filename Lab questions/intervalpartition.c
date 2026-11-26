#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int start, end, index;
} Interval;

typedef struct {
    int end, room;
} Room;

int compare(const void *a, const void *b) {
    const Interval *x = a, *y = b;

    if (x->start != y->start)
        return (x->start > y->start) - (x->start < y->start);

    return (x->end > y->end) - (x->end < y->end);
}

void swap(Room *a, Room *b) {
    Room temp = *a;
    *a = *b;
    *b = temp;
}

void push(Room heap[], int *size, Room value) {
    int i = (*size)++;
    heap[i] = value;

    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap[parent].end <= heap[i].end)
            break;

        swap(&heap[parent], &heap[i]);
        i = parent;
    }
}

Room pop(Room heap[], int *size) {
    Room result = heap[0];
    heap[0] = heap[--(*size)];

    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < *size && heap[left].end < heap[smallest].end)
            smallest = left;
        if (right < *size && heap[right].end < heap[smallest].end)
            smallest = right;

        if (smallest == i)
            break;

        swap(&heap[i], &heap[smallest]);
        i = smallest;
    }

    return result;
}

int main(void) {
    int n;
    printf("Enter number of intervals: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number of intervals.\n");
        return 1;
    }

    Interval *intervals = malloc((size_t)n * sizeof(*intervals));
    Room *heap = malloc((size_t)n * sizeof(*heap));
    int *assignment = malloc((size_t)n * sizeof(*assignment));

    if (!intervals || !heap || !assignment) {
        free(intervals);
        free(heap);
        free(assignment);
        return 1;
    }

    printf("Enter start and end time of each interval:\n");
    for (int i = 0; i < n; i++) {
        if (scanf("%d %d", &intervals[i].start,
                  &intervals[i].end) != 2 ||
            intervals[i].start >= intervals[i].end) {
            printf("Invalid interval: start must be less than end.\n");
            free(intervals);
            free(heap);
            free(assignment);
            return 1;
        }
        intervals[i].index = i;
    }

    qsort(intervals, n, sizeof(*intervals), compare);

    int heapSize = 0, roomCount = 0;

    for (int i = 0; i < n; i++) {
        Room room;

        if (heapSize > 0 && heap[0].end <= intervals[i].start) {
            room = pop(heap, &heapSize);  /* Reuse earliest free room */
        } else {
            room.room = ++roomCount;     /* Allocate a new room */
        }

        room.end = intervals[i].end;
        assignment[intervals[i].index] = room.room;
        push(heap, &heapSize, room);
    }

    printf("\nMinimum rooms required: %d\n", roomCount);
    for (int i = 0; i < n; i++)
        printf("Interval %d -> Room %d\n", i + 1, assignment[i]);

    free(intervals);
    free(heap);
    free(assignment);
    return 0;
}