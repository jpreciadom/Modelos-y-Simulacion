#ifndef QUEUEH
#define QUEUEH

struct ChainNode {
    long TiempoDeLlegada;
    struct ChainNode *Next;
};

struct Queue {
    struct ChainNode *Front;
    struct ChainNode *Tail;
    int Size;
};

struct ChainNode * initNode(long tiempoDeLlegada);
struct Queue * initQueue();
void Enqueue(struct Queue *Queue, struct ChainNode *node);
struct ChainNode *Dequeue(struct Queue *Queue);
struct ChainNode *Peek(struct Queue *Queue);
void FreeNode(struct ChainNode *node);
void FreeQueue(struct Queue *Queue);

#endif // QUEUEH
