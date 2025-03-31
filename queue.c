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
        snprintf(sp, bufsize, "%s", e->value);
    }

    list_del(node);
    return e;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }

    return remove_element(head->next, sp, bufsize);
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
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
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    struct list_head *node, *safe;

    if (head == NULL || list_empty(head)) {
        return false;
    }

    bool is_dup = false;
    list_for_each_safe(node, safe, head) {
        if (node->prev == head) {
            continue;
        }

        if (strcmp(GET_ELEMENT_VALUE(node->prev), GET_ELEMENT_VALUE(node)) ==
            0) {
            is_dup = true;
            DELETE_NODE(node->prev);
            if (node->next == head) {
                DELETE_NODE(node);
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
    if (!head || list_empty(head)) {
        return;
    }

    struct list_head *node;
    list_for_each(node, head) {
        if (node->next == head) {
            break;
        }

        list_move(node->next, node->prev);
    }
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
    if (!head || list_empty(head) || k <= 1) {
        return;
    }

    int list_size = q_size(head);
    if (list_size < k) {
        return;
    }

    int rever_count = list_size / k, i, j;
    for (i = 0; i < rever_count; i++) {
        struct list_head *cur = head->next;
        for (j = 0; j < k - 1; j++) {
            list_move(cur->next, head);
        }

        head = cur;
    }
}

#if 0
// Debugging function to print the list
static void print_list_value(const char* tag, struct list_head *head)
{
    element_t *item = NULL;
    printf("%s: ", tag);
    list_for_each_entry(item, head, list) {
        printf("%s ", item->value);
    }

    printf("\n");
}
#endif

static void split_list(struct list_head *head, struct list_head *list)
{
    if (!head || head->next == head) {
        list->next = list->prev = list;
        return;
    }

    struct list_head *slow = head, *fast = head;
    while (fast->next != head && fast->next->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // list
    list->next = slow->next;
    list->prev = head->prev;
    list->next->prev = list;
    list->prev->next = list;

    // head
    slow->next = head;
    head->prev = slow;
}

static struct list_head *merge_two_sorted_list(struct list_head *head1,
                                               struct list_head *head2,
                                               bool descend)
{
    if (head1 == NULL || head2 == NULL) {
        return head1;
    }

    struct list_head *L = head1->next, *R = head2->next, *cur = head1;
    while (L != head1 && R != head2) {
        if (strcmp(GET_ELEMENT_VALUE(L), GET_ELEMENT_VALUE(R)) <= 0) {
            cur->next = L;
            L->prev = cur;
            cur = L;
            L = L->next;
        } else {
            cur->next = R;
            R->prev = cur;
            cur = R;
            R = R->next;
        }
    }

    if (L == head1) {
        cur->next = R;
        cur->next->prev = cur;
        head2->prev->next = head1;
        head1->prev = head2->prev;
    } else {
        cur->next = L;
        cur->next->prev = cur;
    }

    head2->next = head2->prev = head2;
    return head1;
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    struct list_head list, *plist;
    if (!head || list_empty(head) || head->next->next == head) {
        return;
    }

    plist = &list;
    split_list(head, plist);
    q_sort(head, descend);
    q_sort(plist, descend);
    merge_two_sorted_list(head, plist, descend);
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head)) {
        return 0;
    }

    const char *min = NULL;
    struct list_head *cur = head->next, *next;
    while (cur != head) {
        const element_t *e = list_entry(cur, element_t, list);
        next = cur->next;

        if (min == NULL || strcmp(e->value, min) < 0) {
            min = e->value;
        } else {
            DELETE_NODE(cur);
        }

        cur = next;
    }

    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head)) {
        return 0;
    }

    const char *max = NULL;
    struct list_head *cur = head->prev, *prev;

    while (cur != head) {
        const element_t *e = list_entry(cur, element_t, list);
        prev = cur->prev;

        if (max == NULL || strcmp(e->value, max) > 0) {
            max = e->value;
        } else {
            DELETE_NODE(cur);
        }

        cur = prev;
    }

    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    queue_contex_t *qc;
    struct list_head *head_list[100];
    int head_count = 0;
    int qsize = 0;

    list_for_each_entry(qc, head, chain) {
        head_list[head_count++] = qc->q;
    }

    for (int interval = 1; interval < head_count; interval *= 2) {
        for (int i = 0; i + interval < head_count; i += interval * 2) {
            head_list[i] = merge_two_sorted_list(
                head_list[i], head_list[i + interval], descend);
        }
    }

    qsize = q_size(head_list[0]);
    queue_contex_t *qc1 = list_entry(head->next, queue_contex_t, chain);
    qc1->q = head_list[0];
    head_list[0]->prev->next = qc1->q;
    return qsize;
}
