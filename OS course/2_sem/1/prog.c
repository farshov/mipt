#include <stdio.h>
#include <stdlib.h>
#include "prog.h"

#define ERR_OK 0
#define ERR_FAIL -1

array * array_init()
{
    array * res = calloc(1, sizeof(*res));
    if(res == NULL)
    {
        return NULL;
    }
    res->cap = 10;
    res->head = calloc(res->cap, sizeof(int));
    if(res->head == NULL)
    {
        free(res);
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
    else 
    {
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
        my_realloc(a, a->cap * 2);
        if(new_head == NULL)
        {
            return ERR_FAIL;
        }
        a->cap = a->cap * 2;
        a->head = new_head;
        *(a->head + a->content) = num;
        a->content += 1;
        return ERR_OK;
    }
}

int pop_back(array * a, int * value)
{
    if(!array_is_empty(a))
    {
        *value = * (a->head + (a->content - 1));
        a->content -= 1;
        return ERR_OK;
    }
    else
    {
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

int array_size(array * a)
{
    return a->content;
}

int change_n_elem(array * a, int n, int x)
{
    if(n < a->content)
    {
        *(a->head + n) = x;
        return ERR_OK;
    }
    else 
    {
        return ERR_FAIL;
    }
}

int array_clear(array * a)
{
    a->content = 0;
    return ERR_OK;
}