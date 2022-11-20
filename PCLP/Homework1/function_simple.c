/// Copyright 2022 Stoica Mihai Bogdan 315CA (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include "./function_simple.h"
#define NMAX 27

int cmmdc(int a, int b)///greatest common div
{
	int r;
	while (b)
	{
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}

int index_calculator(int fr[NMAX])///max element index calculator
{
	int i, maxi =  -1, index_maxi;
	for (i = 0; i < 27; i++)
	{
		if (fr[i] > maxi)
		{
			maxi = fr[i];
			index_maxi = i;
		}
	}
	return index_maxi;
}

void scan_Q(int fr[NMAX])///shows number of aparitions(Q)
{
	char s;
	scanf(" %c", &s);
	printf("%c %d\n", s, fr[s - 'a']);
}

void delete_E(int fr[NMAX], long long *count_all_char)//delete frequency(E)
{
	char s;
	scanf(" %c", &s);
	*count_all_char -= fr[s - 'a'];
	fr[s - 'a'] = 0;
}

void scan_T(int fr[NMAX])///first k elements in descending order(T)
{
	int k, copyfr[NMAX], index = 0;
	scanf(" %d", &k);
	for (int i = 0; i < 27; i++)
	{
		copyfr[i] = fr[i];
	}
	while (k > 0)
	{
		k--;
		index = index_calculator(copyfr);
		if (k == 0) printf("%c", index + 'a');
		else printf("%c ", index + 'a');
		copyfr[index] =  -2;
	}
	printf("\n");
}

int  error_case(int fr[NMAX], long long count_all_char, int count_error)///error case
{
	int index = index_calculator(fr);
	if (fr[index] >= ((count_all_char % 2 != 0) ? (count_all_char / 2 + 1) : (count_all_char / 2)))
	{
		if (count_error > 4)
		{
			int cmmdc1;
			cmmdc1 = cmmdc(fr[index], count_all_char);
			printf("%c %d/%lld", index + 'a', fr[index] / cmmdc1, count_all_char / cmmdc1);
			printf("\n");
			return 0;
		} else return count_error;
	} else return count_error;
}

