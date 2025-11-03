#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>

// A node in the linked list.
typedef struct Node Node;
struct Node {
    // A pointer to the previous node.
    Node* back;
    // A pointer to the next node.
    Node* front;
    // A pointer to the item stored in the node.
    void* item;
};

// The linked list struct.
typedef struct {
    // A pointer to the first node in the list.
    Node* head;
    // A pointer to the last node in the list.
    Node* tail;
    // The number of items in the list.
    size_t size;
} LinkedList;

// A macro for iterating over the linked list.
#define LinkedList_foreach(list, node) \
    for (Node* node = (list)->head; node != NULL; node = node->front)

/* Initializes a new empty LinkedList, and returns the pointer to it */
LinkedList* LinkedList_create();

/* Returns a pointer to a Node in a LinkedList at a specific index, returns NULL if index does not exist */
Node* LinkedList_get_index(LinkedList* list, size_t index);

/* Inserts a new item into the LinkedList at a specific index, shifting items after it to the right */
int LinkedList_insert(LinkedList* list, size_t index, void* item);

/* "Appends" a new item into the LinkedList at the last index */
int LinkedList_append(LinkedList* list, void* item);

/*
  Remove a Node from a LinkedList by reference
    The Node is disposed when removed, you should never attempt to access it again.
    The stored item is only disposed if free_function is not NULL
    For basic items, you can use the normal free() function.
    Example: `LinkedList_remove(list, node, free)`
*/
int LinkedList_remove(LinkedList* list, Node* item, void (*free_function)(void*));

/*
  Remove a Node from a LinkedList by index
    Follows the same behavior as LinkedList_remove but searches for a Node by index
*/
int LinkedList_pop(LinkedList* list, size_t index, void (*free_function)(void*));

/*
  Remove all nodes from the LinkedList
    Recommended to send a free_function to properly dispose stored items.
    For basic items, you can use the normal free() function.
    Example: `LinkedList_clear(list, free)`
*/
void LinkedList_clear(LinkedList* list, void (*free_function)(void*));

/*
  Dispose entire LinkedList, removing all nodes
    Recommended to send a free_function to properly dispose stored items.
    Double pointer is used to prevent dangling pointers, after LinkedList_dispose is called your variable will be set to NULL.
    For basic items, you can use the normal free() function.
    Example: `LinkedList_dispose(&list, free)`
*/
void LinkedList_dispose(LinkedList** list, void (*free_function)(void*));

#endif