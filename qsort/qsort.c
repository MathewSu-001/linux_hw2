#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct __node 
{
    struct list_head list;
    struct __node *next;
    long value;
} node_t;

node_t *list_tail(node_t **left) {
    while ((*left) && (*left)->next)
        left = &((*left)->next);
    return *left;
}

int list_length(struct list_head *head) 
{
    
    if (!head || list_empty(head))
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

node_t *list_construct(node_t *list, int n)
{
    node_t *node = malloc(sizeof(node_t));
    list_add(&node->list, &list->list);
    node->value = n;
    return list;
}

void list_free(struct list_head *head) 
{

    if (!head)
        return;

    node_t *cur, *next;
    list_for_each_entry_safe (cur, next, head, list) {
        list_del(&cur->list);
        free(&cur->list);
    }
    free(head);
}

/* Verify if list is order */
static bool list_is_ordered(struct list_head *head) 
{
    node_t *cur, *next;
    list_for_each_entry_safe (cur, next, head, list) {
        if (&next->list == head) //last node
            return true;
        if (cur->value > next->value)
            return false;
    }
    return true;
}

/* shuffle array, only work if n < RAND_MAX */
void shuffle(int *array, size_t n) 
{
    if (n <= 0)
        return;

    for (size_t i = 0; i < n - 1; i++) {
        size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
        int t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}

void quick_sort(struct list_head *head) 
{
    int n = list_length(head);
    int value;
    int i = 0;
    // int max_level = 2 * n;
    // node_t *begin[max_level], *end[max_level];
    struct list_head *begin[n];
    struct list_head *result = NULL, *left = NULL, *right = NULL;

    begin[0] = head->next;

    while (i >= 0) {
        struct list_head *L = begin[i];
        if (L != head->prev) {
            struct list_head *pivot = L;
            value = list_entry(pivot, node_t, list)->value;
            struct list_head *p = pivot->next;
            list_del(pivot);

            while (p != head) {
                struct list_head *n = p;
                p = p->next;
                list_add(n , list_entry(n, node_t, list)->value > value ? right : left);
            }

            begin[i] = left;
            begin[i + 1] = pivot;
            begin[i + 2] = right;

            left = right = NULL;
            i += 2;

        } else {
            if (L)
                list_move(L, result);
            i--;
        }
    }
    list_splice(result, head);
}

int main(int argc, char **argv) {
    node_t *list = NULL;

    size_t count = 100000;

    int *test_arr = malloc(sizeof(int) * count);

    for (int i = 0; i < count; ++i)
        test_arr[i] = i;
    shuffle(test_arr, count);

    while (count--)
        list = list_construct(list, test_arr[count]);

    quick_sort(&list);
    assert(list_is_ordered(list));
    list_free(&list->list);

    free(test_arr);

    return 0;
}
