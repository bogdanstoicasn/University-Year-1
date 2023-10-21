/// Copyright 2022 Stoica Mihai Bogdan 315CA (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include "./function_simple.h"
#define NMAX 27

int main(void)
{
	int fr[NMAX], i, count_error = 0, ok = 0;
	long long count_all_char = 0;
	char copy, c;
	for (i = 0; i < 27; i++)
	{
		fr[i] = 0;
	}
	scanf("%c", &c);
	copy = c;
	while ((c >= 'a' && c <= 'z') || c == 'T' || c == 'E' || c == 'Q')
	{
		if (c >= 'a' && c <= 'z')
		{
			fr[c - 'a']++;
			count_all_char++;
			count_error++;
			if (ok != 0) count_error = error_case(fr, count_all_char, count_error);
		} else
		{
			switch (c)
			{
				case 'Q':
					scan_Q(fr);
					break;
				case 'T':
					scan_T(fr);
					break;
				case 'E':
					delete_E(fr, &count_all_char);
					break;
			}
		}
		scanf(" %c", &c);
		if ((c >= 'a' && c <= 'z') && copy != c) ok = 1;
	}
	return 0;
}
