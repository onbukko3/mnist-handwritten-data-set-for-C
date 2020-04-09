#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"



void createNode(linkedList *L, void *initData)
{
    // char data = (data*)malloc(sizeof(char));
    int size;
    node *New = (node*)malloc(sizeof(node));
    size = strlen(initData);
    New -> data = (char*)malloc(sizeof(char)*strlen(initData)+1);
    strcpy(New->data, initData);
    // strcat(New->data, ", ");
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

void deleteAllNode(linkedList *L)
{
    node *p = L->head;
    node *temp = NULL;

    while (p!=L->tail)
    {
        temp = p;
        p = p->next;
        free(temp->data);
        free(temp);

    }
    free(L);
}

void printNode(linkedList *L)
{
    node *p = L -> head;
    putchar('[');
    while (p != NULL)
    {
        if(p->next == NULL)
        {
            printf("%s", p->data);
            p = p->next;
        }
        else
        {
            printf("%s, \n ", p->data);
            p = p ->next;
        }
    }
    putchar(']');
    putchar('\n');
}