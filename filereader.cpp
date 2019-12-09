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

void ArrayList (char *filename, int linesize)
{
    FILE * pFile = NULL;
    int nread;
    char **arrList;
    int i;
    int j;
    //이차원 배열 동적할당을 할때는 이런식으로 할것.
    arrList = (char**)(malloc(sizeof(char)*linesize));
    for(i =0; i<linesize;i++){
        arrList[i] = (char*)malloc(sizeof(char)*10);
    }
    int n = 0;
    char c;
    
    pFile = fopen(filename, "r");
    if(pFile!=NULL) {
        while(1) {
            for(j= 0 ; j<sizeof(buf)/sizeof(char); j++){
                c = fgetc(pFile);
                if(c!='\n'){
                buf[j] = c;
                }
                else if (c == '\n')
                {
                    break;
                }}
            
            arrList[n] = buf;
            n++;          
                
            
            
        }fclose(pFile);
        

        for(n=0;n<linesize;n++){
            printf("%02d: %s\n",n, arrList[n]);
            free(arrList[n]);
        }
        free(*arrList);
    }
}

int main(){
    char fn[]= "coref/male.txt";
    int noline;
    char maleList;

    
    noline =  nofFileLines(fn);
    //maleList = ArrayList(fn, noline);
     
    printf("%s %d\n ", "The number of lines:", noline);
    ArrayList(fn, noline);
    
    return 0;

}
