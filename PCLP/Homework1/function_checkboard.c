/// Copyright 2022 Stoica Mihai Bogdan 315CA (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include "./function_checkboard.h"
#define NMAX 1000
#define NUM 3
int scanf_matrix(long matrix[NMAX][NMAX])///scan of matrix
{
	int n;
	scanf("%d", &n);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			scanf("%ld", &matrix[i][j]);
		}
	}
	return n;
}

void pos_distance(long matrix[NMAX][NMAX], long x_y_distance[NUM], int n)///distance,line,coloumn
{
	long pos_x = 0, pos_y = 0, value_element, distance = 0;
	while (matrix[pos_x][pos_y] != 0)
	{
		value_element = matrix[pos_x][pos_y];
		matrix[pos_x][pos_y] = 0;
		if ((pos_x + pos_y) % 2 != 0)
		{
			pos_x += value_element;
		} else pos_y += value_element;
		while (pos_x >= n) pos_x -= n;
		while (pos_x < 0) pos_x += n;
		while (pos_y >= n) pos_y -= n;
		while (pos_y < 0) pos_y += n;
		if (value_element < 0) value_element =  -value_element;
		distance += value_element;
	}
	pos_x++;
	x_y_distance[0] = n - pos_x + 1;
	x_y_distance[1] = pos_y;
	x_y_distance[2] = distance;
}

void print_index(int a)///base 26 transformation
{
	if (a < 26)
	{
		printf("%c", 'A' + a);
	} else
	{
		print_index(a / 26 - 1);
		print_index(a % 26);
	}
}
