#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    list_ele_t *free_ptr = q->head;
    list_ele_t *next_ptr = NULL;
    while (NULL != free_ptr) {
        next_ptr = free_ptr->next;
        free(free_ptr->value);
        free(free_ptr);
        free_ptr = next_ptr;
        next_ptr = NULL;
    }
    /* Free queue structure */
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
    if (!q)
        return false;
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    /* Process string */
    size_t length = strlen(s) + 1;
    newh->value = (char *) malloc(length * sizeof(char));
    if (!newh->value) {
        free(newh);
        return false;
    }
    memcpy(newh->value, s, length);

    /* Process link */
    newh->next = q->head;
    q->head = newh;
    /* First insert element is both tail and head */
    if (0 == q->size)
        q->tail = newh;

    /* Update size*/
    q->size += 1;

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
    if (!q)
        return false;
    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    if (!newt)
        return false;

    /* Process string */
    size_t length = strlen(s) + 1;
    newt->value = (char *) malloc(length * sizeof(char));
    if (!newt->value) {
        free(newt);
        return false;
    }
    memcpy(newt->value, s, length);

    /* Process link */
    newt->next = NULL;
    q->tail->next = newt;
    q->tail = newt;
    /* First insert element is both tail and head */
    if (0 == q->size)
        q->tail = newt;

    /* Update size */
    q->size += 1;

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
    if (!q)
        return false;
    if (0 == q->size)
        return false;

    /* Process string */
    size_t length = strlen(q->head->value) + 1;
    if (bufsize < length) {
        memcpy(sp, q->head->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    } else {
        memcpy(sp, q->head->value, length);
    }
    free(q->head->value);

    /* Process link */
    list_ele_t *free_ptr = q->head;
    q->head = q->head->next;
    /* Latest remove element is both tail and head */
    if (1 == q->size)
        q->tail = NULL;
    free(free_ptr);

    /* Update size */
    q->size -= 1;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q)
        return 0;
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
    if (!q)
        return;
    if (0 == q->size)
        return;
    list_ele_t *tmp_ptr = q->head;
    list_ele_t *pre_ptr = NULL;
    while (tmp_ptr) {
        q->tail = tmp_ptr->next;
        tmp_ptr->next = pre_ptr;
        pre_ptr = tmp_ptr;
        tmp_ptr = q->tail;
    }
    q->tail = q->head;
    q->head = pre_ptr;
    return;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q)
        return;
    if (0 == q->size)
        return;
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
}
