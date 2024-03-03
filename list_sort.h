#ifndef LAB0_LIST_SORT_H
#define LAB0_LIST_SORT_H

#include <stdbool.h>
#include <stddef.h>
#include "harness.h"
#include "list.h"
#include "queue.h"

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

typedef int
    __attribute__((nonnull(2, 3))) (*list_cmp_func_t)(void *,
                                                      const struct list_head *,
                                                      const struct list_head *);

__attribute__((nonnull(2, 3))) void list_sort(void *priv,
                                              struct list_head *head,
                                              list_cmp_func_t cmp);

/**
 * q_ksort() - Sort elements of queue in ascending/descending order using Linux
 * library
 * @head: header of queue
 * @descend: whether or not to sort in descending order
 *
 * No effect if queue is NULL or empty. If there has only one element, do
 * nothing.
 */
void q_ksort(struct list_head *head, bool descend);

#endif /* LAB0_LIST_SORT_H */