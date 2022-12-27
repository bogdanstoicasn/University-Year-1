#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "function.h"
#include <string.h>
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
// Function to ignore any comments
// in file
int max_of_numbers(int a,int b)
{
    if(a>b) return a;
    else return b;
}
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
void free_global_matrix(int type,struct global_image* image)
{
    switch(type)
    {
        case 2:
            free_matrix(image->height,image->red);
            //free_matrix(image->height,image->red_crop);
            break;
        case 3:
            {
            free_matrix(image->height,image->red);
            free_matrix(image->height,image->green);
            free_matrix(image->height,image->blue);
            //free_matrix(image->height,image->red_crop);
            //free_matrix(image->height,image->green_crop);
            //free_matrix(image->height,image->blue_crop);
            break;
            }
        case 5:
            free_matrix(image->height,image->red);
            //free_matrix(image->height,image->red_crop);
            break;
        case 6:
            {
            free_matrix(image->height,image->red);
            free_matrix(image->height,image->green);
            free_matrix(image->height,image->blue);
            //free_matrix(image->height,image->red_crop);
            //free_matrix(image->height,image->green_crop);
            //free_matrix(image->height,image->blue_crop);
            break;
            }
        default:
            fprintf(stderr,"Failed to free\n");
            break;
    }
}
void print_matrix(struct global_image element) {
        for(int i=0; i<element.height; i++) {
            for(int j=0; j<element.width; j++) {
                printf("%d ",element.red[i][j]);
            }
            printf("\n");
        }  
}
/// discovers the photo type
/// working as intended
int file_type(FILE *fptr,char s[NMAX])
{
    char determine_type[3];
    fptr=fopen(s,"r");
    if(!fptr)
    {
        fprintf(stderr,"Failed to load %s\n",s);
        return 0;
    }
    ignore_comments(fptr);
    fscanf(fptr,"%s",determine_type);
    if(determine_type[0]!='P') {
        printf("not p\n");
        fclose(fptr);
        return 0;
    }
    if(determine_type[1]>='7' || determine_type[1]<='1' || determine_type[1]=='4' || determine_type[2]!=0) {
        printf("not num\n");
        fclose(fptr);
        return 0;
    }
    switch(determine_type[1])
    {
        case '2':
            fseek(fptr,0,0);
            fclose(fptr);
            return 2;
        case '3':
            fseek(fptr,0,0);
            fclose(fptr);
            return 3;
        case '5':
            fseek(fptr,0,0);
            fclose(fptr);
            return 5;
        case '6':
            fseek(fptr,0,0);
            fclose(fptr);
            return 6;
    }
    return 0;
}
void file_reader_first_version(char s[NMAX],int type,FILE *fptr,struct global_image* image)
{
    switch(type)
    {
        case 2:///pgm ascii
            text_file_reader_pgm_edition(s,fptr,image);
            break;
        case 3:///ppm ascii
            text_file_reader_ppm_edition(s,fptr,image);
            break;
        case 5:///pgm binary
            binary_file_reader_pgm_edition(s,fptr,image);
            break;
        case 6:///ppm binary
            binary_file_reader_ppm_edition(s,fptr,image);
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
    fscanf(fptr,"%d%d",&image->width,&image->height);

    ignore_comments(fptr);
    fscanf(fptr,"%d",&image->maxValue);

    ignore_comments(fptr);
    image->red=alloc_matrix(image->height,image->width);
    
    if(!image->red)
    {
        fprintf(stderr,"Failed to load %s\n",s);
        return;
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
}
/// reads PPM files in ascii format
void text_file_reader_ppm_edition(char s[NMAX],FILE *fptr,struct global_image* image)
{
    fptr=fopen(s,"rt");
    ignore_comments(fptr);
    fscanf(fptr,"%s",image->type);

    ignore_comments(fptr);
    fscanf(fptr,"%d%d",&image->width,&image->height);

    ignore_comments(fptr);
    fscanf(fptr,"%d",&image->maxValue);

    ignore_comments(fptr);
    image->red=alloc_matrix(image->height,image->width);
    image->green=alloc_matrix(image->height,image->width);
    image->blue=alloc_matrix(image->height,image->width);
    if(!image->red || !image->green || !image->blue)
    {
        fprintf(stderr,"Failed to load %s\n",s);
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
        }
    }
    fclose(fptr);
}
/// reads PGM files in binary format
void binary_file_reader_pgm_edition(char s[NMAX],FILE *fptr, struct global_image* image)
{
    fptr=fopen(s,"rb");
    ignore_comments(fptr);
    fscanf(fptr,"%s",image->type);

    ignore_comments(fptr);
    fscanf(fptr,"%d%d",&image->width,&image->height);

    ignore_comments(fptr);
    fscanf(fptr,"%d",&image->maxValue);
    image->red=alloc_matrix(image->height,image->width);
    if(!image->red)
    {
        fprintf(stderr,"Failed to load %s\n",s);
        return ;
    }
    for(int i=0; i<image->height; i++)
    {
        for(int j=0; j<image->width; j++)
        {
            unsigned char c;
            fread(&c,sizeof(unsigned char),1,fptr);
            image->red[i][j]=(int)c;
        }
    }
    fclose(fptr);
}
/// reads PPM files in binary format
void binary_file_reader_ppm_edition(char s[NMAX],FILE *fptr,struct global_image* image)
{
    fptr=fopen(s,"rb");
    ignore_comments(fptr);
    fscanf(fptr,"%s",image->type);

    ignore_comments(fptr);
    fscanf(fptr,"%d%d",&image->width,&image->height);

    ignore_comments(fptr);
    fscanf(fptr,"%d",&image->maxValue);
    ignore_comments(fptr);
    image->red=alloc_matrix(image->height,image->width);
    image->green=alloc_matrix(image->height,image->width);
    image->blue=alloc_matrix(image->height,image->width);
    if(!image->red || !image->green || !image->blue)
    {
        fprintf(stderr,"Failed to load %s\n",s);
        return ;
    }
    for(int i=0; i<image->height; i++)
    {
        for(int j=0; j<image->width; j++)
        {
            unsigned char c;
            fread(&c,sizeof(unsigned char),1,fptr);
            image->red[i][j]=(int)c;

            unsigned char d;
            fread(&d,sizeof(unsigned char),1,fptr);
            image->green[i][j]=(int)d;

            unsigned char e;
            fread(&e,sizeof(unsigned char),1,fptr);
            image->blue[i][j]=(int)e;
        }
    }
    fclose(fptr);

}
void file_printer_for_tests(int *type,struct global_image image)
{   
    char file[NMAX];
    printf("Name output file: ");
    scanf("%s",file);
    printf("\nChoose type:");
    scanf("%d",type);
    FILE *output=fopen(file,"wt");
    switch (*type)
    {
    case 2:
        fprintf(output,"P2\n%d %d\n%d\n",image.width,image.height,image.maxValue);
        for(int i=0; i<image.height; i++)
        {
            for(int j=0; j<image.width; j++)
            {
                fprintf(output,"%d ",image.red[i][j]);
            }
            fprintf(output,"\n");
        }
        break;
    case 3:
        fprintf(output,"P3\n%d %d\n%d\n",image.width,image.height,image.maxValue);
        for(int i=0; i<image.height; i++)
        {
            for(int j=0; j<image.width; j++)
            {
                //image.red[i][j]=255-image.red[i][j];
                //image.green[i][j]=255-image.green[i][j];
                //image.blue[i][j]=255-image.blue[i][j];
                fprintf(output,"%d %d %d ",image.red[i][j],image.green[i][j],image.blue[i][j]);
            }
            fprintf(output,"\n");
        }
        break;
    case 5:
        fprintf(output,"P5\n%d %d\n%d\n",image.width,image.height,image.maxValue);
        for(int i=0; i<image.height; i++)
        {
            for(int j=0; j<image.width; j++)
            {
                unsigned char c=(char)image.red[i][j];
                fwrite(&c,sizeof(unsigned char),1,output);
            }
        }
        break;
    case 6:
        fprintf(output,"P6\n%d %d\n%d\n",image.width,image.height,image.maxValue);
        for(int i=0; i<image.height; i++)
        {
            for(int j=0; j<image.width; j++)
            {
                unsigned char c=(char)image.red[i][j];
                fwrite(&c,sizeof(unsigned char),1,output);

                unsigned char d=(char)image.green[i][j];
                fwrite(&d,sizeof(unsigned char),1,output);
                
                unsigned char e=(char)image.blue[i][j];
                fwrite(&e,sizeof(unsigned char),1,output);
            }
        }
        break;
    default:
        printf("not ppm or pgm\n");
        break;
    }
    fclose(output);
}
int operation_identifier(char s[NMAX],int *x1,int *y1, int *x2,int *y2,int *h1,int *h2,
                        int *angle,char parametre[NMAX],struct global_image image)
{
    char string[NMAX];
    fgets(string,NMAX,stdin);
    string[strlen(string)-1]='\0';
    char *p=strtok(string," ");

    ///load
    if(strcmp(p,"LOAD")==0){
        p=strtok(NULL," ");
        strcpy(s,p);
        return 1;
    }

    /// select
    if(strcmp(p,"SELECT")==0) {
        int contor=0,cpx1,cpx2,cpy1,cpy2;
        while(p)
        {
            if(p[0]>='A' && p[0]<='Z') ; 
            else
            {
            if(contor==0)
            {
                cpx1=atoi(p);
            }
            if(contor==1)
            {
                cpy1=atoi(p);
            }
            if(contor==2)
            {
                cpx2=atoi(p);
            }
            if(contor==3)
            {
               cpy2=atoi(p);
            }
            contor++;
            }
        p = strtok(NULL, " ");
        }
        if(contor == 0) return 3;
        else if(contor ==4) {
            if(max_of_numbers(cpx1,cpx2)>image.width) 
            {
                printf("Invalid coordinates\n");
                return 0;
            }
            if(max_of_numbers(cpy1,cpy2)>image.height) 
            {
                printf("Invalid coordinates\n");
                return 0;
            }
            *x1=cpx1; *x2=cpx2; *y1=cpy1; *y2=cpy2;
            return 2;
        }
        else {
            printf("not enough numbers\n"); return 0;
        }
    }
    ///histogram
    if(strcmp(p,"HISTOGRAM")==0) {
        if(image.type[1]=='3' || image.type[1]=='6') {
            printf("Black and white image needed\n");
            return 0;
        }
        int contor=0;
        while(p)
        {
            if(p[0]>='A' && p[0]<='Z') ; 
            else
            {
            if(contor==0)
            {
                *h1=atoi(p);
            }
            if(contor==1)
            {
                *h2=atoi(p);
            }
            contor++;
            }
        p = strtok(NULL, " ");
        }
        return 4;
    }
    if(strcmp(p,"EXIT")==0)
    {
        return 10;
    }
    return 0;
}

