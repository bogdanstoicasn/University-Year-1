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

// Function to ignore any comments
// in file
void ignore_comments(FILE* fp)
{
    int ch;
    char line[100];
 
    // Ignore any blank lines
    while ((ch = fgetc(fp)) != EOF
           && isspace(ch))
        ;
 
    // Recursively ignore comments
    // in a PGM image commented lines
    // start with a '#'
    if (ch == '#') {
        fgets(line, sizeof(line), fp);
        ignore_comments(fp);
    }
    else
        fseek(fp, -1, SEEK_CUR);
}///se aplica  dupa fiecre citire
void open_text_file(struct global_image *image,FILE *fptr)
{
    char s[3];
    int a,b,c;
   fscanf(fptr,"%s",s);
   strcpy(image->type,s);
   fscanf(fptr,"%d%d%d",&a,&b,&c);
   image->width=a;
   image->height=b;
   image->maxValue=c;
   image->red=alloc_matrix(image->height,image->width);
   if(!image->red)
   {
       fprintf(stderr,"structure not alloc\n");
   }
   for(int i=0; i<image->height; i++)
   {
       for(int j=0; j<image->width; j++)
       {
           int x;
           fscanf(fptr,"%d",&x);
           image->red[i][j]=x;
       }
   }
   fclose(fptr);
   printf("loaded\n");

}
void print_text_file(struct global_image image,FILE *output)
{
    fprintf(output,"%s\n%d %d\n%d\n",image.type,image.width,image.height,image.maxValue);
    for(int i=0; i<image.height; i++)
    {
        for(int j=0; j<image.width; j++)
        {
            fprintf(output,"%d",image.red[i][j]);
            if(j<image.width-1) fprintf(output," ");
        }
        fprintf(output,"\n");
    }
    fclose(output);
}