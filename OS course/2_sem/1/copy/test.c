#include <stdio.h>
#include <stdlib.h>
#include "prog.h"

#define ERR_OK 0
#define ERR_FAIL -1

int main()
{
    test_func();
    test_for_each();
}

int elevate_elem(int * elem, void * n)
{
    int num = *((int *) n);
    int i = 0;
    int res = 1;
    for(i = 0; i < num; i++)
    {
        res = res * (*elem);
    }
    *elem = res;
    return ERR_OK;
}

void test_for_each()
{
    printf("\n____________ Start of test for each\n");

    array * a = array_init();
 
    int i = 0;
    for(i = 0; i < 5; i++)
    {
        push_back(a, i);
    }

    int num = 2;
    for_each(a, &elevate_elem, &num);

    for(i = 0; i < 5; i++)
    {
        printf("%d\n", pop_back(a));
    }
    
    array_destructor(a);

    printf("\n____________ End of test for each\n");
}

void test_func()
{
    printf("\n____________ Start of test func\n");

    array * a = array_init();
    printf("array is empty : %d\n", array_is_empty(a));
    array_front(a);
    array_back(a);
    array_n_elem(a, 3);

    pop_back(a);

    int i = 0;
    for(i = 0; i < 15; i++)
    {
        push_back(a, i);
    }

    array_n_elem(a, 20);
    printf("array 5-elem : %d\n", *array_n_elem(a, 5));

    printf("array is empty : %d\n", array_is_empty(a));

    printf("array front : %d\n", *array_front(a));
    printf("array back : %d\n", *array_back(a));


    for(i = 0; i < 14; i++)
    {
        printf("%d\n", i);
        pop_back(a);
    }

    array_destructor(a);
    printf("\n____________ End of test func\n");
}

