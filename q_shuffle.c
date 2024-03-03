#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "q_shuffle.h"

/* Shuffle queue using Fisher–Yates shuffle */
void q_shuffle(struct list_head *head)
{
    int range = q_size(head);
    struct list_head *tmp;
    while (range) {
        int cnt = rand() % range;
        tmp = head->next;
        while (cnt--) {
            tmp = tmp->next;
        }
        list_move_tail(tmp, head);
        range--;
    }
    return;
}