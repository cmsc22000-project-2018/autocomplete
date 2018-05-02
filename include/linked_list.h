/* Again, credit to
   https://www.geeksforgeeks.org/generic-linked-list-in-c-2/
   for this implementation */

#include <stdio.h>
#include <stdlib.h>

/* A linked list node */
struct Node
{
    // Any data type can be stored in this node
    void  *data;

    struct Node *next;
};

void push(struct Node** head_ref, void *new_data, size_t data_size);

void append(struct Node** tail_ref, void *new_data, size_t data_size);

void fmap(struct Node *node, void (*fptr)(void *));

void fmap2(struct Node *node, void (*fptr)(void *, const void*, const void*), const void* a, const void* b);

void free_linked_list(struct Node *node);
