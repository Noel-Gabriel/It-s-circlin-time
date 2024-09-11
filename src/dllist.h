#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include "src/complex.h"
#include <stdlib.h>

/** A node in the doubly linked list. */
typedef struct DLLNode {
    Complex c; 
    struct DLLNode* succ;
    struct DLLNode* prev;
} DLLNode;

/** Simple doubly linked list. */
typedef struct DLList {
    struct DLLNode* head;
    struct DLLNode* tail;
    size_t size;
} DLList;

/**
 * Creates and allocates a new list.
 */
DLList* dllist_create(void);

/**
 * Inserts the element at the front of the list.
 */
void dllist_insert_front(DLList* list, Complex c);

/**
 * Pops the element at the back of the list.
 */
void dllist_pop_back(DLList* list);

/**
 * Frees all allocated memory of the linked list.
 */
void dllist_free(DLList* list);

#endif
