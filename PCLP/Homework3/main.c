#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "function.h"
#define NMAX 50
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
    int file_type_determine=0,type,h1_stars=0,h2_bins=0,angle=0,count=0,load_one_time=0;
    int x1,x2,y1,y2;
    FILE *fptr=NULL;
    while(1) {
        int oppp=operation_identifier(count,s,&x1,&y1,&x2,&y2,&h1_stars,&h2_bins,&angle,parameter,file,&type,image);
        switch(oppp) {
            case 1: 
            {
                //int aux=file_type_determine;
                file_type_determine=file_type(fptr,s);
                if(file_type_determine==0 && count!=0) {free_global_matrix(file_type_determine,&image);count=0;}
                if(file_type_determine!=0) {
                file_reader_first_version(&count,s,file_type_determine,fptr,&image);
                load_one_time++;
                }
                break;
            }
            case 2://idk, it works
                select_function_integers(&image,x1,x2,y1,y2);
                break;
            case 3://lasi selectul anterior ca nu deranjaza
                select_function_all(&image,&x1,&y1,&x2,&y2);
                break;
            case 4:
                printf("%d-%d-Histogram\n",h1_stars,h2_bins);
                histogram_function(image,h1_stars,h2_bins);
                break;
            case 5:
                equalize_function(&image);
                break;
            case 6:
                rotate_function_helper(&image,angle);
                break;
            case 7:
                crop_function(&image);
                break;
            case 8:
                kernel_interface_helper(&image,parameter,x1,y1,x2,y2);
                break;
            case 9:///works
                file_printer_for_tests(&type,file,image);
                break;
            case 10:
                if(count !=0) free_global_matrix(file_type_determine,&image);
                if(count==0 && load_one_time!=0) printf("No image loaded\n");
                if(count==0 && load_one_time==0) printf("No image loaded\n");
                break;
            case 11:
                printf("No image loaded\n");
                break;
            default:
                break;
        }
        if(oppp==10) break;
    }
    return 0;
}
