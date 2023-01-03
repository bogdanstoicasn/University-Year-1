#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "function.h"
#define NMAX 30
struct global_image
{
    char type[3];
    int width,height,maxValue,x_axis,y_axis;
    int **red,**red_crop;
    int **green,**green_crop;
    int **blue,**blue_crop;
};
int main() {
    char s[NMAX],parameter[NMAX],file[NMAX];
    struct global_image image;
    int type_determine=0,type,x1,x2,y1,y2,h1_stars=0,h2_bins=0,angle=0,count=0;
    FILE *fptr=NULL;
    while(1) {
        int oppp=operation_identifier(count,s,&x1,&y1,&x2,&y2,&h1_stars,&h2_bins,&angle,parameter,file,&type,image);
        switch(oppp) {
            case 1: 
            {
                int aux=type_determine;
                type_determine=file_type(fptr,s);
                if(type_determine!=0) {
                file_reader_first_version(&count,s,type_determine,fptr,&image);
                }
                else type_determine=aux;
                printf("%d\n",type_determine);
                break;
            }
            case 2://idk, it works
                select_function_integers(&image,x1,x2,y1,y2);
                printf("\n");
                break;
            case 3://lasi selectul anterior ca nu deranjaza
                select_function_all(&image);
                break;
            case 4:
                printf("%d-%d-Histogram\n",h1_stars,h2_bins);
                histogram_function(image,h1_stars,h2_bins);
                break;
            case 6:
                printf("ANGLE == %d \n",angle);
                rotate_function_helper(&image,angle);
                break;
            case 7:
                crop_function(&image);
                break;
            case 9:///works
                file_printer_for_tests(&type,file,image);
                break;
            case 10:
                free_global_matrix(type_determine,&image);
                break;
            default:
                printf("Invalid command\n");
                break;
        }
        if(oppp==10) break;
    }
    printf("%d",count);
    return 0;
}