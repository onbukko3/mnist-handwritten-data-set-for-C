#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "linked_list.h"

#define MAX_FOLDER_NAME 5012

linkedList *L;


int main()
{
    char *start_path;
    start_path = (char*)malloc(sizeof(char*)*MAX_FOLDER_NAME);
    start_path = "/home/hsji/study/clanguage";
    L = (linkedList*)malloc(sizeof(linkedList));
    L->head = NULL;
    L->tail = NULL; 
    getfiles(start_path);
    printNode(L);

    return 0;

}
void getfiles(char* path)
{
    DIR *dir;
    struct dirent *ent;
    struct stat st;
    dir = opendir(path);
    int name_size;
    char *name;
    char *filename ;
    char folder_name[1024];


    // if(stat(path,&st) ==-1)
    // {
    //     perror("stat failed");
    //     exit
    // }


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

                        if(strcmp(type[1],"jpg")==0 || strcmp(type[1], "bmp")==0 || strcmp(type[1], "gif")==0)
                        {
                            if(L != NULL)
                            {

                                createNode_char(L, ent->d_name);
                            }
                            // printf("%s\n", ent->d_name);
                        }
                        else
                        {
                            // printf("%s\n", filename);
                            continue;
                        }
                        
                        
                    }
                    else continue;    
                }
                else continue;
            }

            else if(ent->d_type == 4)
            {
                if(strcmp(ent->d_name, "..") == 0 || strcmp(ent->d_name, ".")==0 ) 
                continue;
                else
                {
                    strcpy(folder_name, path);
                    strcat(folder_name, "/");
                    strcat(folder_name,ent->d_name);
                    // printf("%s\n",folder_name);
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
}