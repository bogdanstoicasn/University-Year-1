#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "function.h"
#include "interface.h"
#define NMAX 50
struct global_image
{
    char type[3];
    int width,height,maxValue,x_axis,y_axis;
    int **red,**red_crop;
    int **green,**green_crop;
    int **blue,**blue_crop;
};
// Function to ignore any comments
// in file
int max_of_numbers(int a,int b)
{
    if(a>b) return a;
    else return b;
}

void swap_function(int* a,int* b)
{
    if(*a>*b){
        int aux=*a;
        *a=*b;
        *b=aux;
    }
}


int **alloc_matrix(int line, int coloumn) {
	int **a = (int **)malloc(line * sizeof(int *)); 
    if (!a) {
		fprintf(stderr, "malloc() failed\n");
		return NULL;
	}
	for (int i = 0; i < line; i++) {
		a[i] = (int *)malloc(coloumn * sizeof(int)); 
        if (!a[i]) {
			fprintf(stderr, "malloc() failed\n");
			while (--i >= 0) {
				free(a[i]);
			}
			free(a);
			return NULL;
		}
	}
	return a;
}

void free_matrix(int line,int **matrix) {
    for (int i = 0; i < line; i++) {
		free(matrix[i]);
	}
	free(matrix);
}
/// function which frees memory by type
void free_global_matrix(struct global_image* image)
{
    int type=image->type[1]-'0';
    switch(type)
    {
        case 2:
            free_matrix(image->height,image->red);
            free_matrix(image->y_axis,image->red_crop);
            break;
        case 3:
            {
            free_matrix(image->height,image->red);
            free_matrix(image->height,image->green);
            free_matrix(image->height,image->blue);
            free_matrix(image->y_axis,image->red_crop);
            free_matrix(image->y_axis,image->green_crop);
            free_matrix(image->y_axis,image->blue_crop);
            break;
            }
        case 5:
            free_matrix(image->height,image->red);
            free_matrix(image->y_axis,image->red_crop);
            break;
        case 6:
            {
            free_matrix(image->height,image->red);
            free_matrix(image->height,image->green);
            free_matrix(image->height,image->blue);
            free_matrix(image->y_axis,image->red_crop);
            free_matrix(image->y_axis,image->green_crop);
            free_matrix(image->y_axis,image->blue_crop);
            break;
            }
        default:
            fprintf(stderr,"Failed to free\n");
            break;
    }
}


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

void file_printer_for_tests(int *type,char file[NMAX],struct global_image image)
{   
    FILE *output;
    if(*type==2 || *type==5) output=fopen(file,"wt");
    else output=fopen(file,"wb");
    switch (*type)
    {
    case 2:
        fprintf(output,"P2\n%d %d\n%d\n",image.width,image.height,image.maxValue);
        for(int i=0; i<image.height; i++)
        {
            for(int j=0; j<image.width; j++)
            {
                fprintf(output,"%d",image.red[i][j]);
                if(j<image.width-1) fprintf(output," ");
            }
            fprintf(output,"\n");
        }

        printf("Saved %s\n",file);
        break;
    case 3:
        fprintf(output,"P3\n%d %d\n%d\n",image.width,image.height,image.maxValue);
        for(int i=0; i<image.height; i++)
        {
            for(int j=0; j<image.width; j++)
            {
                fprintf(output,"%d %d %d ",image.red[i][j],image.green[i][j],image.blue[i][j]);
            }
            fprintf(output,"\n");
        }

        printf("Saved %s\n",file);
        break;
    case 5:
        fprintf(output,"P5\n%d %d\n%d\n",image.width,image.height,image.maxValue);
        for(int i=0; i<image.height; i++)
        {
            for(int j=0; j<image.width; j++)
            {
                unsigned char c=(unsigned char)image.red[i][j];
                fwrite(&c,sizeof(unsigned char),1,output);
            }
        }

        printf("Saved %s\n",file);
        break;
    case 6:
        fprintf(output,"P6\n%d %d\n%d\n",image.width,image.height,image.maxValue);
        for(int i=0; i<image.height; i++)
        {
            for(int j=0; j<image.width; j++)
            {
                unsigned char c=(unsigned char)image.red[i][j];
                fwrite(&c,sizeof(unsigned char),1,output);

                unsigned char d=(char)image.green[i][j];
                fwrite(&d,sizeof(unsigned char),1,output);
                
                unsigned char e=(char)image.blue[i][j];
                fwrite(&e,sizeof(unsigned char),1,output);
            }
        }

        printf("Saved %s\n",file);
        break;
    default:
        printf("Failed to load %s\n",file);
        break;
    }
    fclose(output);
}


// de vazut daca se face select dupa select sau se ia de pe matricea principala
void select_function_integers(struct global_image *image,int x1,int x2,int y1,int y2)
{
    if(image->type[1]=='2' || image->type[1]=='5')
    {
        free_matrix(image->y_axis,image->red_crop);
        image->x_axis=x2-x1;
        image->y_axis=y2-y1;
        image->red_crop=alloc_matrix(image->y_axis,image->x_axis);
        if(!image->red_crop) {
            fprintf(stderr,"Alloc failed\n");
        }
        
        for(int i=y1; i<y2; i++)
        {
            for(int j=x1; j<x2; j++)
            {
                image->red_crop[i-y1][j-x1]=image->red[i][j];
            }
        }

        printf("Selected %d %d %d %d\n",x1,y1,x2,y2);
    }
    if(image->type[1]=='3' || image->type[1]=='6')
    {
        free_matrix(image->y_axis,image->red_crop);
        free_matrix(image->y_axis,image->green_crop);
        free_matrix(image->y_axis,image->blue_crop);
        
        image->x_axis=x2-x1;
        image->y_axis=y2-y1;

        image->red_crop=alloc_matrix(image->y_axis,image->x_axis);
        image->green_crop=alloc_matrix(image->y_axis,image->x_axis);
        image->blue_crop=alloc_matrix(image->y_axis,image->x_axis);
        if(!image->red_crop) {
            fprintf(stderr,"Alloc failed\n");
        }
        if(!image->green_crop) {
            free_matrix(y2-y1,image->red_crop);
            fprintf(stderr,"ALLOc failed\n");
        }
        if(!image->blue_crop) {
            free_matrix(y2-y1,image->red_crop);
            free_matrix(y2-y1,image->green_crop);
            fprintf(stderr,"Alloc failed\n");
        }

        for(int i=y1; i<y2; i++)
        {
            for(int j=x1; j<x2; j++)
            {
                image->red_crop[i-y1][j-x1]=image->red[i][j];
                image->green_crop[i-y1][j-x1]=image->green[i][j];
                image->blue_crop[i-y1][j-x1]=image->blue[i][j];
            }
        }
        printf("Selected %d %d %d %d\n",x1,y1,x2,y2);
    }
}

void select_function_all(struct global_image *image,int *x1,int *y1,int *x2,int *y2)
{
    if(image->type[1]=='2' || image->type[1]=='5')
    {
        free_matrix(image->y_axis,image->red_crop);
        image->red_crop=alloc_matrix(image->height,image->width);
        if(!image->red_crop) {
            fprintf(stderr,"Alloc failed\n");
        }

        image->x_axis=image->width;
        image->y_axis=image->height;

        for(int i=0; i<image->height; i++)
        {
            for(int j=0; j<image->width; j++)
                image->red_crop[i][j]=image->red[i][j];
        }

        image->x_axis=image->width;
        image->y_axis=image->height;

        *x1=*y1=0; *x2=image->width; *y2=image->height;

        printf("Selected ALL\n");

    }
    if(image->type[1]=='3' || image->type[1]=='6')
    {
        free_matrix(image->y_axis,image->red_crop);
        free_matrix(image->y_axis,image->green_crop);
        free_matrix(image->y_axis,image->blue_crop);

        image->red_crop=alloc_matrix(image->height,image->width);
        image->blue_crop=alloc_matrix(image->height,image->width);
        image->green_crop=alloc_matrix(image->height,image->width);

        if(!image->red_crop) {
            fprintf(stderr,"Alloc failed\n");
        }
        if(!image->green_crop) {
            free_matrix(image->height,image->red_crop);
            fprintf(stderr,"Alloc failed\n");
        }
        if(!image->blue_crop) {
            free_matrix(image->height,image->red_crop);
            free_matrix(image->height,image->green_crop);
            fprintf(stderr,"Alloc failed\n");
        }

        image->x_axis=image->width;
        image->y_axis=image->height;

        for(int i=0; i<image->height; i++)
        {
            for(int j=0; j<image->width; j++)
            {
                image->red_crop[i][j]=image->red[i][j];
                image->green_crop[i][j]=image->green[i][j];
                image->blue_crop[i][j]=image->blue[i][j];
            }
        }

        *x1=*y1=0; *x2=image->width; *y2=image->height;

        printf("Selected ALL\n");
    }
}

void crop_function(struct global_image *image)
{
    if(image->type[1]=='2' || image->type[1]=='5')
    {
        free_matrix(image->height,image->red);

        image->red=alloc_matrix(image->y_axis,image->x_axis);
        if(!image->red) {
            fprintf(stderr,"Alloc failed\n");
        }

        image->width=image->x_axis;
        image->height=image->y_axis;

        for(int i=0; i<image->y_axis; i++)
        {
            for(int j=0; j<image->x_axis; j++)
            {
                image->red[i][j]=image->red_crop[i][j];
            }
        }
    }
    if(image->type[1]=='3' || image->type[1]=='6')
    {
        free_matrix(image->height,image->red);
        free_matrix(image->height,image->green);
        free_matrix(image->height,image->blue);

        image->red=alloc_matrix(image->y_axis,image->x_axis);
        image->green=alloc_matrix(image->y_axis,image->x_axis);
        image->blue=alloc_matrix(image->y_axis,image->x_axis);

        if(!image->red) {
            fprintf(stderr,"Alloc failed\n");
        }
        if(!image->green) {
            free_matrix(image->y_axis,image->red);
            fprintf(stderr,"Alloc failed\n");
        }
        if(!image->blue) {
            free_matrix(image->y_axis,image->red);
            free_matrix(image->y_axis,image->green);
            fprintf(stderr,"Alloc failed\n");
        }

        image->width=image->x_axis;
        image->height=image->y_axis;

        for(int i=0; i<image->y_axis; i++)
        {
            for(int j=0; j<image->x_axis; j++)
            {
                image->red[i][j]=image->red_crop[i][j];
                image->green[i][j]=image->green_crop[i][j];
                image->blue[i][j]=image->blue_crop[i][j];
            }
        }

    }
}

void rotate_function_helper(struct global_image *image,int angle,int x1,int y1,int x2,int y2)
{
    //if(image->x_axis!=image->y_axis) {printf("The selection must be square\n");return ;}
    switch(angle)
    {
        case 90:
            clockwise_rotation_90(image,x1,y1,x2,y2);
            printf("Rotated 90\n");
            break;
        case -90:
            counter_clockwise_rotation_90(image,x1,y1,x2,y2);
            printf("Rotated -90\n");
            break;
        case 180:
            clockwise_rotation_90(image,x1,y1,x2,y2);
            clockwise_rotation_90(image,x1,y1,x2,y2);
            printf("Rotated 180\n");
            break;
        case -180:
            counter_clockwise_rotation_90(image,x1,y1,x2,y2);
            counter_clockwise_rotation_90(image,x1,y1,x2,y2);
            printf("Rotated -180\n");
            break;
        case 270:
            counter_clockwise_rotation_90(image,x1,y1,x2,y2);
            printf("Rotated 270\n");
            break;
        case -270:
            clockwise_rotation_90(image,x1,y1,x2,y2);
            printf("Rotated -270\n");
            break;
        case 360:
            printf("Rotated 360\n");
            break;
        case -360:
            printf("Rotated -360\n");
            break;
        case 0:
            printf("Rotated 0\n");
            break;
        default:
            printf("Unsupported rotation angle\n");
            break;
    }
}
// both rotations are done and they work as intended 
void counter_clockwise_rotation_90(struct global_image *image,int x1,int y1,int x2,int y2)
{
    ///merge doar o
    if(image->type[1]=='2' || image->type[1]=='5') {
        int **output=alloc_matrix(image->x_axis,image->y_axis);
        if(!output) {
            printf("Failed to rotate\n");
            return ;
        }

        for(int i=0; i<image->y_axis; i++)
            for(int j=0; j<image->x_axis; j++)   
                output[image->x_axis-j-1][i] = image->red_crop[i][j];
        
        free_matrix(image->y_axis,image->red_crop);
        
        int tmp=image->x_axis;
        image->x_axis=image->y_axis;
        image->y_axis=tmp;

        image->red_crop=output;
        int count_image=0, count_select=0;
        for(int i=0; i<image->height; i++)
            for(int j=0; j<image->width; j++)
                count_image++;

        for(int i=0; i<image->x_axis; i++)
            for(int j=0; j<image->y_axis; j++)
                count_select++;

        if(count_image==count_select) {
            free_matrix(image->height,image->red);
            image->red=alloc_matrix(image->width,image->height);
            int aux=image->width;
            image->width=image->height;
            image->height=aux;
            for(int i=0; i<image->height; i++)
                for(int j=0; j<image->width; j++)
                    image->red[i][j]=image->red_crop[i][j];
            
        }
        else {
            for(int i=y1; i<y2; i++)
            {
                for(int j=x1; j<x2; j++)
                {
                    image->red[i][j]=image->red_crop[i-y1][j-x1];
                }
            }
        }

    }

    if(image->type[1]=='3' || image->type[1]=='6') {
        int **output=alloc_matrix(image->x_axis,image->y_axis);
        int **output_b=alloc_matrix(image->x_axis,image->y_axis);
        int **output_g=alloc_matrix(image->x_axis,image->y_axis);
        if(!output) {
            printf("Failed to rotate\n");
            return ;
        }

        if(!output_b) {
            free_matrix(image->x_axis,output);
            printf("Failed to rotate\n");
            return ;
        }

        if(!output_g) {
            free_matrix(image->x_axis,output);
            free_matrix(image->x_axis,output_b);
            printf("Failed to rotate\n");
            return ;
        }

        for(int i=0; i<image->y_axis; i++)
            for(int j=0; j<image->x_axis; j++)   
            {
                output[image->x_axis-j-1][i] = image->red_crop[i][j];
                output_b[image->x_axis-j-1][i] = image->blue_crop[i][j];
                output_g[image->x_axis-j-1][i] = image->green_crop[i][j];

            }
        
        free_matrix(image->y_axis,image->red_crop);
        free_matrix(image->y_axis,image->blue_crop);
        free_matrix(image->y_axis,image->green_crop);

        int tmp=image->x_axis;
        image->x_axis=image->y_axis;
        image->y_axis=tmp;

        image->red_crop=output;
        image->blue_crop=output_b;
        image->green_crop=output_g;

        int count_image=0, count_select=0;
        for(int i=0; i<image->height; i++)
            for(int j=0; j<image->width; j++)
                count_image++;

        for(int i=0; i<image->x_axis; i++)
            for(int j=0; j<image->y_axis; j++)
                count_select++;

        if(count_image==count_select) {
            free_matrix(image->height,image->red);
            free_matrix(image->height,image->blue);
            free_matrix(image->height,image->green);
            image->red=alloc_matrix(image->width,image->height);
            image->green=alloc_matrix(image->width,image->height);
            image->blue=alloc_matrix(image->width,image->height);
            int aux=image->width;
            image->width=image->height;
            image->height=aux;
            for(int i=0; i<image->height; i++)
                for(int j=0; j<image->width; j++)
                    {
                        image->red[i][j]=image->red_crop[i][j];
                        image->blue[i][j]=image->blue_crop[i][j];
                        image->green[i][j]=image->green_crop[i][j];
                    
                    }
        }
        else {
            for(int i=y1; i<y2; i++)
            {
                for(int j=x1; j<x2; j++)
                {
                    image->red[i][j]=image->red_crop[i-y1][j-x1];
                    image->blue[i][j]=image->blue_crop[i-y1][j-x1];
                    image->green[i][j]=image->green_crop[i-y1][j-x1];
                }
            }
        }
    }
}

void clockwise_rotation_90(struct global_image *image,int x1,int y1,int x2,int y2)
{
    if(image->type[1]=='2' || image->type[1]=='5') {
        int **output=alloc_matrix(image->x_axis,image->y_axis);
        if(!output) {
            printf("Failed to rotate\n");
        }

        for(int i=0; i<image->y_axis; i++)
            for(int j=0; j<image->x_axis; j++)   
                output[j][image->y_axis-1-i] = image->red_crop[i][j];
        
        free_matrix(image->y_axis,image->red_crop);
        int tmp=image->x_axis;
        image->x_axis=image->y_axis;
        image->y_axis=tmp;

        image->red_crop=output;

        int count_image=0, count_select=0;
        for(int i=0; i<image->height; i++)
            for(int j=0; j<image->width; j++)
                count_image++;

        for(int i=0; i<image->x_axis; i++)
            for(int j=0; j<image->y_axis; j++)
                count_select++;

        if(count_image==count_select) {
            free_matrix(image->height,image->red);
            image->red=alloc_matrix(image->width,image->height);
            int aux=image->width;
            image->width=image->height;
            image->height=aux;
            for(int i=0; i<image->height; i++)
                for(int j=0; j<image->width; j++)
                    image->red[i][j]=image->red_crop[i][j];
        }
        else {
            for(int i=y1; i<y2; i++)
            {
                for(int j=x1; j<x2; j++)
                {
                    image->red[i][j]=image->red_crop[i-y1][j-x1];
                }
            }
        }
    }

    if(image->type[1]=='3' || image->type[1]=='6') {
        int **output=alloc_matrix(image->x_axis,image->y_axis);
        int **output_b=alloc_matrix(image->x_axis,image->y_axis);
        int **output_g=alloc_matrix(image->x_axis,image->y_axis);
        if(!output) {
            printf("Failed to rotate\n");
            return ;
        }

        if(!output_b) {
            free_matrix(image->x_axis,output);
            printf("Failed to rotate\n");
            return ;
        }

        if(!output_g) {
            free_matrix(image->x_axis,output);
            free_matrix(image->x_axis,output_b);
            printf("Failed to rotate\n");
            return ;
        }

        for(int i=0; i<image->y_axis; i++)
            for(int j=0; j<image->x_axis; j++)   
            {
                output[j][image->y_axis-1-i] = image->red_crop[i][j];
                output_b[j][image->y_axis-1-i] = image->blue_crop[i][j];
                output_g[j][image->y_axis-1-i] = image->green_crop[i][j];

            }
        
        free_matrix(image->y_axis,image->red_crop);
        free_matrix(image->y_axis,image->blue_crop);
        free_matrix(image->y_axis,image->green_crop);


        int tmp=image->x_axis;
        image->x_axis=image->y_axis;
        image->y_axis=tmp;

        image->red_crop=output;
        image->blue_crop=output_b;
        image->green_crop=output_g;

        int count_image=0, count_select=0;
        for(int i=0; i<image->height; i++)
            for(int j=0; j<image->width; j++)
                count_image++;

        for(int i=0; i<image->x_axis; i++)
            for(int j=0; j<image->y_axis; j++)
                count_select++;

        if(count_image==count_select) {
            free_matrix(image->height,image->red);
            free_matrix(image->height,image->blue);
            free_matrix(image->height,image->green);
            image->red=alloc_matrix(image->width,image->height);
            image->green=alloc_matrix(image->width,image->height);
            image->blue=alloc_matrix(image->width,image->height);
            int aux=image->width;
            image->width=image->height;
            image->height=aux;
            for(int i=0; i<image->height; i++)
                for(int j=0; j<image->width; j++)
                    {
                        image->red[i][j]=image->red_crop[i][j];
                        image->blue[i][j]=image->blue_crop[i][j];
                        image->green[i][j]=image->green_crop[i][j];
                    
                    }
        }
        else {
            for(int i=y1; i<y2; i++)
            {
                for(int j=x1; j<x2; j++)
                {
                    image->red[i][j]=image->red_crop[i-y1][j-x1];
                    image->blue[i][j]=image->blue_crop[i-y1][j-x1];
                    image->green[i][j]=image->green_crop[i-y1][j-x1];
                }
            }
        }

    }
}


void histogram_function(struct global_image image,int h1_star,int h2_bins)
{
    int i,j;
    long long *histogram_map=NULL,max_frequency=0;
    histogram_map=calloc(h2_bins,sizeof(long long));
    if(!histogram_map) {
        printf("Alloc failed\n");
        return ;
    }
    int step=(image.maxValue+1)/h2_bins;
    for(i=0; i<image.height; i++)
        for(j=0; j<image.width; j++)
            histogram_map[image.red[i][j]/step]++;
            
    for(i=0; i<h2_bins; i++)
        if(histogram_map[i]>max_frequency) max_frequency=histogram_map[i];
    
    for(i=0; i<h2_bins; i++) {
        double length_of_star=(histogram_map[i]*h1_star/max_frequency);
        
        printf("%d\t|\t",(int)length_of_star);
        
        for(j=0; j<ceil(length_of_star); j++)
            printf("*");
        printf("\n");
    }
    free(histogram_map);
}


void equalize_function(struct global_image *image)
{
    int *frequency=calloc(256,sizeof(int));
    if(!frequency) {
        printf("Alloc failed\n");
        return ;
    }
    for(int i=0; i<image->height; i++)
        for(int j=0; j<image->width; j++)
            frequency[(image->red[i][j])]++;
    

    for(int i=0; i<image->height; i++)
    {
        for(int j=0; j<image->width; j++)
        {
            double sum=0;
            for(int k=0; k<image->red[i][j]; k++)
                sum+=frequency[k];
            
            double final_result,area;
            area=image->width*image->height;
            final_result=255*sum/area;
            image->red[i][j]= round(final_result);
            if(image->red[i][j]>255) image->red[i][j]=255;
            if(image->red[i][j]<0) image->red[i][j]=0;
        }
    }
    free(frequency);
    printf("Equalize done\n");
}


int **kernel_matrixes_creator(char parameter[NMAX])
{
    int i,j;
	int **kernel=alloc_matrix(3,3);

    if(!kernel) {
        printf("Alloc failed");
        return NULL;
    }


    if(strcmp(parameter,"EDGE")==0) {
        for(i=0; i<3; i++)
			for(j=0; j<3; j++)
				kernel[i][j]=-1;
		kernel[1][1]=8;
    }

    if(strcmp(parameter,"SHARPEN")==0) {
        for(i=0; i<3; i++)
			for(j=0; j<=i; j++)
				if((i+j)%2==0)
					kernel[i][j]=kernel[j][i]=0;
				else
					kernel[i][j]=kernel[j][i]=-1;
		kernel[1][1]=5;
    }

    if(strcmp(parameter,"BLUR")==0) {
        for(i=0; i<3; i++)
			for(j=0; j<3; j++)
				kernel[i][j]=1;
    }

    if(strcmp(parameter,"GAUSSIAN_BLUR")==0) {
        for(i=0; i<3; i++)
			for(j=0; j<=i; j++)
				if((i+j)%2==0)
					kernel[i][j]=kernel[j][i]=1;
				else
					kernel[i][j]=kernel[j][i]=2;
		kernel[1][1]=4;
    }
	
	return kernel;
}

void checker_0_and_255_case(double *a)
{
    if(*a<0) *a=0;
    if(*a>255) *a=255;
}
void apply_kernel(struct global_image *image,int x1,int y1,int x2,int y2,int **kernel,int div)
{
    for(int i=y1; i<y2; i++)
    {
        for(int j=x1; j<x2; j++)
        {
            double sum_r=0,sum_g=0,sum_b=0;
            int ok=0;
            for(int m=-1; m<=1; m++) 
            {
                for(int n=-1; n<=1; n++)
                {
                    if(i>0 && i<image->height && j>0 && j<image->width) 
                        {
                            sum_r+=image->red[i+m][j+n]*kernel[m+1][n+1];
                            sum_g+=image->green[i+m][j+n]*kernel[m+1][n+1];
                            sum_b+=image->blue[i+m][j+n]*kernel[m+1][n+1];
                            ok++;
                        }
                }
            }
            sum_r/=div;
            sum_g/=div;
            sum_b/=div;

            checker_0_and_255_case(&sum_b);
            checker_0_and_255_case(&sum_g);
            checker_0_and_255_case(&sum_r);
            if(ok!=0)
            {image->red_crop[i-y1][j-x1]=round(sum_r);
            image->blue_crop[i-y1][j-x1]=round(sum_b);
            image->green_crop[i-y1][j-x1]=round(sum_g);}   
        }
    }
    for(int i=y1; i<y2; i++)
    {
        for(int j=x1; j<x2; j++)
        {
            image->red[i][j]=image->red_crop[i-y1][j-x1];
            image->green[i][j]=image->green_crop[i-y1][j-x1];
            image->blue[i][j]=image->blue_crop[i-y1][j-x1];
        }
    }
    free_matrix(3,kernel);
}

void kernel_interface_helper(struct global_image *image,char parameter[NMAX],int x1,int y1,int x2,int y2)
{
    int **kernel=kernel_matrixes_creator(parameter);

    if(!kernel) {
        printf("Alloc failed");
        return ;
    }

    if(strcmp(parameter,"EDGE")==0) {
        apply_kernel(image,x1,y1,x2,y2,kernel,1);
    }

    if(strcmp(parameter,"SHARPEN")==0) {
        apply_kernel(image,x1,y1,x2,y2,kernel,1);
    }

    if(strcmp(parameter,"BLUR")==0) {
        apply_kernel(image,x1,y1,x2,y2,kernel,9);
    }

    if(strcmp(parameter,"GAUSSIAN_BLUR")==0) {
        apply_kernel(image,x1,y1,x2,y2,kernel,16);
    }
    printf("APPLY %s done\n",parameter);
}


