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
    /* Handle empty queue */
    if (0 != q->size)
        q->tail->next = newt;
    else
        q->head = newt;
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

    if (sp) {
        /* Process string */
        size_t length = strlen(q->head->value) + 1;
        if (bufsize < length) {
            memcpy(sp, q->head->value, bufsize - 1);
            sp[bufsize - 1] = '\0';
        } else {
            memcpy(sp, q->head->value, length);
        }
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

void create(list_ele_t *heap[], int size);
void down_adjust(list_ele_t *heap[], int i, int size);

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
    int step_count = q->size;
    list_ele_t *heap[q->size + 1];
    list_ele_t *tmp_ptr = q->head;
    for (int i = 1; i <= q->size; i++) {
        heap[i] = tmp_ptr;
        tmp_ptr = tmp_ptr->next;
    }

    create(heap, q->size);

    // sorting
    while (step_count > 1) {
        // swap heap[1] and heap[last]
        int last = step_count;
        tmp_ptr = heap[1];
        heap[1] = heap[last];
        heap[last] = tmp_ptr;
        step_count--;
        down_adjust(heap, 1, step_count);
    }

    // reconnect link of q
    q->head = heap[1];
    tmp_ptr = q->head;
    for (int i = 2; i <= q->size; i++) {
        tmp_ptr->next = heap[i];
        tmp_ptr = tmp_ptr->next;
    }
    tmp_ptr->next = NULL;
    q->tail = tmp_ptr;
    return;
}
void create(list_ele_t *heap[], int size)
{
    for (int i = size / 2; i >= 1; i--)
        down_adjust(heap, i, size);
}

void down_adjust(list_ele_t *heap[], int i, int size)
{
    int flag = 1;
    list_ele_t *temp;
    while (2 * i <= size && flag == 1) {
        int j = 2 * i;  // j points to left child
        if (j + 1 <= size && strcmp(heap[j + 1]->value, heap[j]->value) > 0)
            j = j + 1;
        if (strcmp(heap[i]->value, heap[j]->value) > 0)
            flag = 0;
        else {
            temp = heap[i];
            heap[i] = heap[j];
            heap[j] = temp;
            i = j;
        }
    }
}
