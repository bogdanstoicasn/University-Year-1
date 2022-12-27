#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "function.h"
#define NMAX 30
struct global_image
{
    char type[3];
    int width;
    int height;
    int maxValue;
    int **red;
    int **green;
    int **blue;
    int **red_crop;
    int **green_crop;
    int **blue_crop;
};
int main()
{
    char s[NMAX],parameter[NMAX];
    struct global_image image;
    int type_determine,type,x1,x2,y1,y2,h1=0,h2=0,angle=0,count=0;
    FILE *fptr=NULL;
    while(1)
    {
        int oppp=operation_identifier(s,&x1,&y1,&x2,&y2,&h1,&h2,&angle,parameter,image);
        switch(oppp)
        {
            case 1:
            {
                if(count!=0)
                {
                    free_global_matrix(type_determine,&image);
                    type_determine=file_type(fptr,s);
                    file_reader_first_version(s,type_determine,fptr,&image);
                }
                else{
                    type_determine=file_type(fptr,s);
                    if(type_determine!=0) {
                        count=1;
                        file_reader_first_version(s,type_determine,fptr,&image);
                    }
                }
                break;
            }
            case 2:
                printf("%d %d %d %d\n",x1,y1,x2,y2);
                break;
            case 3:
                printf("ALLLLLL\n");
                break;
            case 4:
                printf("%d-%d-Histogram\n",h1,h2);
                break;
            case 10:
                file_printer_for_tests(&type,image);
                free_global_matrix(type_determine,&image);
                break;
            default:
                printf("one operation no good param\n");
                break;
        }
        if(oppp==10) break;
    }
    //free_global_matrix(type_determine,&image);
    return 0;
}