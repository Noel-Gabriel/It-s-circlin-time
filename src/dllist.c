#include "src/dllist.h"

DLList* dllist_create(void) {
    DLList* list = malloc(sizeof(DLList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void dllist_insert_front(DLList* list, Complex c) {
    DLLNode* node = malloc(sizeof(DLLNode));
    node->c = c;
    node->prev = NULL;
    node->succ = NULL;
    
    if(list->size == 0) {
        list->head = node;
        list->tail = node;
    } else {
        node->succ = list->head;
        list->head->prev = node;
        list->head = node;
    }
    ++(list->size);
}

void dllist_pop_back(DLList* list) {
    if(list->size == 0) { return; }
    DLLNode* to_remove = list->tail;
    if(list->size == 1) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->tail = to_remove->prev;
        list->tail->succ = NULL;
    }
    --(list->size);
    free(to_remove);
}

void dllist_free(DLList* list) {
    DLLNode* to_remove = list->head;
    while(to_remove != NULL) {
        DLLNode* next = to_remove->succ;
        free(to_remove);
        to_remove = next;
    }
    free(list);
}
