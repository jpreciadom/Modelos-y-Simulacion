#include <stdlib.h>
#include "ChainNode.h"

struct ChainNode *initChainNode(void *Element){
    struct ChainNode *newNode = malloc(sizeof(struct ChainNode));
    if(newNode == NULL)
        return NULL;
    newNode->Element = Element;
    newNode->Next = NULL;
    return newNode;
}

void FreeChainNode(struct ChainNode *toFree){
    free(toFree->Element);
    toFree->Element = NULL;
    toFree->Next = NULL;
    free(toFree);
}
