#include <stdio.h>
#include <stdlib.h>
#include "fifo.h"

next_link fifo_add(next_link *f, int item)
{
    next_link p = *f;

    Node * new_node = (Node *)malloc(sizeof(Node));
    new_node->item = item;
    new_node->next = NULL;

    if (p != NULL)
    {
        while (p->next != NULL)
        {
            p = p->next;
        }

        p->next = new_node;
        return *f;
    }

    return new_node;
}

next_link fifo_remove(next_link *f){
    if(f != NULL){
        next_link item = *f;
        (*f) = (*f)->next;
        
        return item;
    }

    return NULL;
}