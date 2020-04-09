
typedef struct __list{
    struct __node *cur;
    struct __node *head;
    struct __node *tail;    
} linkedList, *plinkedList;

typedef struct __node{
    char *data;
    struct __node *next; // the address of the node
} node;

void createNode(linkedList *L, void *initData);
void deleteLastNode(linkedList *L);
void deleteAllNode(linkedList *L);
void printNode(linkedList *L);

