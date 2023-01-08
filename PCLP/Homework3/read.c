
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"
#include "read.h"
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
void file_reader_first_version(int *count,char s[NMAX],int type,FILE *fptr,struct global_image* image)
{
    if(*count!=0) {
        free_global_matrix(image);
    }
    switch(type)
    {
        case 2:///pgm ascii
            text_file_reader_pgm_edition(s,fptr,image);
            (*count)=1;
            break;
        case 3:///ppm ascii
            text_file_reader_ppm_edition(s,fptr,image);
            (*count)=1;
            break;
        case 5:///pgm binary
            binary_file_reader_pgm_edition(s,fptr,image);
            (*count)=1;
            break;
        case 6:///ppm binary
            binary_file_reader_ppm_edition(s,fptr,image);
            (*count)=1;
            break;
        default:
            fprintf(stderr,"Failed to load %s\n",s);
            break;
    }
}

/// reads PGM files in ascii format 
void text_file_reader_pgm_edition(char s[NMAX],FILE *fptr,struct global_image* image)
{
    fptr=fopen(s,"rt");
    ignore_comments(fptr);
    fscanf(fptr,"%s",image->type);

    ignore_comments(fptr);
    int x,y;
    fscanf(fptr,"%d%d",&x,&y);
    image->x_axis=image->width=x;
    image->y_axis=image->height=y;

    ignore_comments(fptr);
    fscanf(fptr,"%d",&image->maxValue);

    ignore_comments(fptr);
    image->red=alloc_matrix(image->height,image->width);
    image->red_crop=alloc_matrix(image->height,image->width);
    
    if(!image->red)
    {
        printf("Failed to alloc for %s\n",s);
        return;
    }
    for(int i=0; i<image->height; i++)
    {
        for(int j=0; j<image->width; j++)
        {
            int x;
            fscanf(fptr,"%d",&x);
            
            image->red[i][j]=x;
            image->red_crop[i][j]=x;
        }
    }
    image->x_axis=image->width;
    image->y_axis=image->height;
    printf("Loaded %s\n",s);
    fclose(fptr);
}
/// reads PPM files in ascii format
void text_file_reader_ppm_edition(char s[NMAX],FILE *fptr,struct global_image* image)
{
    fptr=fopen(s,"rt");
    ignore_comments(fptr);
    fscanf(fptr,"%s",image->type);

    ignore_comments(fptr);
    int x,y;
    fscanf(fptr,"%d%d",&x,&y);
    image->x_axis=image->width=x;
    image->y_axis=image->height=y;

    ignore_comments(fptr);
    fscanf(fptr,"%d\n",&image->maxValue);

    ignore_comments(fptr);
    image->red=alloc_matrix(image->height,image->width);
    image->green=alloc_matrix(image->height,image->width);
    image->blue=alloc_matrix(image->height,image->width);

    image->red_crop=alloc_matrix(image->height,image->width);
    image->green_crop=alloc_matrix(image->height,image->width);
    image->blue_crop=alloc_matrix(image->height,image->width);
    if(!image->red || !image->green || !image->blue)
    {
        printf("Failed to alloc for %s\n",s);
        return ;
    }
    for(int i=0; i<image->height; i++)
    {
        for(int j=0; j<image->width; j++)
        {
            int x,y,z;
            fscanf(fptr,"%d%d%d",&x,&y,&z);
            image->red[i][j]=x;
            image->green[i][j]=y;
            image->blue[i][j]=z;

            image->red_crop[i][j]=x;
            image->green_crop[i][j]=y;
            image->blue_crop[i][j]=z;
        }
    }
    image->x_axis=image->width;
    image->y_axis=image->height;
    printf("Loaded %s\n",s);
    fclose(fptr);
}
/// reads PGM files in binary format
void binary_file_reader_pgm_edition(char s[NMAX],FILE *fptr, struct global_image* image)
{
    fptr=fopen(s,"rb");
    ignore_comments(fptr);
    fscanf(fptr,"%s",image->type);

    ignore_comments(fptr);
    int x,y;
    fscanf(fptr,"%d %d",&x,&y);
    image->x_axis=image->width=x;
    image->y_axis=image->height=y;

    ignore_comments(fptr);
    fscanf(fptr,"%d\n",&image->maxValue);
    
    image->red=alloc_matrix(image->height,image->width);
    image->red_crop=alloc_matrix(image->height,image->width);

    if(!image->red)
    {
        printf("Failed to alloc for %s\n",s);
        return ;
    }
    for(int i=0; i<image->height; i++)
    {
        for(int j=0; j<image->width; j++)
        {
            unsigned char c;
            fread(&c,sizeof(unsigned char),1,fptr);
            
            image->red[i][j]=(int)c;
            image->red_crop[i][j]=(int)c;
        }
    }
    image->x_axis=image->width;
    image->y_axis=image->height;
    printf("Loaded %s\n",s);
    fclose(fptr);
}
/// reads PPM files in binary format
void binary_file_reader_ppm_edition(char s[NMAX],FILE *fptr,struct global_image* image)
{
    fptr=fopen(s,"rb");
    ignore_comments(fptr);
    fscanf(fptr,"%s",image->type);

    ignore_comments(fptr);
    int x,y;
    fscanf(fptr,"%d%d",&x,&y);
    image->x_axis=image->width=x;
    image->y_axis=image->height=y;

    ignore_comments(fptr);
    fscanf(fptr,"%d",&image->maxValue);
    ignore_comments(fptr);

    image->red=alloc_matrix(image->height,image->width);
    image->green=alloc_matrix(image->height,image->width);
    image->blue=alloc_matrix(image->height,image->width);

    image->red_crop=alloc_matrix(image->height,image->width);
    image->green_crop=alloc_matrix(image->height,image->width);
    image->blue_crop=alloc_matrix(image->height,image->width);

    if(!image->red || !image->green || !image->blue)
    {
        printf("Failed to alloc for %s\n",s);
        return ;
    }
    for(int i=0; i<image->height; i++)
    {
        for(int j=0; j<image->width; j++)
        {
            unsigned char c;
            fread(&c,sizeof(unsigned char),1,fptr);
            image->red[i][j]=(int)c;
            image->red_crop[i][j]=(int)c;

            unsigned char d;
            fread(&d,sizeof(unsigned char),1,fptr);
            image->green[i][j]=(int)d;
            image->green_crop[i][j]=(int)d;

            unsigned char e;
            fread(&e,sizeof(unsigned char),1,fptr);
            image->blue[i][j]=(int)e;
            image->blue_crop[i][j]=(int)e;
        }
    }
    image->x_axis=image->width;
    image->y_axis=image->height;
    printf("Loaded %s\n",s);
    fclose(fptr);

}

