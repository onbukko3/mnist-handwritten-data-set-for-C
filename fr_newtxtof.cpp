#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "fr_newtxt.h"
//#define MEMORY_CHECK
#define DYNAMIC

typedef enum
{
    FALSE,
    TRUE
} BOOL;

#ifdef DYNAMIC

typedef struct _STRING_
{
    char *str;
    int length;
}STRING, *pSTRING;
STRING *pDMfileContent = NULL;
#else
#define MAX_FILE_LINE 10000
typedef struct _STRING_
{
    char str[FILENAME_MAX];
    int length;
}STRING, *pSTRING;
STRING staFileContent[MAX_FILE_LINE];
#endif

linkedList *L;
BOOL getFileContentsASString(char *filename, pSTRING pBuf);
BOOL getFileContentsASCharacter(char *filename, pSTRING pBuf);
int getFilelineCount(char *filename);
BOOL copyToNewFile(int oflc, pSTRING ofl);
BOOL copyToNewFile_2();
BOOL copyToNewFile_ll(linkedList *L);

void ininMemory(void *p, size_t size);
BOOL copyString(char *dst, const char *src);
int getStringLength(const char *str);


int main(int argc, char *argv[])
{
    int retVal = 0;
    pSTRING pContent;
    char *filename;
    BOOL isValid = FALSE;
    int fileLineCount = 0;
    L = (linkedList*)malloc(sizeof(linkedList));
    L->head = NULL;
    L->tail = NULL;

    if(argc < 2)
    {
        printf("invalid argument.\n");
        printf("usage : $fr {filename}\n");
        return 0;
    }
    else
    {
        filename = argv[1];    
    }
    
    fileLineCount = getFilelineCount(filename);

#ifdef DYNAMIC
    printf("Using dynamic memory.\n");
    pDMfileContent = (pSTRING)malloc(sizeof(STRING) * fileLineCount);
    {
        int i;
        for(i = 0; i < fileLineCount; i++)
        {
            pDMfileContent[i].length = 0;
            pDMfileContent[i].str = NULL;
        }
    }
    pContent = pDMfileContent;
#else
    printf("Using static memory.\n");
    ininMemory((void*)staFileContent, sizeof(staFileContent));
    pContent = staFileContent;
#endif

    isValid = getFileContentsASString(filename, pContent);
    
    
    if(isValid)
    {
        // int i;
        printf("read file contents\n");
        // for(i = 0; i < fileLineCount; i++)
        // {
        //     printf("[%d]\t#%d\t%s",i, pContent[i].length, pContent[i].str);
        // }
        printNode(L);
        printf("end of file\n");
        printf("The file is successfully copied as \"malecp.txt\"!!! \n");
        // pContent = NULL; // for test
        // printf("%s", pContent);
        if(pContent!=NULL || L != NULL){
            // copyToNewFile(fileLineCount, pContent);
            // copyToNewFile_2();
            copyToNewFile_ll(L);
        }
        else{
            printf("You need to look at your original file again!\n");
        }
        
    }
    else
    {
        printf("Could not get file contents in '%s'\n", filename);
    }

#ifdef MEMORY_CHECK
    while(1)
    {
        int input = 0;
        scanf("%d", &input);
        if(input == 77)
            break;
    }
#endif

#ifdef DYNAMIC
    {
        int i;
        for(i = 0; i < fileLineCount; i++)
        {
            free(pDMfileContent[i].str);
        }
        free(pDMfileContent);
    }
#endif

    return retVal;
}

int getFilelineCount(char *filename)
{
    FILE *pf;
    int count = 0;
    pf = fopen(filename, "r");
    if(pf != NULL)
    {
        BOOL isEnd = FALSE;
        while(1)
        {
            //switch case constant number necessary
            switch (getc(pf))
            {
            case '\n':
                count++;
                break;
            case EOF:
                isEnd = TRUE;
                break;
            default:
                break;
            }

            if(isEnd)
                break;
        }
        fclose(pf);
    }

    return count;
}

BOOL getFileContentsASString(char *filename, pSTRING pBuf)
{
    FILE *pf;
    BOOL retVal = FALSE;
    int idx = 0;
    char strBuf[FILENAME_MAX] = {0,}; //char value initialize with value 0 

    // char *strAdd;
    

    pf = fopen(filename, "r");
    if(pf != NULL)
    {
        while(1)
        {
            if(NULL == fgets(strBuf, FILENAME_MAX, pf))
            {
                printf("file read end.\n");
                break;
            }

            // strcat(strBuf, ", ");

            pBuf[idx].length = getStringLength(strBuf);

#ifdef DYNAMIC
            // if(strBuf == NULL)
            // {
                // pBuf[idx].str = (char*)malloc(sizeof(char)*pBuf[idx].length);
                // copyString(pBuf[idx].str, strBuf);
                createNode(L, strBuf);
            // }
#endif
            idx++;
        }
        retVal = TRUE;
        fclose(pf);
    }

    return retVal;
}

BOOL getFileContentsASCharacter(char *filename, pSTRING pBuf)
{
    FILE *pf;
    BOOL retVal = FALSE;
    int idx = 0;
    char ch;
    // char strBuf[FILENAME_MAX] = {0,}; //char value initialize with value 0 
    // char *strAdd;

    pf = fopen(filename, "r");
    if(pf != NULL)
    {
        while(1)
        {
            ch = fgetc(pf);
            if(NULL == ch)
            {
                printf("file read end.\n");
                break;
            }

            }
        retVal = TRUE;
        fclose(pf);
    }

    return retVal;
}

BOOL copyToNewFile(int oflc, pSTRING ofl)
{
    FILE *pf = fopen("malecp.txt","wt");
    BOOL retVal = FALSE;
    int i = 0;

    if(pf!=NULL)
    {
        for(i=0; i <oflc; i++)
    {
        fwrite(ofl[i].str , ofl[i].length , 1 , pf);

    }

    fclose(pf);
    }
    
    return retVal;
}

BOOL copyToNewFile_ll(linkedList *L)
{
    FILE *pf = fopen("malecp_ll.txt","wt");
    BOOL retVal = FALSE;
    int i = 0;
    node *p = L ->head;

    if(pf!=NULL)
    {
        while (p != NULL)   
        {
            fwrite(p->data, strlen(p->data), 1, pf);
            p = p -> next;
        }

    fclose(pf);
    }
    
    return retVal;
}

BOOL copyToNewFile_2()
{   
    FILE *pf = fopen("./coref/male.txt","r");
    FILE *pf1 = fopen("malecp_1.txt","wt");
    BOOL retVal = FALSE;
    char *ch;
    int i = 0;
    if(pf != NULL)
    {
        BOOL isEnd = FALSE;
        while(1)
        {
            *ch = fgetc(pf);
            
            if(*ch == '\n')
            {
                fputc(',', pf1);
                fputc(' ', pf1);
            }
            else
                fputc(*ch, pf1);
            
            if(*ch == EOF)
                break;
            
            }
        fclose(pf);
        fclose(pf1);
    }


    
    return retVal;
}
    

void ininMemory(void *p, size_t size)
{
    char *_p = (char*)p;
    while(size--)
    {
        _p = 0;
        _p++;
    }
}

BOOL copyString(char *dst, const char *src)
{
    int srcLength = getStringLength(src);
    int i = 0;
    int dstLength = 0;
    BOOL retVal = TRUE;

    for(i = 0; i < srcLength; i++)
    {
        dst[i] = src[i];
    }

    dstLength = getStringLength(dst);
    if(dstLength != srcLength)
        retVal = FALSE;

    return retVal;
}

int getStringLength(const char *str)
{
    int i = 0;
    int length = 0;

    while(str[i++] != '\0')
    {
        if(i > FILENAME_MAX)
            break;
        length++;
    }

    return length;
}
