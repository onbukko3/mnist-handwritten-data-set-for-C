#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include "linked_list.h"

typedef struct _STRING_
{
    char *str;
    int length;
}STRING, *pSTRING;

int main()
{
    linkedList * L;
    char *path = (char*)malloc(sizeof(char)*50);
    strcpy(path, "./data/");
    DIR *dir;
    struct dirent *ent;
    pSTRING pBuf;
    dir = opendir(path);
    int idx =0;
    int name_size;
    char *name ;
    L = (linkedList*)malloc(sizeof(linkedList));
    L->head = NULL;
    L->tail = NULL;


    if(dir != NULL)
    {
        while((ent=readdir(dir))!=NULL)
        {
            // printf("%s\n", ent->d_name);
            // name = (char *)malloc(sizeof(char)*ent->d_reclen);
            // strcpy(name, ent->d_name);
            if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, ".")==0) 
            continue;
            else 
                createNode_char(L, ent->d_name);
            
        }
        closedir(dir);
    }
    else
    {
        perror("");
        return EXIT_FAILURE;
    }
    node *p = L ->head;

    while (p !=NULL)
    {
        name = (char*)malloc(sizeof(char)*strlen(p->data));
        strcpy(name, p->data);
        if(strcmp(p->data, "..") == 0 || strcmp(p->data, ".")==0)
        {
            p = p->next;
            continue;
        }
        else;
            printf("%s\n", p->data);
            strcat(path, name);
            // free(name);
            p = p->next;
            

    }
    
    
    return 0;
}