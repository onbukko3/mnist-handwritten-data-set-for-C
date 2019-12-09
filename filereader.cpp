#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 20
#define MAX 3000

char buf[BUFFER_SIZE];
int Token(char* arrList[], char *inp);
//int nofFileLines (char *filename);





//int main(){
//    char fn[]= "coref/male.txt";
//    int noline;
//    char maleList;
    

    
//    noline =  nofFileLines(fn);
    

    //maleList = ArrayList(fn, noline);
     
//    printf("%s %d\n ", "The number of lines:", noline);
//    while (fgets)
   
    

//    return 0;

//}

int main ()
{
    FILE * pFile = NULL;
    int nread;
    char ch;
    int index;
    char* arrList[MAX];
    char* inp;
    char* c;
    inp = (char*)(malloc(sizeof(char)));

    pFile = fopen("coref/male.txt", "r");
    if(pFile!=NULL) {
        while(fgets(inp, MAX*MAX, pFile)) {
            index = Token(arrList, inp);

            //nread = fread(buf, sizeof(char), BUFFER_SIZE, pFile);
            //printf("%d",nread);
            //if(nread<=0) break;
            //for(int i=0; i<nread; i++) {
            //    if( buf[i] == '\n' ) 
            //        line++;
 //           }
        }fclose(pFile);
        //printf("%d", Token(arrList, inp).line);
    }
    return 0;
}

int Token(char* arrList[], char *inp){
    int i =0;
    
    int line = 0;

    char* c = strtok(inp, "\n");

    while(c != NULL){

        arrList[i] = c;
        printf("%s\n", arrList[i]);
        c= strtok(NULL, "\n");
        i++;
        line++;
        }
        return i;

}
