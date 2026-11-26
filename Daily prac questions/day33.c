/*
Q33. Remove the N-th node from the end of a singly linked list.

Input: list length, then the node values, then N.
N = 1 removes the last node. N = list length removes the head.
Example: 5 10 20 30 40 50 2
Output: 10 20 30 50
Removal: O(length) time, O(1) extra space.
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

void freeList(Node *head) {
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

int main(void) {
    Node dummy = {0, NULL};
    Node *tail = &dummy;
    int length, n;

    if (scanf("%d", &length) != 1 || length < 0)
        return 1;

    for (int i = 0; i < length; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            freeList(dummy.next);
            return 1;
        }

        Node *node = malloc(sizeof *node);
        if (node == NULL) {
            freeList(dummy.next);
            return 1;
        }

        node->data = value;
        node->next = NULL;
        tail->next = node;
        tail = node;
    }

    if (scanf("%d", &n) != 1 || n < 1 || n > length) {
        puts("Invalid position.");
        freeList(dummy.next);
        return 1;
    }

    Node *fast = &dummy;
    Node *slow = &dummy;

    for (int i = 0; i < n; i++)
        fast = fast->next;

    while (fast->next != NULL) {
        fast = fast->next;
        slow = slow->next;
    }

    Node *removed = slow->next;
    slow->next = removed->next;
    free(removed);

    if (dummy.next == NULL)
        printf("Empty list");

    for (Node *p = dummy.next; p != NULL; p = p->next)
        printf("%d ", p->data);
    putchar('\n');

    freeList(dummy.next);
    return 0;
}