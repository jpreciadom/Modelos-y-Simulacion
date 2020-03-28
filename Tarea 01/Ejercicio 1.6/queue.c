#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

struct ChainNode * initNode(long tiempoDeLlegada){
    struct ChainNode* node;
    do{
        node = (struct ChainNode *)malloc(sizeof(struct ChainNode));
    } while(node == NULL);
    node->TiempoDeLlegada = tiempoDeLlegada;
    node->Next = NULL;
    return node;
}

struct Queue * initQueue(){
    struct Queue* queue;
    do{
        queue = malloc(sizeof(struct Queue));
    } while(queue == NULL);
    queue->Front = queue->Tail = NULL;
    queue->Size = 0;
    return queue;
}

void Enqueue(struct Queue *Queue, struct ChainNode *node){
    if(Queue->Size == 0){
        Queue->Front = Queue->Tail = node;
        Queue->Size++;
    } else {
        Queue->Tail->Next = node;
        Queue->Tail = node;
        Queue->Size++;
    }
}

struct ChainNode *Dequeue(struct Queue *Queue){
    struct ChainNode *toReturn = Peek(Queue);
    if(toReturn != NULL){
        Queue->Front = toReturn->Next;
        toReturn->Next = NULL;
        Queue->Size--;
    }
    return toReturn;
}

struct ChainNode *Peek(struct Queue *Queue) {
    return Queue->Front;
}

void FreeNode(struct ChainNode *node){
    node->Next = NULL;
    free(node);
}

void FreeQueue(struct Queue *Queue){
    while(Queue->Size > 0){
        FreeNode(Dequeue(Queue));
    }
    Queue->Front = Queue->Tail = NULL;
    free(Queue);
}
