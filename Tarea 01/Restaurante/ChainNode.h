#ifndef CHAINNODEH
#define CHAINNODEH

struct ChainNode {
    void *Element;
    struct ChainNode *Next;
};

struct ChainNode *initChainNode(void *Element);
void FreeChainNode(struct ChainNode *toFree, short FreeElement);

#endif // CHAINNODEH
