#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fr_newtxt.h"



void createNode(linkedList *L, char *initData)
{
    // char data = (data*)malloc(sizeof(char));
    node *New = (node*)malloc(sizeof(node));
    New -> data = (char*)malloc(sizeof(char)*strlen(initData));
    strncpy(New->data, initData, strlen(initData)-1);
    strcat(New->data, ", ");
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
        printf("%s", p->data);
        p = p ->next;
    }
    putchar(']');
    putchar('\n');
}