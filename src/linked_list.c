/* Credit for this linked list goes to
   https://www.geeksforgeeks.org/generic-linked-list-in-c-2/
   I've applied minimal tweaking only.
*/

// C program for generic linked list
#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

/* I've left this function for reference only, append (created by tweaking
   this function) will be what we use */

/* Function to add a node at the beginning of Linked List.
   This function expects a pointer to the data to be added
   and size of the data type */
void push(struct Node** head_ref, void *new_data, size_t data_size)
{
    // Allocate memory for node
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));

    new_node->data  = malloc(data_size);
    new_node->next = (*head_ref);

    // Copy contents of new_data to newly allocated memory.
    // Assumption: char takes 1 byte.
    int i;
    for (i=0; i<data_size; i++)
        *(char *)(new_node->data + i) = *(char *)(new_data + i);

    // Change head pointer as new node is added at the beginning
    (*head_ref)    = new_node;
}

/* Function to add a node at the end of Linked List.
   This function expects a pointer to the data to be added
   and size of the data type */
void append(struct Node** tail_ref, void *new_data, size_t data_size)
{
    // Allocate memory for node
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));

    new_node->data  = malloc(data_size);
    (*tail_ref)->next = (new_node);

    // Copy contents of new_data to newly allocated memory.
    // Assumption: char takes 1 byte.
    int i;
    for (i=0; i<data_size; i++)
        *(char *)(new_node->data + i) = *(char *)(new_data + i);

    // Change tail pointer as new node is added at the beginning
    (*tail_ref)    = new_node;
}

/* Function to map over nodes in a given linked list. fptr is used
   to access the function to be applied to current node data.
   Note that different data types need different functions */
void fmap(struct Node *node, void (*fptr)(void *))
{
    while (node != NULL)
    {
        (*fptr)(node->data);
        node = node->next;
    }
}
