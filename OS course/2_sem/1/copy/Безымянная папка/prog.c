#include <stdio.h>
#include <stdlib.h>
#include "prog.h"

#define ERR_OK 0
#define ERR_FAIL -1
#define LIM_ERR 1
#define MEM_ERR 2

void return_error(int err_type)
{
    if(err_type == LIM_ERR)
    {
        fprintf(stderr, "going beyond the limits\n");
    }
    if(err_type == MEM_ERR)
    {
        fprintf(stderr, "memory error\n");
    }
}

array* array_init()
{
    array * res = calloc(1, sizeof(*res));
    if(res == NULL)
    {
        fprintf(stderr, "calloc_1 failed in array_init\n");
        return NULL;
    }
    res->cap = 10;
    res->head = calloc(res->cap, sizeof(int));
    if(res == NULL)
    {
        fprintf(stderr, "calloc_2 failed in array_init\n");
        return NULL;
    }
    res->content = 0;
    return res;
}

int array_is_empty(array * a)
{
    if(a->content == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void array_destructor(array * a)
{
    free(a->head);
    free(a);
}

int * array_n_elem(array * a, int n)
{   
    if(n < a->content)
    {
        int * res = (a->head + n);
        return res;
    }
    else if(array_is_empty(a))
    {
        fprintf(stderr, "try to get n-elem from empty array\n");
        return NULL;
    }
    else
    {
        fprintf(stderr, "going beyond the bounds of the array in n_elem\n");
        return NULL;
    }
}

int * array_front(array * a)
{
    if(!array_is_empty(a))
    {
        int * res = (a->head);
        return res;
    }
    else
    {
        fprintf(stderr, "array is empty in array_front\n");
        return NULL;
    }
}

int * array_back(array * a)
{
    if(!array_is_empty(a))
    {
        int * res = (a->head + (a->content - 1));
        return res;
    }
    else
    {
        fprintf(stderr, "array is empty in array_back\n");
        return NULL;
    }
}

int push_back(array * a, int num)
{
    if(a->content < a->cap)
    {
        *(a->head + a->content) = num;
        a->content += 1;
        return ERR_OK;
    }
    else
    {
        a->cap = a->cap * 2;
        int * new_head = realloc(a->head, a->cap * sizeof(int));
        if(new_head == NULL)
        {
            fprintf(stderr, "realloc failed in push_back\n");
            return ERR_FAIL;
        }
        a->head = new_head;
        *(a->head + a->content) = num;
        a->content += 1;
        return ERR_OK;
    }
}

int pop_back(array * a)
{
    int num;
    if(!array_is_empty(a))
    {
        num = * (a->head + (a->content - 1));
        a->content -= 1;
        return num;
    }
    else
    {
        fprintf(stderr, "pop from empty array\n");
        return ERR_FAIL;
    }
}

int for_each(array * a, int (*act)(int * obj, void* ctx), void* ctx)
{
    int i = 0;
    for(i = 0; i < a->content; i++)
    {
        act(a->head + i, ctx); 
    }
    return ERR_OK;
}
