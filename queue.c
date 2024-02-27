#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    head->prev = head->next = head;
    return head;
}

#define max(x, y) (x) > (y) ? (x) : (y)
#define min(x, y) (x) < (y) ? (x) : (y)

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    struct list_head *curr = l->next;
    struct list_head *next;

    while (curr != l) {
        next = curr->next;
        element_t *temp = container_of(curr, element_t, list);
        free(temp->value);
        free(temp);
        curr = next;
    }
    free(l);
    return;
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *element = malloc(sizeof(element_t));
    if (element == NULL)
        return false;
    size_t len = strlen(s) + 1;
    element->value = malloc(len);
    if (element->value == NULL) {
        free(element);
        return false;
    }
    strncpy(element->value, s, len);

    list_add(&element->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *element = malloc(sizeof(element_t));
    if (element == NULL)
        return false;
    size_t len = strlen(s) + 1;
    element->value = malloc(len);
    if (element->value == NULL) {
        free(element);
        return false;
    }
    strncpy(element->value, s, len);

    list_add_tail(&element->list, head);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head)
        return NULL;
    if (head->next == head)
        return NULL;
    struct list_head *first = head->next;
    element_t *temp = container_of(first, element_t, list);
    strncpy(sp, temp->value, min(strlen(temp->value) + 1, bufsize));

    list_del(first);
    return temp;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head)
        return NULL;
    if (head->next == head)
        return NULL;
    struct list_head *last = head->prev;
    element_t *temp = container_of(last, element_t, list);
    strncpy(sp, temp->value, min(strlen(temp->value) + 1, bufsize));

    list_del(last);
    return temp;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head)
        return false;
    if (head->next == head)
        return false;
    struct list_head *fw = head->next;
    struct list_head *bw = head->prev;
    while (!(fw == bw) && !(fw->next == bw)) {
        fw = fw->next;
        bw = bw->prev;
    }

    list_del(bw);
    element_t *temp = container_of(bw, element_t, list);
    free(temp->value);
    free(temp);

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head)
        return;
    struct list_head *node = head->next;
    struct list_head *next = node->next;
    while (!(next == head) && !(node == head)) {
        list_del(node);
        list_add(node, next);
        node = node->next;
        next = node->next;
    }
    return;
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    struct list_head *node;
    struct list_head *temp;

    temp = head->next;
    head->next = head->prev;
    head->prev = temp;
    list_for_each (node, head) {
        temp = node->next;
        node->next = node->prev;
        node->prev = temp;
    }
    return;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
