/// Copyright 2022-2023 Stoica Mihai-Bogdan 315CA (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include <stdlib.h>
#include "function.h"
struct octave
{
    int line,coloumn;
    int **matrix;
};
int main()
{
    struct octave *vector=NULL;
    int size=0,index,index1;
    char character;
    character=getchar();
    while(1) {
        switch(character) {
            case 'L': {   /// we allocating/reallocating the vector of struct
                vector=realloc(vector,(++size)*sizeof(struct octave));
                if(!vector) {
                    fprintf(stderr,"not alloc");
                    for(int i=0; i<size; i++) {
                        free_matrix(vector[i].line, vector[i].matrix);
                    }
                    free(vector);
                    return -1;
                }
                vector[size-1].line=read_function();
                vector[size-1].coloumn=read_function();
                (vector+size-1)->matrix=alloc_matrix((vector+size-1)->line,(vector+size-1)->coloumn);
                read_matrix(vector[size-1].line,vector[size-1].coloumn,(vector+size-1)->matrix);
                break;
            } case 'D': {   /// printing dimensions
                index=read_function();
                (index>=size || index<0) ? printf("No matrix with the given index\n") : print_dimension(vector[index].line,vector[index].coloumn);
                break;
            } case 'P': {   /// printing matrix
                index=read_function();
                (index>=size || index<0) ? printf("No matrix with the given index\n") : print_matrix(vector[index]);
                break;
            } case 'Q': {   /// freeing the memory and ending the program
                for(int i=0; i<size; i++) {
                    free_matrix(vector[i].line, vector[i].matrix);
                }
                free(vector);
                return 0;
            } case 'F': {   /// eliminating one matrix from vector: elim one matrix then moving elements from vector one step back
                index=read_function();
                if(index>=size || index<0) printf("No matrix with the given index\n");
                else {
                    free_matrix(vector[index].line,vector[index].matrix);
                    moving_matrix(vector,&size,index);
                    vector=(struct octave*)realloc(vector,size*sizeof(struct octave));
                }
                break;
            } case 'M': {   /// allocating new memory for matrix mulitplication + doing the multiplication
                index=read_function();index1=read_function();
                if(index1>=size || index>=size || index1<0 || index<0) printf("No matrix with the given index\n");
                else {
                    if(vector[index].coloumn!=vector[index1].line) printf("Cannot perform matrix multiplication\n");
                    else {
                    vector=(struct octave*)realloc(vector,(++size)*sizeof(struct octave));
                    if(!vector) {
                        fprintf(stderr,"not alloc");
                        for(int i=0; i<size; i++) {
                            free_matrix(vector[i].line, vector[i].matrix);
                        }
                        free(vector);
                        return -1;
                    }
                    vector[size-1].line=vector[index].line;
                    vector[size-1].coloumn=vector[index1].coloumn;
                    (vector+size-1)->matrix=alloc_matrix(vector[size-1].line,vector[size-1].coloumn);
                    /// sorry for the function below(it's on steroids):))
                    matrix_multiplication(vector[index].line,vector[index1].line,vector[index1].coloumn,vector[index].matrix,vector[index1].matrix,vector[size-1].matrix);
                    }
                    break;
                }
                break;
            } case 'O': {   /// sorting elements in vector by sum of elements contained by matrixes
                sort_by_element_sum(vector,size);
                break;
            } case 'T': {   /// we get the address of old matrix in pointer + we get transposition + we put transposition in it's place + free old matrix memory(basically free pointer) 
                index=read_function();
                if(index>=size || index<0) printf("No matrix with the given index\n");
                else {
                    int **pointer=vector[index].matrix;
                    int linie=vector[index].line;
                    vector[index].matrix=transposition_matrix(vector[index]);
                    swap(&vector[index].line,&vector[index].coloumn);
                    free_matrix(linie,pointer);
                }
                break;
            } case 'C': {   /// resizing matrix + deleting old matrix + we put new resized matrix in place 
                index=read_function();
                /// We use complement_function_C when index does not exist
                if(index>=size || index<0) complement_function_C();
                else {
                    int **pointer=vector[index].matrix,l=vector[index].line;
                    vector[index].matrix=resize_matrix(&vector[index].line,&vector[index].coloumn,vector[index].matrix);
                    free_matrix(l,pointer);

                }
                break;
            } case 'R': {   /// matrix to "pow" power 
                index=read_function();
                int pow=read_function();
                if(index>=size || index<0) printf("No matrix with the given index\n");
                else {
                    if (pow<=0) printf("Power should be positive\n");
                    else {
                        matrix_pow(vector[index].line,vector[index].coloumn,pow,vector[index].matrix);
                    }
                }
                break;
            } case 'S': {   /// alloc memory for strassen + strassen
                index=read_function(); index1=read_function();
                if(index1>=size || index>=size || index1<0 || index<0) printf("No matrix with the given index\n");
                else {
                    if(vector[index].line!=vector[index1].line) printf("Cannot perform matrix multiplication\n");
                    else {
                        vector=(struct octave*)realloc(vector,(++size)*sizeof(struct octave));
                        if(!vector) {
                            fprintf(stderr,"not alloc");
                            for(int i=0; i<size; i++) {
                                free_matrix(vector[i].line, vector[i].matrix);
                            }
                            free(vector);
                            return -1;
                        }
                        vector[size-1].line=vector[index].line;
                        vector[size-1].coloumn=vector[index1].coloumn;
                        (vector+size-1)->matrix=alloc_matrix(vector[size-1].line,vector[size-1].coloumn);
                        Strassen_algorithm(vector[index].matrix,vector[index1].matrix,vector[size-1].matrix,vector[index].line);
                        transform(vector[index].line,vector[size-1].matrix);
                    }
                }
                break;
            } default :
                printf("Unrecognized command\n");
                break;
        }
        scanf(" %c",&character);
    }
    return 0;
}
