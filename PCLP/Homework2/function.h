/// Copyright 2022-2023 Stoica Mihai-Bogdan 315CA (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
struct octave;
void swap(int *a,int *b);
int **alloc_matrix(int line, int coloumn);
void read_matrix(int line, int coloumn, int **a);
void free_matrix(int line, int **a);
int read_function();
void print_dimension(int line,int coloumn);
void print_matrix(struct octave element);
void moving_matrix(struct octave *element,int *size,int index);
void matrix_multiplication(int line1,int line2,int coloumn2,int **first,int **second,int **third);
int sum_elements_matrix(int line,int coloumn,int **a);
void sort_by_element_sum(struct octave *element,int size);
int **transposition_matrix(struct octave element);
void complement_function_C();
int **resize_matrix(int *line,int *coloumn,int **matrix);
void square_matrix_multiplication(int line,int **first,int **second);
void matrix_pow(int line,int coloumn,int pow,int **matrix);
void matrix_subb(int line,int **a,int **b,int **c);
void matrix_add(int line,int **a,int **b,int **c);
void Strassen_algorithm_helper(int **a,int **b,int **c,int size);
void Strassen_algorithm(int **a,int **b,int **c,int line);
int nextpower2of2(int k);
void add(int **first,int **second,int **third,int line);
void sub(int **a,int **b,int **c,int line);
void transform(int line,int **c);