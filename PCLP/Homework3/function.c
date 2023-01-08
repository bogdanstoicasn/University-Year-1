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
            image->red[i][j]=round(final_result);
            if(image->red[i][j]>=255) image->red[i][j]=255;
            if(image->red[i][j]<0) image->red[i][j]=0;
        }
    }
    free(frequency);
    printf("Equalize done\n");
}
