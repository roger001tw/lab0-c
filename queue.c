#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (head) {
        head->next = head;
        head->prev = head;
    }

    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head) {
        return;
    }

    struct list_head *cur, *tmp;
    cur = head->next;
    while (cur != head) {
        tmp = cur->next;
        element_t *e = list_entry(cur, element_t, list);
        q_release_element(e);
        cur = tmp;
    }

    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head || !s) {
        return false;
    }

    element_t *new = malloc(sizeof(element_t));
    if (!new) {
        return false;
    }

    new->value = strdup(s);
    if (!new->value) {
        free(new);
        return false;
    }

    list_add(&new->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head || !s) {
        return false;
    }

    element_t *new = malloc(sizeof(element_t));
    if (!new) {
        return false;
    }

    new->value = strdup(s);
    if (!new->value) {
        free(new);
        return false;
    }

    list_add_tail(&new->list, head);
    return true;
}

static element_t *remove_element(struct list_head *node,
                                 char *sp,
                                 size_t bufsize)
{
    element_t *e = list_entry(node, element_t, list);
    if (sp != NULL && e != NULL && e->value != NULL && bufsize > 1) {
        strncpy(sp, e->value, bufsize);
    }

    list_del(node);
    return e;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head) {
        return NULL;
    }

    return remove_element(head->next, sp, bufsize);
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head) {
        return NULL;
    }

    return remove_element(head->prev, sp, bufsize);
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    int count = 0;
    struct list_head *cur;

    if (!head) {
        return 0;
    }

    cur = head->next;
    while (cur != head) {
        count++;
        cur = cur->next;
    }

    return count;
}

static element_t *remove_element_from_index(struct list_head *head, int index)
{
    int i;
    struct list_head *cur = head->next;

    for (i = 0; i < index; i++) {
        cur = cur->next;
    }

    list_del(cur);
    return list_entry(cur, element_t, list);
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (head == NULL) {
        return false;
    }

    int list_size = q_size(head);
    if (list_size <= 0) {
        return false;
    }

    element_t *e = remove_element_from_index(head, list_size / 2);
    q_release_element(e);
    return true;
}

#define GET_ELEMENT_VALUE(node) (list_entry(node, element_t, list)->value)
#define DELETE_NODE(node)                                    \
    do {                                                     \
        struct list_head *tmp = node;                        \
        list_del(tmp);                                       \
        q_release_element(list_entry(tmp, element_t, list)); \
    } while (0)

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    struct list_head *node, *safe;
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/

    if (head == NULL || list_empty(head)) {
        return false;
    }

    bool is_dup = false;
    list_for_each_safe(node, safe, head) {
        printf("node: %s\n", GET_ELEMENT_VALUE(node));
        if (node->prev == head) {
            continue;
        }

        if (strcmp(GET_ELEMENT_VALUE(node->prev), GET_ELEMENT_VALUE(node)) ==
            0) {
            is_dup = true;
            DELETE_NODE(node->prev);
            if (node->next == head) {
                list_del(node);
                q_release_element(list_entry(node, element_t, list));
            }
        } else {
            if (is_dup == true) {
                is_dup = false;
                DELETE_NODE(node->prev);
            }
        }
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return;
    }

    struct list_head *cur = head->next;
    struct list_head *tmp;
    while (cur != head) {
        tmp = cur->next;
        list_move(cur, head);
        cur = tmp;
    }
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
