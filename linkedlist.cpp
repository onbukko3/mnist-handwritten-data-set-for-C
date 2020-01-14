#include <stdio.h>
#include <stdlib.h>
#include "include/linkedlist.h"
#include "include/fr.h"


void createNode(linkedList *L, char *initData)
{
    node *New = (node*)malloc(sizeof(node));
    New->data = (char*)malloc(sizeof(char) * getStringLength(initData));
    copyString(New->data, initData);
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
        free(temp->data);
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
        printf("%s", p->data);
        p = p->next;
    }
    putchar(']');
    putchar('\n');
}