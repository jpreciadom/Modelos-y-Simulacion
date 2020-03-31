#include "LinkedList.h"
#include <stdlib.h>

struct LinkedList * initLinkedList(){
    struct LinkedList *list = malloc(sizeof(struct LinkedList));
    if(list == NULL)
        return NULL;

    list->First = list->Last = NULL;
    list->Size = 0;
    return list;
}

void FreeNodes(struct ChainNode *node){
    if(node != NULL){
        FreeNodes(node->Next);
        FreeChainNode(node);
    }
}


void MakeEmpty(struct LinkedList *List){
    FreeNodes(List->First);
    List->First = List->Last = NULL;
    List->Size = 0;
}

void AddFirst(struct LinkedList *List, void *Element){
    struct ChainNode *node = initChainNode(Element);
    node->Next = List->First;
    List->First = node;
    if(List->Size == 0)
        List->Last = node;

    List->Size++;
}

short Add(struct LinkedList *List, void *Element, int index) {
    if(index > List->Size)
        return -1;

    struct ChainNode *node = initChainNode(Element);
    if(index == 0){
        node->Next = List->First;
        List->First = node;
    } else {
        struct ChainNode *reg = List->First;
        for(int i = 1; i < index; i++){
            reg = reg->Next;
        }
        node->Next = reg->Next;
        reg->Next = node;
    }

    if(index == List->Size)
        List->Last = node;

    List->Size++;
    return index;
}

void AddLast(struct LinkedList *List, void *Element){
    struct ChainNode *node = initChainNode(Element);
    if(List->Size == 0){
        List->First = node;
    } else {
        List->Last->Next = node;
    }
    List->Last = node;
    List->Size++;
}

void Remove(struct LinkedList *List, int index){

}

void RemoveFirst(struct LinkedList *List){
    struct ChainNode *toRemove = List->First;

    List->First = toRemove->Next;
    FreeChainNode(toRemove);
    List->Size--;
    if(List->Size == 0)
        List->Last = NULL;
}

void RemoveLast(struct LinkedList *List){

}

void RemoveElement(struct LinkedList *List, void *Element){

}
