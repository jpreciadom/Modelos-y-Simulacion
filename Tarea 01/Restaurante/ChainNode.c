#include <stdlib.h>
#include "ChainNode.h"

struct ChainNode *initChainNode(void *Element){
    struct ChainNode *newNode = (struct ChainNode *)malloc(sizeof(struct ChainNode));
    if(newNode == NULL)
        return NULL;
    newNode->Element = Element;
    newNode->Next = NULL;
    return newNode;
}

void FreeChainNode(struct ChainNode *toFree, short FreeElement){
    if(FreeElement == 1)
        free(toFree->Element);
    toFree->Element = NULL;
    toFree->Next = NULL;
    free(toFree);
}
