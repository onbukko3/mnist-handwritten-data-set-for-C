#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/fr.h"
#include "include/linkedlist.h"
#include "include/util.h"

#ifdef DYNAMIC
STRING *pDMfileContent = NULL;
#else
STRING staFileContent[MAX_FILE_LINE];
#endif

//#define MEMORY_CHECK

static linkedList *list;

static BOOL getFileContents(char *filename, pSTRING pBuf);
static BOOL getFileContentsWithLinkedlist(char *filename, linkedList *plist);
static int getFilelineCount(char *filename);
BOOL copyToNewFile(int oflc, pSTRING ofl);
BOOL copyToNewFileWithLinkedlist(int oflc, linkedList *plist);

void ininMemory(void *p, size_t size);


int _process(int argc, char *argv[])
{
    int retVal = 0;
    pSTRING pContent;
    char *filename;
    BOOL isValid = FALSE;
    int fileLineCount = 0;
    BOOL isUsingLinkedlist = FALSE;

    if(argc < 2)
    {
        printf("invalid argument.\n");
        printf("usage : $fr {filename}\n");
        return 0;
    }
    else
    {
        filename = argv[1];
        if(argv[2][0] == 'l')
        {
            isUsingLinkedlist = TRUE;
        }
    }
    
    fileLineCount = getFilelineCount(filename);

    if(isUsingLinkedlist)
    {
        list = (linkedList*)malloc(sizeof(linkedList));
        list->cur = NULL;
        list->head = NULL;
        list->tail = NULL;

        isValid = getFileContentsWithLinkedlist(filename, list);

        if(isValid)
        {
            printf("read file contents\n");
            printNode(list);
            printf("end of file\n");

            if(list->head != NULL){

                copyToNewFileWithLinkedlist(fileLineCount, list);
                printf("The file is successfully copied!! \n");
            }
            else{
                printf("You need to look at your original file again!\n");
            }
        }
        else
        {
            printf("Could not get file contents in '%s'\n", filename);
        }
    }
    else
    {    
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

        isValid = getFileContents(filename, pContent);
        
        if(isValid)
        {
            int i;
            printf("read file contents\n");
            for(i = 0; i < fileLineCount; i++)
            {
                printf("[%d]\t#%d\t%s",i, pContent[i].length, pContent[i].str);
            }
            printf("end of file\n");

            if(pContent!=NULL){

                copyToNewFile(fileLineCount, pContent);
                printf("The file is successfully copied!! \n");
            }
            else{
                printf("You need to look at your original file again!\n");
            }
        }
        else
        {
            printf("Could not get file contents in '%s'\n", filename);
        }
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
    if(isUsingLinkedlist == FALSE)
    {
        int i;
        for(i = 0; i < fileLineCount; i++)
        {
            free(pDMfileContent[i].str);
        }
        free(pDMfileContent);
    }
    else
    {
        deleteAllNode(list);
        list = NULL;    
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

BOOL getFileContents(char *filename, pSTRING pBuf)
{
    FILE *pf;
    BOOL retVal = FALSE;
    int idx = 0;
    char strBuf[FILENAME_MAX] = {0,};

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
            
            pBuf[idx].length = getStringLength(strBuf);

#ifdef DYNAMIC
            if(pBuf[idx].str == NULL)
            {
                pBuf[idx].str = (char*)malloc(sizeof(char) * pBuf[idx].length);
            }
#endif
            copyString(pBuf[idx].str, strBuf);
            idx++;
        }
        retVal = TRUE;
        fclose(pf);
    }

    return retVal;
}

BOOL getFileContentsWithLinkedlist(char *filename, linkedList *plist)
{
    FILE *pf;
    BOOL retVal = FALSE;
    int length = 0;
    char strBuf[FILENAME_MAX] = {0,};

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
            
            length = getStringLength(strBuf);
            if(strBuf[length - 1] == '\n')
            {
                strBuf[length - 1] = 0;
                length--;
            }

            createNode(plist, strBuf, length);
        }
        retVal = TRUE;
        fclose(pf);
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



BOOL copyToNewFile(int oflc, pSTRING ofl)
{
    FILE *pf = fopen("malecp.txt","wt");
    BOOL retVal = FALSE;
    int i = 0;

    for(i=0; i <oflc; i++)
    {
        fwrite(ofl[i].str,ofl[i].length,1,pf);

    }
    fclose(pf);

    return retVal;
}

BOOL copyToNewFileWithLinkedlist(int oflc, linkedList *plist)
{
    FILE *pf = fopen("malecp.csv","wt");
    BOOL retVal = FALSE;
    int i = 0;

    node *p = plist->head;

    while (p != NULL)
    {
        fprintf(pf, "%s",p->data.str);
        if(i < oflc)
        {
            fprintf(pf, ",");
        }
        p = p->next;
    }

    fclose(pf);

    return retVal;
}