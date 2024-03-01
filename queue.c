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
    if (head != NULL)
        INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
        return;
    struct list_head *curr = l->next;
    struct list_head *next;
    while (curr != l) {
        next = curr->next;
        element_t *temp = container_of(curr, element_t, list);
        if (temp->value != NULL)
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
    strncpy(sp, temp->value, bufsize);
    sp[bufsize - 1] = '\0';

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
    strncpy(sp, temp->value, bufsize);
    sp[bufsize - 1] = '\0';

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

static inline bool is_element_eq(struct list_head *cmp1, struct list_head *cmp2)
{
    return strcmp(list_entry(cmp1, element_t, list)->value,
                  list_entry(cmp2, element_t, list)->value) == 0
               ? true
               : false;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head)
        return false;
    element_t *element;
    element_t *safe;
    struct list_head *dup_start = head->next;
    struct list_head *dup_end = head->next;
    struct list_head *temp;
    bool is_cnting = false;
    struct list_head trash;
    INIT_LIST_HEAD(&trash);
    while (dup_end != head) {
        dup_end = dup_end->next;
        if (is_cnting) {
            if (dup_end == head || !is_element_eq(dup_start, dup_end)) {
                dup_start = dup_start->prev;
                temp = dup_end->prev;
                list_cut_position(&trash, dup_start, temp);
                list_for_each_entry_safe (element, safe, &trash, list) {
                    if (element) {
                        free(element->value);
                        free(element);
                    }
                }
                INIT_LIST_HEAD(&trash);
                dup_start = dup_end;
                is_cnting = false;
            }
        } else {
            if (dup_end == head || !is_element_eq(dup_start, dup_end)) {
                dup_start = dup_start->next;
            } else {
                is_cnting = true;
            }
        }
    }

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
    if (!head)
        return;

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
    if (!head)
        return;
    if (k <= 0)
        return;
    if (q_size(head) < k)
        return;
    struct list_head stack;
    struct list_head result;
    INIT_LIST_HEAD(&stack);
    INIT_LIST_HEAD(&result);
    while (!list_empty(head)) {
        list_move(head->next, &stack);
        if (q_size(&stack) == k) {
            while (!list_empty(&stack)) {
                list_move_tail(stack.next, &result);
            }
        }
    }
    while (!list_empty(&stack)) {
        list_move_tail(stack.prev, &result);
    }
    list_splice(&result, head);
    return;
}

static bool isbigger(struct list_head *node1, struct list_head *node2)
{
    element_t *ele1 = container_of(node1, element_t, list);
    element_t *ele2 = container_of(node2, element_t, list);
    char *str1 = ele1->value;
    char *str2 = ele2->value;
    if (strcmp(str1, str2) > 0)
        return true;
    else
        return false;
}

static bool issmaller(struct list_head *node1, struct list_head *node2)
{
    element_t *ele1 = container_of(node1, element_t, list);
    element_t *ele2 = container_of(node2, element_t, list);
    char *str1 = ele1->value;
    char *str2 = ele2->value;
    if (strcmp(str1, str2) < 0)
        return true;
    else
        return false;
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head)
        return;
    if (q_size(head) < 2)
        return;
    struct list_head *dirty = head->next->next;
    struct list_head *probe;
    struct list_head *temp;
    bool (*cmp_func)(struct list_head *, struct list_head *);
    if (descend)
        cmp_func = &issmaller;
    else
        cmp_func = &isbigger;
    while (dirty != head) {
        for (probe = dirty->prev; probe != head; probe = probe->prev) {
            if (!cmp_func(probe, dirty))
                break;
        }
        temp = dirty->next;
        list_del(dirty);
        list_add(dirty, probe);
        dirty = temp;
    }
    return;
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;
    if (list_is_singular(head))
        return 1;

    struct list_head *front;
    struct list_head *rear;
    element_t *element;
    bool done = false;
    while (!done) {
        if (q_size(head) <= 1)
            break;
        front = head->next;
        rear = front->next;
        done = true;
        while (rear != head) {
            if (isbigger(front, rear)) {
                list_del(front);
                element = list_entry(front, element_t, list);
                if (element) {
                    free(element->value);
                    free(element);
                }
                done = false;
            }
            front = rear;
            rear = rear->next;
        }
    }
    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;
    if (list_is_singular(head))
        return 1;

    struct list_head *front;
    struct list_head *rear;
    element_t *element;
    bool done = false;
    while (!done) {
        if (q_size(head) <= 1)
            break;
        front = head->next;
        rear = front->next;
        done = true;
        while (rear != head) {
            if (issmaller(front, rear)) {
                list_del(front);
                element = list_entry(front, element_t, list);
                if (element) {
                    free(element->value);
                    free(element);
                }
                done = false;
            }
            front = rear;
            rear = rear->next;
        }
    }
    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    /* Use the concept of merge sort to merge different queues together */

    /* Handle special cases */
    if (!head)
        return 0;
    if (q_size(head) == 0)
        return 0;
    if (q_size(head) == 1)
        return list_entry(head->next, queue_contex_t, chain)->size;

    /* Seperate queue chain into two part, each part might contains multiple
     * queues */
    struct list_head *slow = head->next;
    struct list_head head_left;
    for (struct list_head *fast = head->next;
         fast != head && fast->next != head;
         fast = fast->next->next, slow = slow->next) {
    }
    list_cut_position(&head_left, head, slow->prev);

    /* Perform q_merge on two chain of queues */
    q_merge(&head_left, descend);
    q_merge(head, descend);

    /* Merge two sorted queues on different chains together */
    struct list_head result;
    struct list_head *q_left =
        list_entry(head_left.next, queue_contex_t, chain)->q;
    struct list_head *q_right =
        list_entry(head->next, queue_contex_t, chain)->q;
    bool (*cmp_func)(struct list_head *, struct list_head *);
    if (descend)
        cmp_func = &isbigger;
    else
        cmp_func = &issmaller;
    INIT_LIST_HEAD(&result);
    while (!list_empty(q_left) && !list_empty(q_right)) {
        if (cmp_func(q_left->next, q_right->next)) {
            list_move_tail(q_left->next, &result);
        } else {
            list_move_tail(q_right->next, &result);
        }
    }
    while (!list_empty(q_left))
        list_move_tail(q_left->next, &result);
    while (!list_empty(q_right))
        list_move_tail(q_right->next, &result);
    list_splice(&result, q_left);
    list_entry(head_left.next, queue_contex_t, chain)->size = q_size(q_left);
    list_entry(head->next, queue_contex_t, chain)->size = q_size(q_right);

    /* Merge two chain of queues back to one chain */
    list_splice(&head_left, head);

    return list_entry(head->next, queue_contex_t, chain)->size;
}
