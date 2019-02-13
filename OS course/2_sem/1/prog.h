#ifndef HEADER_H
#define HEADER_H

#define my_realloc(a, new_size) \
do {                                    \
    int * new_head = realloc(a->head, new_size)                                  \
    if (new_head == NULL)  \
        return ERR_FAIL;    \
    a->head = new_head;     \
    a->cap = new_size;      \
} while (0)   


typedef struct array {
    int * head;
    int cap;
    int content;
} array;

array* array_init(); // инициализирует массив
void array_destructor(array* a);  // уничтожает массив
int array_is_empty(array * a);  // проверка на наличие элементов в массиве: если пуст, возвращает единицу, иначе - 0
int * array_n_elem(array * a, int n);  // возвращает указатель на n-ый элемент, если таковой имеется
int * array_front(array * a);  // возвращает указатель на первый элемент, если таковой имеется
int * array_back(array * a);  // возвращает указатель на последний элемент, если таковой имеется
int push_back(array * a, int num);  // кладет элемент num в конец массива a
int pop_back(array * a, int * value); // извлекает последний элемент из a и кладет по адресу value
int array_size(array * a);  // возвращает число элементов в массиве
int array_clear(array * a);  // удаляет все элементы из массива, при этом массив продолжает существовать
int change_n_elem(array * a, int n, int x);  // в массиве а заменяет элемент под индексом n на элемент х
int for_each(array * a, int (*act)(int * obj, void* ctx), void* ctx);  // исполнение функции act для каждого элемента a

void test_func(); //функция тестирования
void test_for_each();  //функция тестирования

#endif