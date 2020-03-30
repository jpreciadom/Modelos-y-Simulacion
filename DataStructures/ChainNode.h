#ifndef CHAINNODEH
#define CHAINNODEH

struct ChainNode {
    void *Element;
    struct ChainNode *Next;
};

struct ChainNode *initSimpleChainNode(void *Element);
void FreeChainNode(struct ChainNode *toFree);

#endif // CHAINNODEH
