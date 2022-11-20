/// Copyright 2022 Stoica Mihai Bogdan 315CA (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include <math.h>
#include "./function_prod.h"

int main(void)
{
	unsigned long n;
	 long long ai, bi, ps = 0;
	long long a_max = -1, maxim_a =  -1, b_max =  -1, maxim_b =  -1;
	double n_a = 0, n_b = 0;
	scanf("%ld", &n);
	for (unsigned long i = 1; i <= n; i++)
	{
		scanf("%llo%llo", &ai, &bi);
		scalar_product(ai, bi, &ps);///scalar product calculator
		if (maxim_a < ai)///second maxim calculator ai
		{
			a_max = maxim_a;
			maxim_a = ai;
		} else if (ai > a_max && ai != maxim_a) a_max = ai;
		if (maxim_b < bi)///second maxim calculator for bi
		{
			b_max = maxim_b;
			maxim_b = bi;
		} else if (bi > b_max && bi != maxim_b) b_max = bi;
		norma_sum(ai, &n_a);///norm calculator
		norma_sum(bi, &n_b);
	}
	n_a = sqrt(n_a);
	n_b = sqrt(n_b);
	printf("%lld\n", ps);
	if (a_max !=  -1) printf("%lld ", a_max);
	else printf("-1 ");
	if (b_max !=  -1) printf("%lld\n", b_max);
	else printf("-1\n");
	printf("%0.7lf%c%0.7lf\n", n_a, ' ', n_b);
	return 0;
}
