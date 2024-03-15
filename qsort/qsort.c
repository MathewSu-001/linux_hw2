#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct __node 
{
    struct list_head list;
    //struct __node *next;
    int value;
} node_t;

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

void list_construct(struct list_head *head, int n)
{
    node_t *node = malloc(sizeof(node_t));
    node->value = n;
    list_add(&node->list, head);
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
            break;
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
    for (int j = 0; j < n; j++){
        begin[j] = malloc(sizeof(struct list_head));
        INIT_LIST_HEAD(begin[j]);
    }
    LIST_HEAD(result);
    LIST_HEAD(left);
    LIST_HEAD(right);

    list_splice(head, begin[0]);

    while (i >= 0) {
        if (!list_is_singular(begin[i])) {
            node_t *pivot = list_first_entry(begin[i], node_t, list);
            value = pivot->value;
            list_del(&pivot->list);
            
            node_t *cur, *next;
            list_for_each_entry_safe (cur, next, head, list) {
                list_move(&cur->list, cur->value > value ? &right : &left);
            }

            list_splice(&left, begin[i]);
            list_splice(&pivot->list, begin[i + 1]);
            list_splice(&right, begin[i + 2]);

            INIT_LIST_HEAD(&left);
            INIT_LIST_HEAD(&right);
            i += 2;

        } else {
            if (!begin[i] || !list_empty(begin[i])){
                node_t *node = list_first_entry(begin[i], node_t, list);
                list_move(&node->list, &result);
            }
            i--;
        }
    }
    list_splice(&result, head);
}

int main(int argc, char **argv) {
    
    LIST_HEAD(head);
    size_t count = 10;

    int *test_arr = malloc(sizeof(int) * count);

    for (int i = 0; i < count; ++i)
        test_arr[i] = i;
    shuffle(test_arr, count);

    while (count--)
        list_construct(&head, test_arr[count]);

    quick_sort(&head);
    assert(list_is_ordered(&head));
    list_free(&head);

    free(test_arr);

    return 0;
}
