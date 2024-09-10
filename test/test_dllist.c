#include "test/test.h"
#include "src/dllist.h"
#include "src/complex.h"

#include <assert.h>

void test_dllist_create(void);
void test_dllist_insert_front(void);
void test_dllist_pop_back(void);

void test_dllist(void) {
    test_dllist_create();
    test_dllist_insert_front();
    test_dllist_pop_back();
}

void test_dllist_create() {
    DLList* list = dllist_create();
    
    assert(list->head == NULL);
    assert(list->tail == NULL);
    assert(list->size == 0);

    dllist_free(list);
}

void test_dllist_insert_front() {
    DLList* list = dllist_create();

    Complex c1 = {1, 0};
    Complex c2 = {2, 0};
    Complex c3 = {3, 0};

    dllist_insert_front(list, c1);

    assert(complex_equal(list->head->c, c1));
    assert(complex_equal(list->tail->c, c2));
    assert(list->size == 1);

    dllist_insert_front(list, c2);
    
    assert(complex_equal(list->head->c, c2));
    assert(complex_equal(list->tail->c, c1));
    assert(list->size == 2);

    dllist_insert_front(list, c3);

    assert(complex_equal(list->head->c, c3));
    assert(complex_equal(list->tail->c, c1));
    assert(list->size == 3);

    // forward
    DLLNode* n = list->head;
    assert(complex_equal(n->c, c3));
    n = n->succ;
    assert(complex_equal(n->c, c2));
    n = n->succ;
    assert(complex_equal(n->c, c1));
    assert(n->succ == NULL);

    //backward
    n = n->prev;
    assert(complex_equal(n->c, c2));
    n = n->prev;
    assert(complex_equal(n->c, c3));
    assert(n->prev = NULL);

    dllist_free(list);
}

void test_dllist_pop_back() {
    DLList* list = dllist_create();

    Complex c1 = {1, 0};
    Complex c2 = {2, 0};
    Complex c3 = {3, 0};

    dllist_insert_front(list, c1);
    dllist_insert_front(list, c2);
    dllist_insert_front(list, c3);

    assert(list->size == 3);
    assert(complex_equal(list->tail->c, c1));

    dllist_pop_back(list);

    assert(list->size == 2);
    assert(complex_equal(list->tail->c, c2));

    dllist_pop_back(list);

    assert(list->size == 1);
    assert(complex_equal(list->tail->c, c3));
    assert(list->tail == list->head);

    dllist_pop_back(list);
    
    assert(list->size == 0);
    assert(list->head == list->tail);
    assert(list->tail == NULL);

    dllist_free(list);
}
