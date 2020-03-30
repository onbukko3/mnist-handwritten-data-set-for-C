#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "linked_list.h"


int main()
{
    char *path;
    path = (char*)malloc(sizeof(char*)*1000);
    path = "/home/hsji/study";
    getfiles(path);

    return 0;

}
void getfiles(const char* path)
{
    linkedList * L;
    DIR *dir;
    struct dirent *ent;
    dir = opendir(path);
    int name_size;
    char *name;
    char *filename ;
    char folder_name[1024];
    L = (linkedList*)malloc(sizeof(linkedList));
    L->head = NULL;
    L->tail = NULL;


    if(dir != NULL)
    {
        while((ent=readdir(dir))!=NULL)
        {
            // printf("%s\n", ent->d_name);
            char *type[2] = {NULL,};
            int i =0;
            filename = (char *)malloc(sizeof(char)*strlen(ent->d_name));
            strcpy(filename, ent->d_name);

            if(ent->d_type==8)
            {
                if(strchr(filename,'.')!=NULL)
                {
                    char *ptr = strtok(filename, ".");
                    while(ptr != NULL)
                    {
                        type[i] = ptr;
                        i++;

                        ptr = strtok(NULL, " ");
                    }
                    if(type[1] != NULL)
                    {

                        if(strcmp(type[1],"jpg")==0 || strcmp(type[1], "bmp")==0)
                        {
                            createNode_char(L, ent->d_name);
                        }
                    }
                    else continue;    
                }
                else continue;
            }

            else if(ent->d_type == 4)
            {
                if(strcmp(ent->d_name, "..") == 0 || strcmp(ent->d_name, ".")==0) 
                continue;
                else
                {
                    strcpy(folder_name, path);
                    strcat(folder_name, "/");
                    strcat(folder_name,ent->d_name);
                    getfiles(folder_name);
                }
                
                    // printf("%s\n", path);
                
            }
        }
        closedir(dir);
    }
    else
    {
        perror("");
    }
    // printNode(L);
    node *p = L ->head;

    while (p !=NULL)
    {
        name = (char*)malloc(sizeof(char)*strlen(p->data));
        strcpy(name, p->data);
        printf("%s\n",name);
        p = p->next;
    }
    
    
}