#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_
#include "userdef.h"

typedef struct __list{
    struct __node *cur;
    struct __node *head;
    struct __node *tail;    
} linkedList;

typedef struct __node{
    STRING data;
    struct __node *next; // the address of the node
} node;

void createNode(linkedList *L, char *initData, int length);
void deleteLastNode(linkedList *L);
void printNode(linkedList *L);
void deleteAllNode(linkedList *L);

#endif