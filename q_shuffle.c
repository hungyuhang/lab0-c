#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "q_shuffle.h"

/* Shuffle queue using Fisher–Yates shuffle */
void q_shuffle(struct list_head *head)
{
    srand(time(NULL));
    int range = q_size(head);
    struct list_head *tmp;
    while (range) {
        int cnt = rand() % range;
        if (cnt != 0) {
            tmp = head;
            while (cnt--) {
                tmp = tmp->next;
            }
            list_move_tail(tmp, head);
        }
        range--;
    }
    return;
}