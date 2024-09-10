#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include "src/complex.h"
#include <stdlib.h>

typedef struct DLLNode {
    Complex c; 
    struct DLLNode* succ;
    struct DLLNode* prev;
} DLLNode;

typedef struct DLList {
    struct DLLNode* head;
    struct DLLNode* tail;
    size_t size;
} DLList;

DLList* dllist_create(void);
void dllist_insert_front(DLList* list, Complex c);
void dllist_pop_back(DLList* list);
void dllist_free(DLList* list);

#endif
