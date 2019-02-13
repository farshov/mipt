#ifndef HEADER_H
#define HEADER_H

typedef struct array {
    int * head;
    int cap;
    int content;
} array;

array* array_init();
void array_destructor(array* a);
int array_is_empty(array * a);
int * array_n_elem(array * a, int n);
int * array_front(array * a);
int * array_back(array * a);
int push_back(array * a, int num);
int pop_back(array * a);
int for_each(array * a, int (*act)(int * obj, void* ctx), void* ctx);
void test_func();
void test_for_each();

#endif