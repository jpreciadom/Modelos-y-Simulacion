#ifndef LINKEDLISTH
#define LINKEDLISTH

#include "ChainNode.h"

struct LinkedList {
    struct ChainNode *First;
    struct ChainNode *Last;
    unsigned int Size;
};

struct LinkedList * initLinkedList();
void MakeEmpty();
void AddFirst(void *Element);
short Add(void *Element, int index);
void AddLast(void *Element);
void * Remove(int index);
void * RemoveFirst();
void * RemoveLast();
void RemoveElement(void *Element);

#endif // LINKEDLISTH
