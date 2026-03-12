#ifndef FIFO_H
#define FIFO_H

typedef struct Node
{
    int item;
    struct Node* next;
} Node;
typedef Node* next_link;

next_link fifo_add(next_link *f, int item);
next_link fifo_remove(next_link *f);

#endif