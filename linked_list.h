//linked list for character data
typedef struct __list{
    struct __node *cur;
    struct __node *head;
    struct __node *tail;    
} linkedList;

//;inked list for number data
typedef struct __list_num{
    struct __node_num *cur;
    struct __node_num *head;
    struct __node_num *tail;    
} linkedList_num;

// node for character data
typedef struct __node{
    char *data;
    struct __node *next; // the address of the node
} node;

//node for number data
typedef struct __node_num{
    float data;
    struct __node_num *next; // the address of the node
} node_num;

void createNode(linkedList *L, char *initData);
void createNode(linkedList_num *L, float initData);
void deleteLastNode(linkedList *L);
void printNode(linkedList *L);
