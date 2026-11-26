/*
Q19. Detect a cycle in a doubly linked list using slow and fast pointers.

Input: n, followed by n node values, then a position.
Enter -1 for no cycle, or a 0-based position for the tail's next link.
Example: 5 10 20 30 40 50 1

Cycle detection: O(n) time, O(1) extra space.
*/

#include <stdio.h>

#define MAX 1000

typedef struct Node {
    int data;
    struct Node *prev;
    struct Node *next;
} Node;

int hasCycle(Node *head) {
    Node *slow = head;
    Node *fast = head;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast)
            return 1;
    }
    return 0;
}

int main(void) {
    Node nodes[MAX];
    int n, position;

    if (scanf("%d", &n) != 1 || n < 0 || n > MAX)
        return 1;

    for (int i = 0; i < n; i++) {
        if (scanf("%d", &nodes[i].data) != 1)
            return 1;

        nodes[i].prev = (i > 0) ? &nodes[i - 1] : NULL;
        nodes[i].next = (i + 1 < n) ? &nodes[i + 1] : NULL;
    }

    if (scanf("%d", &position) != 1 || position < -1 || position >= n)
        return 1;

    /* Introduce a cycle in the forward links for testing. */
    if (position != -1)
        nodes[n - 1].next = &nodes[position];

    Node *head = (n > 0) ? &nodes[0] : NULL;
    puts(hasCycle(head) ? "Cycle detected" : "No cycle");
    return 0;
}