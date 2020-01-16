#include <stdio.h>
#include <stdlib.h>
#include "include/linkedlist.h"
#include "include/util.h"



void createNode(linkedList *L, char *initData, int length)
{
    node *New = (node*)malloc(sizeof(node));
#ifdef DYNAMIC
    New->data.str = (char*)malloc(sizeof(char) * getStringLength(initData));
#endif
    copyString(New->data.str, initData);
    New->data.length = length;
    New->next = NULL;

    if(L->head == NULL && L->tail == NULL)
        L->head = L->tail = New;
    else
    {
        L->tail->next = New;
        L->tail = New;
    }

    L->cur = New;
}

void deleteLastNode(linkedList *L)
{
    node *p = L->head;
    node *temp;
    while(p->next->next != NULL)
    {
        temp = p;
        p = p->next;
#ifdef DYNAMIC
        free(temp->data.str);
#endif
        free(temp);
    }
    p->next = p->next->next;
    L->tail = p;
}

void deleteAllNode(linkedList *L)
{
	while (L->head != NULL)
	{
        deleteLastNode(L);
	}

	free(L);
}

void printNode(linkedList *L)
{
    node *p = L->head;
    putchar('[');
    while (p != NULL)
    {
        printf("%s", p->data.str);
        p = p->next;
    }
    putchar(']');
    putchar('\n');
}