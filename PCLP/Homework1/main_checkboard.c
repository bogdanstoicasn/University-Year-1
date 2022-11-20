/// Copyright 2022 Stoica Mihai Bogdan 315CA (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include "./function_checkboard.h"
#define NMAX 1000
#define NUM 3

int main(void)
{
	int n;
	long matrix[NMAX][NMAX], x_y_distance[NUM];
	n = scanf_matrix(matrix);
	for (int i = 0; i < 3; i++)
	{
		x_y_distance[i] = 0;
	}
	pos_distance(matrix, x_y_distance, n);
	printf("%ld\n", x_y_distance[2]);
	printf("%ld ", x_y_distance[0]);
	print_index(x_y_distance[1]);
	printf("\n");

	return 0;
}

