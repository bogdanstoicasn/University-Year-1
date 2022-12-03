/// Copyright 2022-2023 Stoica Mihai-Bogdan 315CA (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include <stdlib.h>
#include "function.h"
#include <math.h>
struct octave
{
    int line;
    int coloumn;
    int **matrix;
};
/// Wwap 2 elements
void swap(int *a,int *b) {
    int aux=*a;
    *a=*b;
    *b=aux;
}
/// Matrix dinamicaly alocated with defensive programming
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
/// Reading a matrix from stdin
void read_matrix(int line, int coloumn, int **matrix)
{
    int x;
	for (int i = 0; i < line; i++) {
		for (int j = 0; j < coloumn; j++) {
			scanf("%d", &x);
            matrix[i][j]=x;
		}
	}
}
/// Memory free for matrix
void free_matrix(int line,int **matrix) {
    for (int i = 0; i < line; i++) {
		free(matrix[i]);
	}
	free(matrix);
}
int read_function() {
    int number;
    scanf("%d",&number);
    return number;
}
/// Dimensions of matrix to stdout
void print_dimension(int line,int coloumn) {
    printf("%d %d\n",line,coloumn);
}
/// Matrix to stdout
void print_matrix(struct octave element) {
        for(int i=0; i<element.line; i++) {
            for(int j=0; j<element.coloumn; j++) {
                printf("%d ",element.matrix[i][j]);
            }
            printf("\n");
        }
    
}
/// Eliminating a matrix from vector and updating the size of the vector
void moving_matrix(struct octave *element,int *size,int index) {
    int i;
    for(i=index; i<(*size)-1; i++) {
        *(element+i)=*(element+i+1);
    }
    (*size)--;
}
/// As the name suggests: matrix multiplication. The function returns the new matrix
void matrix_multiplication(int line1,int line2,int coloumn2,int **first,int **second,int **third) {
    int i,j,k;
        for(i=0; i<line1; i++) {
            for(j=0; j<coloumn2; j++) {
                third[i][j]=0;
                for(k=0; k<line2; k++) {
                   if(first[i][k]*second[k][j]>=0) third[i][j]+=((first[i][k]*second[k][j])%10007);
                   else third[i][j]=third[i][j]+(first[i][k]*second[k][j])%10007+10007;
                }
            }
        }
        for(i=0; i<line1; i++)
            for(j=0; j<coloumn2; j++)
                third[i][j]=third[i][j]%10007;
}
/// Sum of elements 
int sum_elements_matrix(int line,int coloumn,int **matrix) {
    int s=0;
    for(int i=0; i<line; i++) {
        for(int j=0; j<coloumn; j++) {
           if(matrix[i][j]>=0) s=s+matrix[i][j]%10007;
           else s=s+(matrix[i][j]%10007+10007);
        }
    }
    s=s%10007;
    return s;
}
/// Sorts the matrixes by element sum
void sort_by_element_sum(struct octave *element,int size) {
    struct octave aux;
    for(int i=0; i<size-1; i++) {
        for(int j=i+1; j<size; j++) {    ///Sorry again. Love y'all :P
            if(sum_elements_matrix(element[i].line,element[i].coloumn,element[i].matrix)>sum_elements_matrix(element[j].line,element[j].coloumn,element[j].matrix)) {
               aux=element[i];
               element[i]=element[j];
               element[j]=aux;
            }
        }
    }
}
/// Creates a new matrix which is the transposition(of first matrix) and returns pointer
int **transposition_matrix(struct octave element) {
    int **a=alloc_matrix(element.coloumn,element.line);
    if(!a) {
        printf("alloc failed in transposition\n");
        return NULL;
    }
    for(int i=0; i<element.line; i++) {
        for(int j=0; j<element.coloumn; j++) {
            a[j][i]=element.matrix[i][j];
        }
    }
    return a;
}
void complement_function_C() {
    int nrline,nrcoloumn,*line,*coloumn,i;
    scanf("%d",&nrline);
    line=(int*)malloc(nrline*sizeof(int));
    if(!line) exit(0);
    for(i=0;i<nrline;i++) {
        scanf("%d",&line[i]);
    }
    scanf("%d",&nrcoloumn);
    coloumn=(int*)malloc(nrcoloumn*sizeof(int));
    if(!coloumn) exit(0);
    for(i=0;i<nrcoloumn;i++) {
        scanf("%d",&coloumn[i]);
    }
    free(line);
    free(coloumn);
    printf("No matrix with the given index\n");
}
/// Gets from input the index of a matrix, n lines, m coloumns
/// The function it's creating a new resized matrix and returns pointer to it
int **resize_matrix(int *line1,int *coloumn1,int **matrix) {
    int nrline,nrcoloumn,*line,*coloumn,i,j;
    scanf("%d",&nrline);
    line=(int*)malloc(nrline*sizeof(int));
    if(!line) {
        fprintf(stderr, "malloc() failed\n");
		return NULL;
    }
    ///lines we want to resize
    for(i=0;i<nrline;i++) {
        scanf("%d",&line[i]);
    }
    scanf("%d",&nrcoloumn);
    coloumn=(int*)malloc(nrcoloumn*sizeof(int));
    if(!coloumn) {
        fprintf(stderr, "malloc() failed\n");
		return NULL;
    }
    ///coloumns we want to resize
    for(i=0;i<nrcoloumn;i++) {
        scanf("%d",&coloumn[i]);
    }
    ///alloc of new matrix
    int **a=alloc_matrix(nrline,nrcoloumn);
    for(i=0; i<nrline; i++) {
        for(j=0; j<nrcoloumn; j++) {
            a[i][j]=matrix[line[i]][coloumn[j]];
        }
    }
    free(line);
    free(coloumn);
    (*line1)=nrline;
    (*coloumn1)=nrcoloumn;
    return a;
}
///Matrix multiplication but both are square and with equal lines
void square_matrix_multiplication(int line,int **first,int **second) {
    int **third=alloc_matrix(line,line);
    if(!third) {
        fprintf(stderr, "malloc() failed\n");
    }
    for(int i=0; i<line; i++) {
        for(int j=0; j<line; j++) {
            third[i][j]=0;
            for(int k=0; k<line; k++) {
                if(first[i][k]*second[k][j]>=0) third[i][j]+=((first[i][k]*second[k][j])%10007);
                else third[i][j]=third[i][j]+(first[i][k]*second[k][j])%10007+10007;
            }
        }
    }
    for(int i=0; i<line; i++) {
        for(int j=0; j<line; j++) {
            third[i][j]=third[i][j]%10007;
            first[i][j]=third[i][j];
        }
    }
    free_matrix(line,third);
}
/// Matrix to pow power in O(logn)
/// We copy the matrix in an identity matrix
/// If pow odd we muliply matrix by identity else we multiply identity to identiy
void matrix_pow(int line,int coloumn,int pow,int **matrix) {
    
    if(line!=coloumn) printf("Cannot perform matrix multiplication\n");
    else {
        int **identity=alloc_matrix(line,line);
        for(int i=0; i<line; i++) {
            for(int j=0; j<line; j++) {
                identity[i][j]=matrix[i][j];
        }
    }
    pow--;
    while(pow>0) {
        if(pow&1) {
            square_matrix_multiplication(line,matrix,identity);
        }
        square_matrix_multiplication(line,identity,identity);
        pow=pow>>1;
    }
     free_matrix(line,identity);
    }
        
}


/// Strassen algorithm very badly implemented I have to say 
/// It's done step by step
/// Recursive
/// It's bellow
/// It took me 2 days :((
int nextpower2of2(int k) {
    return pow(2,ceil(log2(k)));
}
/// 1 matrix + 1 matrix = matrix
void add(int **first,int **second,int **third,int line) {
    for(int i=0; i<line; i++) {
        for(int j=0; j<line; j++) {
             third[i][j]=first[i][j]+second[i][j];   
        }
    }
}
/// 1 matrix - 1 matrix = matrix
void sub(int **first,int **second,int **third,int line) {
    for(int i=0; i<line; i++) {
        for(int j=0; j<line; j++) {
            third[i][j]=first[i][j]-second[i][j];    
        }
    }
}
void Strassen_algorithm_helper(int **a,int **b,int **c,int size) {
    /// This case was the easiest
    if(size==1) {
        c[0][0]=a[0][0]*b[0][0];
    } else {
        int new_size=size/2;
        int **a11=alloc_matrix(new_size,new_size);
        int **a12=alloc_matrix(new_size,new_size);
        int **a21=alloc_matrix(new_size,new_size);
        int **a22=alloc_matrix(new_size,new_size);
        int **b11=alloc_matrix(new_size,new_size);
        int **b12=alloc_matrix(new_size,new_size);
        int **b21=alloc_matrix(new_size,new_size);
        int **b22=alloc_matrix(new_size,new_size);
        int **c11=alloc_matrix(new_size,new_size);
        int **c12=alloc_matrix(new_size,new_size);
        int **c21=alloc_matrix(new_size,new_size);
        int **c22=alloc_matrix(new_size,new_size);
        int **m1=alloc_matrix(new_size,new_size);
        int **m2=alloc_matrix(new_size,new_size);
        int **m3=alloc_matrix(new_size,new_size);
        int **m4=alloc_matrix(new_size,new_size);
        int **m5=alloc_matrix(new_size,new_size);
        int **m6=alloc_matrix(new_size,new_size);
        int **m7=alloc_matrix(new_size,new_size);
        int **aResult=alloc_matrix(new_size,new_size);
        int **bResult=alloc_matrix(new_size,new_size);
        /// Sorry again :)))
        if(!a11 || !a12 || !a21 || !a22 || !b11 || !b12 || !b21 || !b22 || !c11 || !c12 || !c21 || !c22 || !m1 || !m2 || !m3 || !m4 || !m5 || !m6 || !m7 || !aResult || !bResult) fprintf(stderr,"alloc matrix failed\n");
        int i,j;
        for(i=0; i<new_size; i++) {
            for(j=0; j<new_size; j++) {
                a11[i][j] = a[i][j];
                a12[i][j] = a[i][j + new_size];
                a21[i][j] = a[i + new_size][j];
                a22[i][j] = a[i + new_size][j + new_size];

                b11[i][j] = b[i][j];
                b12[i][j] = b[i][j + new_size];
                b21[i][j] = b[i + new_size][j];
                b22[i][j] = b[i + new_size][j + new_size];
            }
        }
        /// Calculating m1 to m7:
        add(a11, a22, aResult, new_size); /// a11 + a22
        add(b11, b22, bResult, new_size); /// b11 + b22
        Strassen_algorithm_helper(aResult, bResult, m1, new_size);
        /// m1 = (a11+a22) * (b11+b22)

        add(a21, a22, aResult, new_size); /// a21 + a22
        Strassen_algorithm_helper(aResult, b11, m2, new_size);
        /// m2 = (a21+a22) * (b11)

        sub(b12, b22, bResult, new_size); /// b12 - b22
        Strassen_algorithm_helper(a11, bResult, m3, new_size);
        /// m3 = (a11) * (b12 - b22)

        sub(b21, b11, bResult, new_size); /// b21 - b11
        Strassen_algorithm_helper(a22, bResult, m4, new_size);
        /// m4 = (a22) * (b21 - b11)

        add(a11, a12, aResult, new_size); /// a11 + a12
        Strassen_algorithm_helper(aResult, b22, m5, new_size);
        /// m5 = (a11+a12) * (b22)

        sub(a21, a11, aResult, new_size); /// a21 - a11
        add(b11, b12, bResult, new_size); /// b11 + b12
        Strassen_algorithm_helper(aResult, bResult, m6, new_size);
        /// m6 = (a21-a11) * (b11+b12)

        sub(a12, a22, aResult, new_size); /// a12 - a22
        add(b21, b22, bResult, new_size); /// b21 + b22
        Strassen_algorithm_helper(aResult, bResult, m7, new_size);
        /// m7 = (a12-a22) * (b21+b22)

        /// Calculating c11,c12,c21,c22:

        add(m3, m5, c12, new_size); /// c12 = m3 + m5
        add(m2, m4, c21, new_size); /// c21 = m2 + m4

        add(m1, m4, aResult, new_size); /// m1 + m4
        add(aResult, m7, bResult, new_size); /// m1 + m4 + m7
        sub(bResult, m5, c11, new_size); /// c11 = m1 + m4 - m5 + m7

        add(m1, m3, aResult, new_size); /// m1 + m3
        add(aResult, m6, bResult, new_size);  /// m1 + m3 + m6
        sub(bResult, m2, c22, new_size); /// c22 = m1 + m3 - m2 + m6

        /// Grouping in a single matrix:
        for (i = 0; i < new_size; i++) {
            for (j = 0; j < new_size; j++) {
                c[i][j] = c11[i][j];
                c[i][j + new_size] = c12[i][j];
                c[i + new_size][j] = c21[i][j];
                c[i + new_size][j + new_size] = c22[i][j];
            }
        }
        /// Free memory as I should:))
        free_matrix(new_size,a11);
        free_matrix(new_size,a12);
        free_matrix(new_size,a21);
        free_matrix(new_size,a22);
        free_matrix(new_size,b11);
        free_matrix(new_size,b12);
        free_matrix(new_size,b21);
        free_matrix(new_size,b22);
        free_matrix(new_size,c11);
        free_matrix(new_size,c12);
        free_matrix(new_size,c21);
        free_matrix(new_size,c22);
        free_matrix(new_size,aResult);
        free_matrix(new_size,bResult);
        free_matrix(new_size,m1);
        free_matrix(new_size,m2);
        free_matrix(new_size,m3);
        free_matrix(new_size,m4);
        free_matrix(new_size,m5);
        free_matrix(new_size,m6);
        free_matrix(new_size,m7);
    }
}
void Strassen_algorithm(int **a,int **b,int **c,int line) {
    int s=nextpower2of2(line);
    int **copya=alloc_matrix(line,line);
    if(!copya) fprintf(stderr,"alloc matrix failed\n");
    for(int i=0; i<line;i++) {
        for(int j=0; j<line; j++) {
            copya[i][j]=a[i][j];
        }
    }
    int **copyb=alloc_matrix(line,line);
    if(!copyb) fprintf(stderr,"alloc matrix failed\n");
    for(int i=0; i<line;i++) {
        for(int j=0; j<line; j++) {
            copyb[i][j]=b[i][j];
        }
    }
    int **copyc=alloc_matrix(line,line);
    if(!copyc) fprintf(stderr,"alloc matrix failed\n");
    Strassen_algorithm_helper(copya,copyb,copyc,s);
    for(int i=0; i<line; i++) {
        for(int j=0; j<line; j++) {
            c[i][j]=copyc[i][j];
        }
    }
    /// Free memory again. This gettin annoying
    free_matrix(line,copya);
    free_matrix(line,copyb);
    free_matrix(line,copyc);
}
void transform(int line,int **c) {
    for(int i=0; i<line; i++) {
        for(int j=0; j<line; j++) {
            if(c[i][j]>=0) c[i][j]=c[i][j]%10007;
            else c[i][j]=c[i][j]%10007+10007;
        }
    }
}