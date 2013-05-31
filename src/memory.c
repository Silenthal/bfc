#include <stdlib.h>
#include "memory.h"

typedef struct MemCell mc_link_t;

struct MemCell
{
    uint8_t data;
    mc_link_t* prev;
    mc_link_t* next;
};

static mc_link_t* current = NULL;

mc_link_t* bf_mem_create()
{
    mc_link_t* ret = malloc(sizeof(mc_link_t));
    ret->next = NULL;
    ret->prev = NULL;
    ret->data = 0;
    return ret;
}

void bf_mem_free(mc_link_t* inp)
{
    inp->next = NULL;
    inp->prev = NULL;
    free(inp);
}

void bf_mem_init()
{
    if (current == NULL)
    {
        current = bf_mem_create();
    }
}

void bf_mem_inc()
{
    if (current == NULL)
    {
        bf_mem_init();
    }
    ++current->data;
}

void bf_mem_dec()
{
    if (current == NULL)
    {
        bf_mem_init();
    }
    --current->data;
}

void bf_mem_left()
{
    if (current == NULL)
    {
        bf_mem_init();
    }
    if (current->prev == NULL)
    {
        mc_link_t* nCell = bf_mem_create();
        nCell->next = current;
        current->prev = nCell;
    }
    current = current->prev;
}

void bf_mem_right()
{
    if (current == NULL)
    {
        bf_mem_init();
    }
    if (current->next == NULL)
    {
        mc_link_t* nCell = bf_mem_create();
        nCell->prev = current;
        current->next = nCell;
    }
    current = current->next;
}

uint8_t bf_mem_read()
{
    if (current == NULL)
    {
        bf_mem_init();
    }
    return current->data;
}

void bf_mem_write(uint8_t data)
{
    if (current == NULL)
    {
        bf_mem_init();
    }
    current->data = data;
}

void bf_mem_clear()
{
    if (current != NULL)
    {
        while (current->next != NULL)
        {
            current = current->next;
        }
        while(current->prev != NULL)
        {
            mc_link_t* temp = current;
            current = current->prev;
            bf_mem_free(temp);
        }
        bf_mem_free(current);
        current = NULL;
    }
}
