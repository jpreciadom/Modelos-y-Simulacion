#include "LinkedList.h"
#include <stdlib.h>

struct ChainNode *NextNode;

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
        return -1;

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
    if(index >= List->Size)
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

void Remove(struct LinkedList *List, int index){
    if(index == 0){
        RemoveFirst(List);
    } else if(index == List->Size-1){
        RemoveLast(List);
    } else if(index < List->Size && index > 0){
        struct ChainNode *node = List->First;
        for(int i = 1; i<index; i++){
            node = node->Next;
        }
        struct ChainNode *toDelete = node->Next;
        node->Next = toDelete->Next;
        FreeChainNode(toDelete);
        List->Size--;
    }
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
    if(List->Size > 1){
        struct ChainNode *node = List->First;
        while(node->Next != List->Last){
            node = node->Next;
        }
        FreeChainNode(node->Next);
        node->Next = NULL;
        List->Last = node;
        List->Size--;

    } else if (List->Size == 1){
        FreeChainNode(List->First);
        List->First = List->Last = NULL;
        List->Size = 0;
    }
}

void RemoveElement(struct LinkedList *List, void *Element){

}

void GetIterator(struct LinkedList *List){
    NextNode = List->First;
}

void * GetNext(){
    if(HasNext() == 1){
        void * reg = NextNode->Element;
        NextNode = NextNode->Next;
        return reg;
    } else {
        return NULL;
    }
}

short HasNext(){
    if(NextNode == NULL){
        return 0;
    } else {
        return 1;
    }
}
