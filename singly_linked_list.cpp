#include <stdio.h>
#include <stdlib.h>

typedef struct __list{
    struct __node *cur;
    struct __node *head;
    struct __node *tail;    
} linkedList;

typedef struct __node{
    int data;
    struct __node *next; // the address of the node
} node;



void createNode(linkedList *L, int initData)
{
    node *New = (node*)malloc(sizeof(node));
    New -> data = initData;
    New -> next = NULL;

    if(L -> head == NULL && L -> tail == NULL)
        L->head = L->tail = New;
    else
    {
        L -> tail -> next = New;
        L -> tail = New;
    }

    L->cur = New;
}

void deleteLastNode(linkedList *L)
{
    node *p = L ->head;
    while(p->next->next != NULL) p = p-> next;
    p -> next = p -> next -> next;
    L -> tail = p;
}

void printNode(linkedList *L)
{
    node *p = L -> head;
    putchar('[');
    while (p != NULL)
    {
        printf("%d", p->data);
        p = p ->next;
    }
    putchar(']');
    putchar('\n');
}