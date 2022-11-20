/// Copyright 2022 Stoica Mihai Bogdan 315CA (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include "./function_another.h"
#define NMAX 401

int main(void)
{
	int matrix[NMAX][NMAX], n;
	long long m;
	char c;
	scanf("%d %lld\n", &n, &m);
	scan_matrix(matrix, n);
	while (m) {
		m = m - 1;
		scanf(" %c", &c);
		switch (c)
		{
			case '1':
			scan1(matrix, n);///every element with check_number_line
			break;
			case '2':
			scan2(matrix, n);///every element with check_number_coloumn
			break;
			case '3':
			scan3(matrix, n);///every element with check_number_box
			break;
			case '4':
			scan4(matrix, n);///every element at that coordinates with check_number_cell
			break;
			case '5':
			scan5(matrix);///changes the cell
			break;
			case '6':
			printf("%d\n", table_state(matrix, n));
			break;
			default:
			break;
		}
	}
	return 0;
}
