#ifndef LAB0_Q_SHUFFLE_H
#define LAB0_Q_SHUFFLE_H

#include <stdbool.h>
#include <stddef.h>

#include "harness.h"
#include "list.h"
#include "queue.h"

/**
 * q_shuffle() - Shuffle queue using Fisher–Yates shuffle
 * @head: header of queue
 */
void q_shuffle(struct list_head *head);

#endif /* LAB0_Q_SHUFFLE_H */