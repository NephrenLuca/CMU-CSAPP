/**
 * @file queue.c
 * @brief Implementation of a queue that supports FIFO and LIFO operations.
 *
 * This queue implementation uses a singly-linked list to represent the
 * queue elements. Each queue element stores a string value.
 *
 * Assignment for basic C skills diagnostic.
 *
 * TODO: fill in your name and Student ID
 * @author Yan Zheng <23300240012@m.fudan.edu.cn>
 */

#include "queue.h"
#include "harness.h"

#include <stdlib.h>
#include <string.h>

/**
 * @brief Allocates a new queue
 * @return The new queue, or NULL if memory allocation failed
 */
queue_t *queue_new(void) {
    queue_t *q = malloc(sizeof(queue_t));
    /* What if malloc returned NULL? */
    if (q == NULL) {
        free(q);
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->queueSize = 0;
    return q;
}

/**
 * @brief Frees all memory used by a queue
 * @param[in] q The queue to free
 */

static void freePtr(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    free(ptr);
    ptr = NULL;
}

static void freeNode(list_ele_t *node) {
    freePtr(node->value);
    // freePtr(node->next);
    freePtr(node);
}

static void queueFreeHelper(list_ele_t *head) {
    list_ele_t *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        freeNode(tmp);
    }
}

void queue_free(queue_t *q) {
    /* How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (q == NULL) {
        return;
    }
    queueFreeHelper(q->head);
    freePtr(q);
}

/**
 * @brief Attempts to insert an element at head of a queue
 *
 * This function explicitly allocates space to create a copy of `s`.
 * The inserted element points to a copy of `s`, instead of `s` itself.
 *
 * @param[in] q The queue to insert into
 * @param[in] s String to be copied and inserted into the queue
 *
 * @return true if insertion was successful
 * @return false if q is NULL, or memory allocation failed
 */
bool queue_insert_head(queue_t *q, const char *s) {
    list_ele_t *newh;
    /* What should you do if the q is NULL? */
    if (q == NULL) {
        return false;
    }
    newh = malloc(sizeof(list_ele_t));
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    if (newh == NULL) {
        free(newh);
        return false;
    }
    newh->value = malloc(sizeof(s) * (strlen(s) + 1));
    if (newh->value == NULL) {
        freeNode(newh);
        return false;
    }
    strcpy(newh->value, s);
    q->queueSize++;
    newh->next = q->head;
    q->head = newh;
    if (q->queueSize == 1) {
        q->tail = q->head;
    }
    return true;
}

/**
 * @brief Attempts to insert an element at tail of a queue
 *
 * This function explicitly allocates space to create a copy of `s`.
 * The inserted element points to a copy of `s`, instead of `s` itself.
 *
 * @param[in] q The queue to insert into
 * @param[in] s String to be copied and inserted into the queue
 *
 * @return true if insertion was successful
 * @return false if q is NULL, or memory allocation failed
 */
bool queue_insert_tail(queue_t *q, const char *s) {
    /* You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    if (q == NULL) {
        return false;
    }
    if (q->queueSize == 0) {
        return queue_insert_head(q, s);
    }
    list_ele_t *node = malloc(sizeof(list_ele_t));
    if (node == NULL) {
        free(node);
        return false;
    }
    node->next = NULL;
    node->value = malloc(sizeof(s) * (strlen(s) + 1));
    if (node->value == NULL) {
        freeNode(node);
        return false;
    }
    strcpy(node->value, s);
    q->queueSize++;
    q->tail->next = node;
    q->tail = node;
    return true;
}

/**
 * @brief Attempts to remove an element from head of a queue
 *
 * If removal succeeds, this function frees all memory used by the
 * removed list element and its string value before returning.
 *
 * If removal succeeds and `buf` is non-NULL, this function copies up to
 * `bufsize - 1` characters from the removed string into `buf`, and writes
 * a null terminator '\0' after the copied string.
 *
 * @param[in]  q       The queue to remove from
 * @param[out] buf     Output buffer to write a string value into
 * @param[in]  bufsize Size of the buffer `buf` points to
 *
 * @return true if removal succeeded
 * @return false if q is NULL or empty
 */
static size_t min(size_t a, size_t b) {
    return a < b ? a : b;
}

bool queue_remove_head(queue_t *q, char *buf, size_t bufsize) {
    /* You need to fix up this code. */
    if (q == NULL || q->head == NULL) {
        return false;
    }
    list_ele_t *nowHead = q->head;
    if (buf != NULL) {
        size_t len = min(bufsize - 1, strlen(nowHead->value));
        for (size_t i = 0; i < len; i++) {
            buf[i] = nowHead->value[i];
        }
        buf[len] = '\0';
    }
    q->head = q->head->next;
    freeNode(nowHead);
    q->queueSize--;
    return true;
}

/**
 * @brief Returns the number of elements in a queue
 *
 * This function runs in O(1) time.
 *
 * @param[in] q The queue to examine
 *
 * @return the number of elements in the queue, or
 *         0 if q is NULL or empty
 */
size_t queue_size(queue_t *q) {
    /* You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    if (q == NULL) {
        return 0;
    }
    return q->queueSize;
}

/**
 * @brief Reverse the elements in a queue
 *
 * This function does not allocate or free any list elements, i.e. it does
 * not call malloc or free, including inside helper functions. Instead, it
 * rearranges the existing elements of the queue.
 *
 * @param[in] q The queue to reverse
 */
void queue_reverse(queue_t *q) {
    /* You need to write the code for this function */
    if (q == NULL) {
        return;
    }
    if (q->queueSize < 2) {
        return;
    }
    list_ele_t *pre = q->head;
    list_ele_t *node = q->head->next;
    while (node != NULL) {
        list_ele_t *nxt = node->next;
        node->next = pre;
        pre = node;
        node = nxt;
    }
    q->head->next = NULL;
    q->tail = q->head;
    q->head = pre;
}
