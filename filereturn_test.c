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
    char *path = "/home/hsji/study";
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
            name = (char *)malloc(ent->d_reclen);
            strcpy(name, ent->d_name);
            createNode_char(L, name);
            // printf("%s", L->head->data);
            idx++;
        }
        closedir(dir);
    }
    else
    {
        perror("");
        return EXIT_FAILURE;
    }

    printNode(L);

    return 0;
}