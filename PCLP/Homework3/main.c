#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "function.h"
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
    int contor=0;
    struct global_image image;
    FILE *pointer=fopen("cup.pgm","r"),*out=fopen("out.pgm","w");
    open_text_file(&image,pointer);
    print_text_file(image,out);
    contor=1;
    if(contor==1) free_matrix(image.height,image.red);
    return 0;
}