#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "linkedlist.h"

#define MAX_FOLDER_NAME 5012

linkedList *L;
void getfiles(char* path);
// char *get_filename_ext(const char *filename);


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
    dir = opendir(path);
    char *name;
    char *filename ;
    char folder_name[1024];
    char *file_loc;

    if(dir != NULL)
    {
        while((ent=readdir(dir))!=NULL)
        {
            char *type[2] = {NULL,};
            int i =0;
            filename = (char *)malloc(sizeof(char)*ent->d_reclen);
            strcpy(filename, ent->d_name);
            file_loc = (char*)malloc(sizeof(char)*(strlen(path)+strlen(filename)+2));

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
                        if(strcmp(type[1],"jpg")==0)
                        {
                            if(L != NULL)
                            {
                                if(filename != NULL)
                                {
                                    strcpy(file_loc, path);
                                    strcat(file_loc, "/");
                                    strcat(file_loc, ent->d_name);
                                    // printf("%ld\t %ld\t %ld\t %ld\t \n", strlen(ent->d_name), strlen(path), strlen(filename), strlen(file_loc));

                                    // printf("%s\n", file_loc);
                                    createNode(L, file_loc);
                                    free(filename);
                                    filename = NULL;
                                }
                                if(file_loc != NULL)
                                {
                                    free(file_loc);
                                    file_loc = NULL;
                                }
                            }
                        }
                        else
                        {
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
                    if(ent->d_name[0] != '.')
                    {
                        getfiles(folder_name);
                    }
                }
                                
            }
        }
        closedir(dir);
    }
    else
    {
        perror("");
    }    
}

// char *get_filename_ext(const char *filename)
// {
// 	char *dot = strrchr(filename, '.');
// 	if (!dot || dot == filename) {
// 		return "";
// 	}
// 	return dot + 1;
// }