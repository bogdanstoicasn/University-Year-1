#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "function.h"
#include <string.h>
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
    if(type==0) return ;
    
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
        fclose(fptr);
        return 0;
    }
    if(determine_type[1]>='7' || determine_type[1]<='1' || determine_type[1]=='4' || determine_type[2]!=0) {
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
    fclose(fptr);
}

void file_reader_first_version(int *count,char s[NMAX],int type,FILE *fptr,struct global_image* image)
{
    if(*count!=0) {
        int n=image->type[1]-'0';
        free_global_matrix(n,image);
    }
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
    (*count)=1;
}

/// reads PGM files in ascii format 
void text_file_reader_pgm_edition(char s[NMAX],FILE *fptr,struct global_image* image)
{
    fptr=fopen(s,"rt");
    ignore_comments(fptr);
    fscanf(fptr,"%s",image->type);

    ignore_comments(fptr);
    fscanf(fptr,"%d%d",&image->width,&image->height);
    image->x_axis=image->width;
    image->y_axis=image->height;

    ignore_comments(fptr);
    fscanf(fptr,"%d",&image->maxValue);

    ignore_comments(fptr);
    image->red=alloc_matrix(image->height,image->width);
    image->red_crop=alloc_matrix(image->height,image->width);
    
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
            image->red_crop[i][j]=x;
        }
    }
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
    fscanf(fptr,"%d%d",&image->width,&image->height);
    image->x_axis=image->width;
    image->y_axis=image->height;

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

            image->red_crop[i][j]=x;
            image->green_crop[i][j]=y;
            image->blue_crop[i][j]=z;
        }
    }
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
    fscanf(fptr,"%d%d",&image->width,&image->height);
    image->x_axis=image->width;
    image->y_axis=image->height;

    ignore_comments(fptr);
    fscanf(fptr,"%d",&image->maxValue);
    
    image->red=alloc_matrix(image->height,image->width);
    image->red_crop=alloc_matrix(image->height,image->width);

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
            image->red_crop[i][j]=(int)c;
        }
    }
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
    fscanf(fptr,"%d%d",&image->width,&image->height);
    image->x_axis=image->width;
    image->y_axis=image->height;

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
    printf("Loaded %s\n",s);
    fclose(fptr);

}

void file_printer_for_tests(int *type,char file[NMAX],struct global_image image)
{   
    FILE *output=fopen(file,"w");
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
            if(i<image.height-1) fprintf(output,"\n");
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
            if(i<image.height-1) fprintf(output,"\n");
        }

        printf("Saved %s\n",file);
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

        printf("Saved %s\n",file);
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

        printf("Saved %s\n",file);
        break;
    default:
        printf("Failed to load %s\n",file);
        break;
    }
    fclose(output);
}

int operation_identifier(int count,char s[NMAX],int *x1,int *y1, int *x2,int *y2,int *h1,int *h2,
                        int *angle,char parametre[NMAX],char file[NMAX],int *type,struct global_image image)
{
    char string[NMAX],copy_string[NMAX];
    fgets(string,NMAX,stdin);
    for(size_t i=0; i<strlen(string); i++)
    {
        if(string[i]=='\n') string[i]='\0';
    }
    
    strcpy(copy_string,string);
    
    char *p=strtok(string," ");

    /*if(count==0 && strcmp(p,"LOAD")!=0) {
        if(strcmp(p,"EXIT")==0)  return 10;
        printf("No image loaded\n");
        return 0;
    }*/
    ///load
    if(strcmp(p,"LOAD")==0){
        return load_function_identifier(copy_string,s);
    }

    /// select + we use select_function_identifier
    if(strcmp(p,"SELECT")==0) {
        return select_function_identifier(count,copy_string,image,x1,y1,x2,y2);
    }

    ///histogram + we use histogram_function_identifier
    if(strcmp(p,"HISTOGRAM")==0) {
        return histogram_function_identifier(count,copy_string,image,h1,h2);
    }

    if(strcmp(p,"ROTATE")==0) {
        return rotate_function_identifier(count,copy_string,image,angle);
    }

    if(strcmp(p,"EQUALIZE")==0) {
        return equalize_function_identifier(count,image);
    }

    /// crop 
    if(strcmp(p,"CROP")==0) {
        if(count==0) return 11;
        return 7;
    }

    if(strcmp(p,"APPLY")==0) {
        return apply_function_identifier(count,copy_string,parametre,image);
    }

    /// save + we use save_function_identifier
    if(strcmp(p,"SAVE")==0) {
        if(count==0) return 11;
        return save_function_identifier(copy_string,file,type,image);
    }

    /// exit
    if(strcmp(p,"EXIT")==0)
    {
        if(count==0) return 11;
        return 10;
    }
    if(strcmp(p,"EXIT")==0)
    {
        if(count==0) return 11;
        return 10;
    }


    printf("Invalid command\n");
    return 0;
}

int load_function_identifier(char string[NMAX],char s[NMAX])
{
    char *p=strtok(string," ");
    int ok=0;
    while(p)
    {
        ok++;
        if(ok==2) strcpy(s,p);
        p=strtok(NULL," ");
    }
    if(ok==1 || ok>=3) {
        printf("Invalid command\n");
        return 0;
    }
    return 1;
}

int select_function_identifier(int count,char string[NMAX],struct global_image image,int *x1,int *y1,int *x2,int *y2)
{
    char *p=strtok(string," ");
    int contor=0,cpx1=-1,cpx2=-1,cpy1=-1,cpy2=-1,ok=0;
    while(p)
    {
        if(p[0]>='A' && p[0]<='Z') {
            if(strcmp(p,"ALL")==0) ok=1;
            else ok--;
        }
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
        p = strtok(NULL," ");
    }
    if(ok==1 && contor==0) {
        if(count==0) {
            return 11;
        }
        return 3;
    }
    
    if(contor ==4) {
        if(count==0) return 11;

        if(max_of_numbers(cpx1,cpx2)>image.width) {
            printf("Invalid set of coordinates\n");
            return 0;
        }
        if(max_of_numbers(cpy1,cpy2)>image.height) {
            printf("Invalid set of coordinates\n");
            return 0;
        }
        if(cpx1==cpx2 || cpy1==cpy2 || cpx1<0 || cpx2<0 || cpy1<0 || cpy2<0) {
            printf("Invalid set of coordinates\n");
            return 0;
        }
        *x1=cpx1; *x2=cpx2; *y1=cpy1; *y2=cpy2;
        
        swap_function(x1,x2);
        swap_function(y1,y2);
        
        return 2;
    }
    if(contor!=3 || ok!=0) {
        printf("Invalid command\n");
        return 0;
    }
    return 0;
}

int histogram_function_identifier(int count,char string[NMAX],struct global_image image,int *h1,int *h2)
{

    char *p=strtok(string," ");
    int contor=0,ch1=-1,ch2=-1;
    while(p)
    {
        if(p[0]>='A' && p[0]<='Z') ; 
        else
        {
        if(contor==0)
        {
            ch1=atoi(p);
        }
        if(contor==1)
        {
            ch2=atoi(p);
        }
        contor++;
        }
    p = strtok(NULL," ");
    }
    
    if(ch1==-1 || ch2==-1) {
        printf("Invalid command\n");
        return 0;
    }

    if(count == 0) 
        return 11;

    if(image.type[1]=='3' || image.type[1]=='6') {
        printf("Black and white image needed\n");
        return 0;
    }
    if(ch2<2 || ch2>256 || (ch2&(ch2-1))!=0) {
        printf("Invalid set of parameters\n");
        return 0;
    }
    *h1=ch1;
    *h2=ch2;
    return 4;
}

int save_function_identifier(char string[NMAX],char file[NMAX],int *type,struct global_image image)
{
    int contor=0;
    for(size_t i=0; i<strlen(string); i++)
    {
        if(string[i]==' ') contor++;
    }
    
    char *p=strtok(string," ");
    p=strtok(NULL," ");
    strcpy(file,p);

    *type=image.type[1]-'0';
    if(contor==2) {
        if(*type==5) {
            *type=2;
            return 9;
        }
        if(*type==6) {
            *type=3;
            return 9;
        }
        return 9;
    }
    else {
        if(*type==2) {
            *type=5;
            return 9;
        }
        if(*type==3) {
            *type=6;
            return 9;
        }
        return 9;
    }
}

int rotate_function_identifier(int count,char string[NMAX],struct global_image image,int *angle)
{
    char *p=strtok(string," ");
    p=strtok(NULL," ");
    int copy_angle=-1;
    while(p)
    {
        copy_angle=atoi(p);
        p=strtok(NULL," ");
    }
    if(copy_angle==-1) {
        printf("Invalid command\n");
        return 0;
    }

    if(count==0) 
        return 11;

    if(image.x_axis!=image.y_axis) {
        printf("The selection must be square\n");
        return 0;
    }

    *angle=copy_angle;
    return 6;
}
// de vazut daca se face select dupa select sau se ia de pe matricea principala
void select_function_integers(struct global_image *image,int x1,int x2,int y1,int y2)
{
    if(image->type[1]=='2' || image->type[1]=='5')
    {
       free_matrix(image->y_axis,image->red_crop);
        
        image->red_crop=alloc_matrix(y2-y1,x2-x1);
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

        image->x_axis=x2-x1;
        image->y_axis=y2-y1;

        printf("Selected %d %d %d %d\n",x1,y1,x2,y2);
    }
    if(image->type[1]=='3' || image->type[1]=='6')
    {
        free_matrix(image->y_axis,image->red_crop);
        free_matrix(image->y_axis,image->green_crop);
        free_matrix(image->y_axis,image->blue_crop);
        
        image->red_crop=alloc_matrix(y2-y1,x2-x1);
        image->green_crop=alloc_matrix(y2-y1,x2-x1);
        image->blue_crop=alloc_matrix(y2-y1,x2-x1);
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
        image->x_axis=x2-x1;
        image->y_axis=y2-y1;
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

        for(int i=0; i<image->height; i++)
        {
            for(int j=0; j<image->width; j++)
            {
                image->red_crop[i][j]=image->red[i][j];
                image->green_crop[i][j]=image->green[i][j];
                image->blue_crop[i][j]=image->blue[i][j];
            }
        }

        image->x_axis=image->width;
        image->y_axis=image->height;

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

        for(int i=0; i<image->y_axis; i++)
        {
            for(int j=0; j<image->x_axis; j++)
            {
                image->red[i][j]=image->red_crop[i][j];
            }
        }

        image->height=image->y_axis;
        image->width=image->x_axis;

        printf("Image cropped\n");
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

        for(int i=0; i<image->y_axis; i++)
        {
            for(int j=0; j<image->x_axis; j++)
            {
                image->red[i][j]=image->red_crop[i][j];
                image->green[i][j]=image->green_crop[i][j];
                image->blue[i][j]=image->blue_crop[i][j];
            }
        }

        image->width=image->x_axis;
        image->height=image->y_axis;

        printf("Image cropped\n");
    }
}

void rotate_function_helper(struct global_image *image,int angle)
{
    //if(image->x_axis!=image->y_axis) {printf("The selection must be square\n");return ;}
    switch(angle)
    {
        case 90:
            clockwise_rotation_90(image);
            break;
        case -90:
            counter_clockwise_rotation_90(image);
            break;
        case 180:
            clockwise_rotation_90(image);
            clockwise_rotation_90(image);
            break;
        case -180:
            counter_clockwise_rotation_90(image);
            counter_clockwise_rotation_90(image);
            break;
        case 270:
            counter_clockwise_rotation_90(image);
            break;
        case -270:
            clockwise_rotation_90(image);
        case 360:
            break;
        case -360:
            break;
        default:
            printf("Unsupported rotation angle\n");
            break;
    }
}

void counter_clockwise_rotation_90(struct global_image *image)
{
    if(image->type[1]=='2' || image->type[1]=='5') {
        // Consider all squares one by one
        for (int x = 0; x < image->y_axis / 2; x++) {
            // Consider elements in group
            // of 4 in current square
            for (int y = x; y < image->x_axis - x - 1; y++) {
                // Store current cell in
                // temp variable
                int temp = image->red_crop[x][y];
    
                // Move values from right to top
                image->red_crop[x][y] =image->red_crop[y][image->y_axis - 1 - x];
    
                // Move values from bottom to right
                image->red_crop[y][image->x_axis - 1 - x] = image->red_crop[image->x_axis - 1 - x][image->x_axis - 1 - y];
    
                // Move values from left to bottom
                image->red_crop[image->x_axis - 1 - x][image->x_axis - 1 - y] = image->red_crop[image->x_axis - 1 - y][x];
    
                // Assign temp to left
                image->red_crop[image->x_axis - 1 - y][x] = temp;
            }
        }
    }

    if(image->type[1]=='3' || image->type[1]=='6') {
        // Consider all squares one by one
        for (int x = 0; x < image->y_axis / 2; x++) {
            // Consider elements in group
            // of 4 in current square
            for (int y = x; y < image->x_axis - x - 1; y++) {
                // Store current cell in
                // temp variable
                int temp = image->red_crop[x][y];
    
                // Move values from right to top
                image->red_crop[x][y] =image->red_crop[y][image->y_axis - 1 - x];
    
                // Move values from bottom to right
                image->red_crop[y][image->x_axis - 1 - x] = image->red_crop[image->x_axis - 1 - x][image->x_axis - 1 - y];
    
                // Move values from left to bottom
                image->red_crop[image->x_axis - 1 - x][image->x_axis - 1 - y] = image->red_crop[image->x_axis - 1 - y][x];
    
                // Assign temp to left
                image->red_crop[image->x_axis - 1 - y][x] = temp;

                // repeat for green
                temp = image->green_crop[x][y];

                image->green_crop[x][y] =image->green_crop[y][image->y_axis - 1 - x];
    
                image->green_crop[y][image->x_axis - 1 - x] = image->green_crop[image->x_axis - 1 - x][image->x_axis - 1 - y];
    
                image->green_crop[image->x_axis - 1 - x][image->x_axis - 1 - y] = image->green_crop[image->x_axis - 1 - y][x];
    
                image->green_crop[image->x_axis - 1 - y][x] = temp;

                // repeat for blue
                temp = image->blue_crop[x][y];

                image->blue_crop[x][y] =image->blue_crop[y][image->y_axis - 1 - x];
    
                image->blue_crop[y][image->x_axis - 1 - x] = image->blue_crop[image->x_axis - 1 - x][image->x_axis - 1 - y];
    
                image->blue_crop[image->x_axis - 1 - x][image->x_axis - 1 - y] = image->blue_crop[image->x_axis - 1 - y][x];

                image->blue_crop[image->x_axis - 1 - y][x] = temp;                
            }
        }
    }
}

void clockwise_rotation_90(struct global_image *image)
{
 
    if(image->type[1]=='2' || image->type[1]=='5') {
        for (int i = 0; i < image->x_axis / 2; i++) {
            for (int j = i; j < image->x_axis - i - 1; j++) {
            
                // Swap elements of each cycle
                // in clockwise direction
                int temp = image->red_crop[i][j];
                image->red_crop[i][j] = image->red_crop[image->x_axis - 1 - j][i];
                image->red_crop[image->x_axis - 1 - j][i] = image->red_crop[image->x_axis - 1 - i][image->x_axis - 1 - j];
                image->red_crop[image->x_axis - 1 - i][image->x_axis - 1 - j] = image->red_crop[j][image->x_axis - 1 - i];
                image->red_crop[j][image->x_axis - 1 - i] = temp;
            }
        }
    }

    if(image->type[1]=='3' || image->type[1]=='6') {
        for (int i = 0; i < image->x_axis / 2; i++) {
            for (int j = i; j < image->x_axis - i - 1; j++) {
            
                // Swap elements of each cycle
                // in clockwise direction
                int temp = image->red_crop[i][j];
                image->red_crop[i][j] = image->red_crop[image->x_axis - 1 - j][i];
                image->red_crop[image->x_axis - 1 - j][i] = image->red_crop[image->x_axis - 1 - i][image->x_axis - 1 - j];
                image->red_crop[image->x_axis - 1 - i][image->x_axis - 1 - j] = image->red_crop[j][image->x_axis - 1 - i];
                image->red_crop[j][image->x_axis - 1 - i] = temp;

                temp = image->green_crop[i][j];
                image->green_crop[i][j] = image->green_crop[image->x_axis - 1 - j][i];
                image->green_crop[image->x_axis - 1 - j][i] = image->green_crop[image->x_axis - 1 - i][image->x_axis - 1 - j];
                image->green_crop[image->x_axis - 1 - i][image->x_axis - 1 - j] = image->green_crop[j][image->x_axis - 1 - i];
                image->green_crop[j][image->x_axis - 1 - i] = temp;

                temp = image->blue_crop[i][j];
                image->blue_crop[i][j] = image->blue_crop[image->x_axis - 1 - j][i];
                image->blue_crop[image->x_axis - 1 - j][i] = image->blue_crop[image->x_axis - 1 - i][image->x_axis - 1 - j];
                image->blue_crop[image->x_axis - 1 - i][image->x_axis - 1 - j] = image->blue_crop[j][image->x_axis - 1 - i];
                image->blue_crop[j][image->x_axis - 1 - i] = temp;
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
        //free_global_matrix(2,&image);
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
        double length_of_star=(histogram_map[i]*(1.0)/max_frequency)*h1_star;
        printf("%d\t|\t",(int)length_of_star);
        for(j=0; j<(int)length_of_star; j++)
            printf("*");
        printf("\n");
    }
    free(histogram_map);
}

int equalize_function_identifier(int count,struct global_image image)
{
    if(count == 0) {
        return 11;
    }
    if(image.type[1]=='3' || image.type[1]=='6') {
        printf("Black and white image needed\n");
        return 0;
    }
    return 5;
}

void equalize_function(struct global_image *image)
{
    int *frequency=calloc(257,sizeof(int));
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
            final_result=sum*(1.0)*255/area;
            image->red[i][j]= round(final_result);
            if(image->red[i][j]>255) image->red[i][j]=255;
            if(image->red[i][j]<0) image->red[i][j]=0;
        }
    }
    free(frequency);
    printf("Equalize done\n");
}

int apply_function_identifier(int count,char string[NMAX],char parameter[NMAX],struct global_image image)
{
    if(count==0) {
        return 11;
    }
    char *p=strtok(string," ");
    int ok=0;
    while(p)
    {
        ok++;
        if(ok==2) strcpy(parameter,p);
        p=strtok(NULL," ");
    }

    if(ok==1 || ok>=3) {
        printf("Invalid command\n");
        return 0;
    }
    if(strcmp(parameter,"EDGE")==0 || strcmp(parameter,"SHARPEN")==0 || strcmp(parameter,"BLUR")==0 || strcmp(parameter,"GAUSSIAN_BLUR")==0) {
       
        if(image.type[1]=='2' || image.type[1]=='5') {
            printf("Easy, Charlie Chaplin\n");
            return 0;
        }

        return 8;
    }
    else {
        printf("APPLY parameter invalid\n");
        return 0;
    }


    return 8;
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
			for(j=0; j<=i; j++)
				kernel[i][j]=kernel[j][i]=-1;
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
			for(j=0; j<=i; j++)
				kernel[i][j]=kernel[j][i]=1;
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
            for(int m=-1; m<=1; m++) 
            {
                for(int n=-1; n<=1; n++)
                {
                    if(i+m>=0 && i+m<image->height && j+n>=0 && j+n<image->width) 
                        {
                            sum_r+=image->red[i+m][j+n]*kernel[m+1][n+1];
                            sum_g+=image->green[i+m][j+n]*kernel[m+1][n+1];
                            sum_b+=image->blue[i+m][j+n]*kernel[m+1][n+1];
                        }
                }
            }
            sum_r/=div;
            sum_g/=div;
            sum_b/=div;

            checker_0_and_255_case(&sum_b);
            checker_0_and_255_case(&sum_g);
            checker_0_and_255_case(&sum_r);

            image->red_crop[i-y1][j-x1]=(int)round(sum_r);
            image->blue_crop[i-y1][j-x1]=(int)round(sum_b);
            image->green_crop[i-y1][j-x1]=(int)round(sum_g);   
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
    printf("Apply %s done\n",parameter);
}
