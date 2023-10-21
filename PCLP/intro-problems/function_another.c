/// Copyright 2022 Stoica Mihai Bogdan 315CA (bogdanstoicasn@yahoo.com)
#include <stdio.h>

#include "./function_another.h"
#define NMAX 401
void scan_matrix(int matrix[NMAX][NMAX], int n)
{
	for (int i = 0; i < n * n; i++)
	{
		for (int j = 0; j < n * n; j++)
		{
			scanf("%d", &matrix[i][j]);
		}
	}
}

int check_number_line(int matrix[NMAX][NMAX], int n, int row, int num)///if number appears on line
{
	int i, ok = 0;
	for (i = 0; i < n * n; i++) {
		if (matrix[row][i] == num) ok++;
		if (ok >= 2) break;
	}
	return ok;
}

int check_number_coloumn(int matrix[NMAX][NMAX], int n, int coloumn, int num)///if number appears on coloumn
{
	int i, ok = 0;
	for (i = 0; i < n * n; i++) {
		if (matrix[i][coloumn] == num) ok++;
		if (ok >= 2) break;
	}
	return ok;
}

int  check_number_box(int matrix[NMAX][NMAX], int n, int z, int num)///if number appears in box, if we know number of box
{
	z--;
	int row, coloumn;
	row = (z / n) * n;
	coloumn = (z % n) * n;
	for (int r = 0; r < n; r++) {
		for (int c = 0; c < n; c++) {
			if (matrix[r + row][c + coloumn] == num) return 0;
		}
	}
	return 1;
}

int check_number_box_by_line_coloumn(int matrix[NMAX][NMAX], int n, int row, int coloumn, int num)///if numbers appear in box, if we know coordinates
{
	int r, c, ok = 0;
	row = (row / n) * n;
	coloumn = (coloumn / n) * n;
	for (r = 0; r < n; r++) {
		for (c = 0; c < n; c++) {
			if (matrix[row + r][coloumn + c] == num) ok++;
		}
	}
	return ok;
}

int check_number_cell(int matrix[NMAX][NMAX], int n, int row, int coloumn, int num)///if number can be put in cell
{
	if (check_number_box_by_line_coloumn(matrix, n, row, coloumn, num) == 0) {
		if (check_number_coloumn(matrix, n, coloumn, num) == 0) {
			if (check_number_line(matrix, n, row, num) == 0) return 1;
			else return 0;
		} else return 0;
	} else return 0;
}

void change_cell(int matrix[NMAX][NMAX], int row, int coloumn, int value)///changes number
{
	if (matrix[row][coloumn] == 0) matrix[row][coloumn] = value;
}

int table_state(int matrix[NMAX][NMAX], int n)///table state
{
	int i, j, ibox, jbox, box;
	for (i = 0; i < n * n; i++)
	{
		int vfr[NMAX] = {0};
		for (j = 0; j < n * n; j++)
			{
				vfr[matrix[i][j]]++;
			if (vfr[matrix[i][j]] > 1) break;
			}
		for (j = 1; j <= n * n; j++)
			if (vfr[j] > 1) return 1;///checks the lines
	}
	for (i = 0; i < n * n; i++)
	{
		int vfr[NMAX] = {0};
		for (j = 0; j < n * n; j++)
			{
				vfr[matrix[j][i]]++;
				if (vfr[matrix[i][j]] > 1) break;
			}
		for (j = 1; j <= n * n; j++)
			if (vfr[j] > 1) return 1; /// checks the coloumns
	}
	for (box = 0; box < n * n; box++)
	{
		ibox = (box / n) * n;
		jbox = (box % n) * n;
		int vfr[NMAX] = {0};
		for (i = ibox; i < ibox + n; i++)
		{
			for (j = jbox; j < jbox + n; j++)
			{
				vfr[matrix[i][j]]++;
				if (vfr[matrix[i][j]] > 1) break;
			}
		}
		for (i = 1; i < n * n; i++)
		{
			if (vfr[i] > 1) return 1;///checks the box
		}
	}
	for (i = 0; i < n * n; i++)
	{
		for (j = 0; j < n * n; j++)
		{
			if (matrix[i][j] == 0) return 0;
		}
	}
	return 2;
}

void scan1(int matrix[NMAX][NMAX], int n)///every element with check_number_line
{
	int x, i, ok;
	scanf("%d", &x);
	x--;
	for (i = 1; i <= n * n; i++) {
		ok = check_number_line(matrix, n, x, i);
		if (ok == 0) printf("1 ");
		else printf("0 ");
	}
	printf("\n");
}

void scan2(int matrix[NMAX][NMAX], int n)///every element with check_number_coloumn
{
	int y, i, ok;
	scanf("%d", &y);
	y--;
	for (i = 1; i <= n * n; i++) {
		ok = check_number_coloumn(matrix, n, y, i);
		if (ok == 0) printf("1 ");
		else printf("0 ");
	}
	printf("\n");
}

void scan3(int matrix[NMAX][NMAX], int n)///every element with check_number_box
{
	int z, i;
	scanf("%d", &z);
	for (i = 1; i <= n * n; i++) {
		if (check_number_box(matrix, n, z, i) == 1) printf("1 ");
		else printf("0 ");
	}
	printf("\n");
}

void scan4(int matrix[NMAX][NMAX], int n)///every element at that coordinates with check_number_cell
{
	int x, y, i;
	scanf("%d%d", &x, &y);
	x--; y--;
	if (matrix[x][y] != 0) {
		for (i = 1; i <= n * n; i++)
			printf("0 ");
	} else {
		for (i = 1; i <= n * n; i++) {
		if (check_number_cell(matrix, n, x, y, i) == 1) printf("1 ");
		else printf("0 ");
	}
	}
	printf("\n");
}

void scan5(int matrix[NMAX][NMAX])///changes the cell
{
	int x, y, c;
	scanf("%d%d%d", &x, &y, &c);
	x--; y--;
	change_cell(matrix, x, y, c);
}
