#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 10

char buf[BUFFER_SIZE];


int nofFileLines (char *filename)
{
    FILE * pFile = NULL;
    int nread;
    int line = 0;
    
    pFile = fopen(filename, "r");
    if(pFile!=NULL) {
        while(1) {
            nread = fread(buf, sizeof(char), BUFFER_SIZE, pFile);
            //printf("%d",nread);
            if(nread<=0) break;
            for(int i=0; i<nread; i++) {
                if( buf[i] == '\n' ) 
                    line++;
            }
        }
        fclose(pFile);
    }
    return line;
}

int ArrayList (char *filename, int linesize)
{
    FILE * pFile = NULL;
    int nread;
    char * arrList[linesize];
    int n = 0;
    char c;
    char namestring[10];

    pFile = fopen(filename, "r");
    if(pFile!=NULL) {
        while(1) {
            //nread = fread(buf, sizeof(char), BUFFER_SIZE, pFile);
            
            //printf("%d",nread);
            
            //if(nread>0){
            //    char* newstrptr = (char*)malloc(sizeof(char)*(nread));
            //    for(int i =0; i<BUFFER_SIZE;i++){
            //        strcpy(newstrptr, buf);
            //        arrList[n] = newstrptr;
            //        n++;}
            //}
            //else if (nread <= 0) break;
            for(int i= 0 ; i<sizeof(namestring)/sizeof(char); i++){
                
                c = fgetc(pFile);
                if(c!='\n'){
                namestring[i] = c;
                char* newcharptr = (char*)malloc(sizeof(char)*linesize+1);
                strcpy(newcharptr,namestring);
                arrList[n] = newcharptr;
                n++;}
                else if (c == '\n')
                {
                    break;
                }
                
                }
            
        fclose(pFile);    
        }
        

        for(n=0;n<linesize;n++){
            printf("%02d: %s\n",n, arrList[n]);
            free(arrList[n]);
        }
    }
    return 0;
}

int main(){
    char fn[]= "male.txt";
    int noline;
    char maleList;

    
    noline =  nofFileLines(fn);
    maleList = ArrayList(fn, noline);
     
    printf("%s %d\n ", "The number of lines:", noline);
    //printf("%s\n", maleList);

    return 0;

}
