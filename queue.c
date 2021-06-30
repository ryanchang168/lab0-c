#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */

list_ele_t *merge_list(list_ele_t *node);

list_ele_t *merge_sort(list_ele_t *l, list_ele_t *r);

queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q) {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
    }
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q) {
        return;
    }

    list_ele_t *tmp = q->head;
    while (tmp) {
        tmp = q->head->next;
        free(q->head->value);
        free(q->head);
        q->head = tmp;
    }

    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }

    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        return false;
    }

    newh->value = NULL;
    newh->next = NULL;

    size_t len = strlen(s) + 1;
    newh->value = malloc(len * sizeof(char));
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, len);
    newh->next = q->head;
    q->head = newh;

    if (!q->tail) {
        q->tail = newh;
        q->tail->next = NULL;
    }
    q->size++;

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }

    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (!newt) {
        return false;
    }

    newt->value = NULL;
    newt->next = NULL;

    size_t len = strlen(s) + 1;
    newt->value = malloc(len * sizeof(char));
    if (newt->value == NULL) {
        free(newt);
        return false;
    }
    strncpy(newt->value, s, len);
    if (!q->head) {
        q->head = newt;
    } else {
        q->tail->next = newt;
    }
    q->tail = newt;
    q->size++;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head || !sp) {
        return false;
    }


    strncpy(sp, q->head->value, bufsize - 1);
    sp[bufsize - 1] = '\0';

    list_ele_t *tmp = q->head;
    if (q->head == q->tail) {
        q->tail = NULL;
    }
    q->head = q->head->next;
    q->size--;

    free(tmp->value);
    free(tmp);

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q || !q->head) {
        return 0;
    }
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head) {
        return;
    }

    list_ele_t *prev = q->head, *cur = q->head->next;
    q->tail = q->head;

    while (cur) {
        q->head = cur;
        cur = cur->next;
        q->head->next = prev;
        prev = q->head;
    }
    q->tail->next = NULL;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->size) {
        return;
    }
    q->head = merge_list(q->head);
    q->tail = q->head;
    while (q->tail->next) {
        q->tail = q->tail->next;
    }
}

list_ele_t *merge_list(list_ele_t *node)
{
    if (!node || !node->next) {
        return node;
    }

    list_ele_t *slow = node, *fast = node->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;

    list_ele_t *l = merge_list(node), *r = merge_list(fast);
    return merge_sort(l, r);
}

list_ele_t *merge_sort(list_ele_t *l, list_ele_t *r)
{
    list_ele_t tmp;
    list_ele_t *node = &tmp;

    tmp.next = NULL;

    while (l && r) {
        if (strcmp(l->value, r->value) < 0) {
            node->next = l;
            node = node->next;
            l = l->next;
        } else {
            node->next = r;
            node = node->next;
            r = r->next;
        }
    }

    if (l) {
        node->next = l;
    } else {
        node->next = r;
    }

    return tmp.next;
}
