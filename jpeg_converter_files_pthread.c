#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <jpeglib.h>
#include <setjmp.h>
#include <pthread.h>
#include "linkedlist.h"
#include "bmp.h"

#define INVENTORY_MAX_STRING_SIZE 1000
#define MAX_FOLDER_NAME 5012

static pthread_mutex_t mutex_lock_input;


struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

// JSAMPARRAY buffer;

static linkedList *L;
node *p;


METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{

    my_error_ptr myerr = (my_error_ptr) cinfo->err;

    (*cinfo->err->output_message) (cinfo);

    longjmp(myerr->setjmp_buffer,1);
}

char *changing_ext(char* filename)
{
    char *bmp = ".bmp";
    strtok(filename, ".");
    strcat(filename,bmp);

    return filename;
}

GLOBAL(int)
convert_file (char *filename)
{
    struct jpeg_decompress_struct cinfo;

    BITMAPFILEHEADER _bfh;
    BITMAPINFOHEADER _bih;
    struct my_error_mgr jerr;
    FILE *srcfile;
    int row_stride;
    unsigned char *buffer[1];
    unsigned char *buffer_bmp;


    if((srcfile = fopen(filename,"rb")) == NULL)
    {
        fprintf(stderr, "cannot open %s\n", filename);
        return 0;
    }

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;

    if(setjmp(jerr.setjmp_buffer)){
        jpeg_destroy_decompress(&cinfo);
        fclose(srcfile);
        return 0;
    }

    jpeg_create_decompress(&cinfo);

    jpeg_stdio_src(&cinfo, srcfile);

    (void) jpeg_read_header(&cinfo, TRUE);

    (void) jpeg_start_decompress(&cinfo);

    row_stride = cinfo.output_width * cinfo.output_components;


    // buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);
    buffer_bmp = (unsigned char*)malloc(sizeof(unsigned char*)*row_stride*cinfo.output_height);
    int i  = 0;
    int j = 0;
    while (cinfo.output_scanline < cinfo.output_height)
    {   

        buffer[0] = buffer_bmp + (cinfo.output_scanline) * row_stride;    
        (void) jpeg_read_scanlines(&cinfo, &buffer[0], 1);
        // memcpy(buffer_bmp, *buffer, sizeof(buffer));   
        
    }
    // printf("the number of size: %ld\n", sizeof(buffer_bmp));
    (void) jpeg_finish_decompress(&cinfo);

    jpeg_destroy_decompress(&cinfo);

    fclose(srcfile);
    
    // free(buffer);

    //adding bmp infos in file and info header
    //bmp file header
    _bfh.bfType = ((unsigned int)('M' << 8) | 'B');
    _bfh.bfReserved1 = 0;
    _bfh.bfReserved2 = 0;
    _bfh.bfSize = cinfo.output_height*cinfo.output_components + 54;
    _bfh.bfOffBits = 54;
    
    //bmp info header
    _bih.biSize = 40;
    _bih.biBitCount = cinfo.output_components*8;
    _bih.biWidth = cinfo.output_width;
    _bih.biHeight = cinfo.output_height;
    _bih.biSizeImage = _bih.biWidth * _bih.biHeight*cinfo.output_components;
    
    changing_ext(filename);

    //starting to convert to bmp file

    FILE *tgtFile;

    if((tgtFile = fopen(filename,"wb")) == NULL)
    {
        fprintf(stderr, "cannot write %s\n", filename);
        return 0;
    }
   
    int wid_padding=0;
    // int hei_padding=0;
    int WIDTH=0;
    int Height = 0;
    int count=0;
    int width = _bih.biWidth;
    int height = _bih.biHeight;

    if(tgtFile!=NULL)
    {

        fwrite(&_bfh, 1, sizeof(BITMAPFILEHEADER), tgtFile);
        fwrite(&_bih, 1, sizeof(BITMAPINFOHEADER), tgtFile);
        // traverse the picture 
        
        int count = _bih.biBitCount;
        wid_padding = ((PIXEL_ALIGN - ((width * count / 8) % PIXEL_ALIGN)) % PIXEL_ALIGN);
        // hei_padding = PIXEL_ALIGN - (height % PIXEL_ALIGN);
        int pixelsize = getPixelSize(_bih.biBitCount);
        WIDTH = width*pixelsize + wid_padding;
        long size = WIDTH * height;
        char temp;

        unsigned char *image = (unsigned char *)malloc(size);


        for(int i =0; i<size/2; ++i )
        {
            temp = buffer_bmp[i];
            buffer_bmp[i] = buffer_bmp[size -1 -i];
            buffer_bmp[size -1 -i] = temp;
        }
        
        memcpy(image, buffer_bmp, size);

        int HEIGHT = _bih.biHeight -1;
        for(i=HEIGHT;i>=0 ; i--)
        {
            for(j=0;j<(_bih.biWidth);j++)
            {
                pRGBTRIPLE pRGB = (pRGBTRIPLE)&buffer_bmp[(i * WIDTH) + (j * pixelsize)];
                pRGB -> rgbtBlue = image[(i * WIDTH) + ((_bih.biWidth - j) * pixelsize)];
                pRGB -> rgbtGreen = image[(i * WIDTH) + ((_bih.biWidth - j) * pixelsize)+1];
                pRGB -> rgbtRed = image[(i * WIDTH) + ((_bih.biWidth - j) * pixelsize)+2];  

            }
            
        }

        fwrite(buffer_bmp,1,size,tgtFile);
    }

    fclose(tgtFile);
    free(buffer_bmp);

    return 1;
    
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
    char buf[1024];

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
                                    int size = strlen(file_loc);
                                    createNode(L, file_loc);
                                    free(filename);
                                    filename = NULL;
                                    if(file_loc != NULL)
                                    {
                                        free(file_loc);
                                        file_loc = NULL;
                                    }
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


char *getNextFilename(void)
{
    char *filename;
    if(p != NULL)
    {
        filename = p->data;
        p = p->next;
    }

    return filename;
}

void *process(void *data)
{
    // pid_t pid;
    pthread_t tid;
    char *presentFile;
    int ret;

    // pid = getpid();
    tid = pthread_self();
    while(1)
    {
        pthread_mutex_lock(&mutex_lock_input);
        presentFile = getNextFilename();
        if(presentFile ==NULL)
        {
            break;
        }
        printf("%ld \t %s\n", tid, presentFile);
        // sleep(1);
        pthread_mutex_unlock(&mutex_lock_input);

        
        convert_file(presentFile);
        // convert_jpeg_to_bmp();
        // changing_ext(presentFile);
        // write_bmp_file(presentFile);

        if(p == L->tail)
        {
            break;
        }
    }
    
    pthread_exit(&ret);
}

int getCorecnt(void)
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}


int main()
{
    char *path;
    path = (char*)malloc(sizeof(char*)*MAX_FOLDER_NAME);
    path = "/home/hsji/study/clanguage";

    L = (linkedList*)malloc(sizeof(linkedList));
    L->head = NULL;
    L->tail = NULL; 
  
    getfiles(path);
  
    int threadCnt;
    int count = 0;
    pthread_t *pthread;
    int *thr_id;
    int *status;

    p = L->head;


    threadCnt = getCorecnt();
    pthread = (pthread_t*)malloc(sizeof(pthread_t)*threadCnt);
    thr_id = (int*)malloc(sizeof(int)*threadCnt);
    status = (int*)malloc(sizeof(int)*threadCnt);

    pthread_mutex_init(&mutex_lock_input, NULL);
    for(count; count<threadCnt; count++)
    {
        thr_id[count] = pthread_create(&pthread[count], NULL, process, (void*)NULL);
        // printf("%ln\n", pthread);
        
        if(thr_id<0)
            {
                perror("thread creates error: ");
                exit(0);
            }

    }

    for(count=0; count<threadCnt; count++)
    {
        pthread_join(pthread[count],(void **)&status[count]);
    }

    free(pthread);
    free(thr_id);
    free(status);

    deleteAllNode(L);

    return 0;

}