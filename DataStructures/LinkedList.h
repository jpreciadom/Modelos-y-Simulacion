#ifndef LINKEDLISTH
#define LINKEDLISTH

#include "ChainNode.h"

struct LinkedList {
    struct ChainNode *First;
    struct ChainNode *Last;
    unsigned int Size;
};

struct LinkedList * initLinkedList();
void MakeEmpty(struct LinkedList *List);
void AddFirst(struct LinkedList *List, void *Element);
short Add(struct LinkedList *List, void *Element, int index);
void AddLast(struct LinkedList *List, void *Element);
void Remove(struct LinkedList *List, int index);
void RemoveFirst(struct LinkedList *List);
void RemoveLast(struct LinkedList *List);
void RemoveElement(struct LinkedList *List, void *Element);

#endif // LINKEDLISTH
