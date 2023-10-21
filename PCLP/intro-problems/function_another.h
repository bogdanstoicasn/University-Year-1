/// Copyright 2022 Stoica Mihai Bogdan 315CA (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#define NMAX 401

void scan_matrix(int matrix[NMAX][NMAX], int n);
int check_number_line(int matrix[NMAX][NMAX], int n, int row, int num);
int check_number_coloumn(int matrix[NMAX][NMAX], int n, int coloumn, int num);
int check_number_box(int matrix[NMAX][NMAX], int n, int z, int num);
int check_number_box_by_line_coloumn(int matrix[NMAX][NMAX], int n, int row, int coloumn, int num);
int check_number_cell(int matrix[NMAX][NMAX], int n, int row, int coloumn, int num);
void change_cell(int matrix[NMAX][NMAX], int row, int coloumn, int value);
int table_state(int matrix[NMAX][NMAX], int n);
void scan1(int matrix[NMAX][NMAX], int n);
void scan2(int matrix[NMAX][NMAX], int n);
void scan3(int matrix[NMAX][NMAX], int n);
void scan4(int matrix[NMAX][NMAX], int n);
void scan5(int matrix[NMAX][NMAX]);
