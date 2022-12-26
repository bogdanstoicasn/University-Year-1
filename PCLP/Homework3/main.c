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
};
int main()
{
    char s[NMAX];
    struct global_image image;
    int type_determine,type;
    FILE *fptr=NULL;
    type_determine=file_type(fptr,s);
    printf("!!!!!!%d\n",type_determine);
    if(type_determine!=0)
    {
        file_reader_first_version(s,type_determine,fptr,&image);
    printf("type==%d\n%s\n%d %d\n%d\n",type_determine,image.type,image.width,image.height,image.maxValue);
    file_printer_for_tests(&type,image);
    free_global_matrix(type_determine,&image);
    }
    else printf("workkkk");
    return 0;
}