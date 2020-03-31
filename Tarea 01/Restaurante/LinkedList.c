#include "LinkedList.h"
#include <stdlib.h>

struct LinkedList * initLinkedList(){
    struct LinkedList *list = (struct LinkedList *)malloc(sizeof(struct LinkedList));
    if(list == NULL)
        return NULL;

    list->First = list->Last = NULL;
    list->Size = 0;
    return list;
}

void FreeNodes(struct ChainNode *node){
    if(node != NULL){
        FreeNodes(node->Next);
        FreeChainNode(node, 1);
    }
}


void MakeEmpty(struct LinkedList *List){
    FreeNodes(List->First);
    List->First = List->Last = NULL;
    List->Size = 0;
}

short AddFirst(struct LinkedList *List, void *Element){
    struct ChainNode *node = initChainNode(Element);
    if(node == NULL)
        return -1;
    node->Next = List->First;
    List->First = node;
    if(List->Size == 0)
        List->Last = node;

    List->Size++;
    return 0;
}

short Add(struct LinkedList *List, void *Element, int index) {
    if(index > List->Size)
        return -1;

    struct ChainNode *node = initChainNode(Element);
    if(node == NULL)
        return -1;

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

short AddLast(struct LinkedList *List, void *Element){
    struct ChainNode *node = initChainNode(Element);
    if(node == NULL)
        return NULL;

    if(List->Size == 0){
        List->First = node;
    } else {
        List->Last->Next = node;
    }
    List->Last = node;
    List->Size++;
    return List->Size;
}

void * Get(struct LinkedList *List, int index){
    if(index > List->Size)
        return NULL;

    struct ChainNode *toGet = List->First;
    for(int i = 0; i<index; i++){
        toGet = toGet->Next;
    }
    return toGet->Element;
}

void * GetLast(struct LinkedList *List){
    return (List->Size > 0) ? List->Last->Element : NULL;
}

void * Remove(struct LinkedList *List, int index){
    void * toReturn = NULL;
    if(index == List->Size){
        toReturn = RemoveLast(List);
    } else if(index == 0){
        toReturn = RemoveFirst(List);
    } else if(index < List->Size && index > 0){
        struct ChainNode *node = List->First;
        for(int i = 1; i<index; i++){
            node = node->Next;
        }
        struct ChainNode *toDelete = node->Next;
        node->Next = toDelete->Next;
        toReturn = toDelete->Element;
        FreeChainNode(toDelete, 0);
    }
    return toReturn;
}

void * RemoveFirst(struct LinkedList *List){
    void * toReturn = NULL;
    if(List->Size > 0){
        struct ChainNode *toRemove = List->First;
        toReturn = toRemove->Element;
        List->First = toRemove->Next;
        FreeChainNode(toRemove, 0);
        List->Size--;
        if(List->Size == 0)
            List->Last = NULL;
    }
    return toReturn;
}

void * RemoveLast(struct LinkedList *List){
    void * toReturn = NULL;
    if(List->Size > 1){
        struct ChainNode *node = List->First;
        while(node->Next != List->Last){
            node = node->Next;
        }
        toReturn = node->Next->Element;
        FreeChainNode(node->Next, 0);
        node->Next = NULL;
        List->Last = node;
        List->Size--;

    } else if (List->Size == 1){
        toReturn = List->First->Element;
        FreeChainNode(List->First, 0);
        List->First = List->Last = NULL;
        List->Size = 0;
    }
    return toReturn;
}
