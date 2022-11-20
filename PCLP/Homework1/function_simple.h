/// Copyright 2022 Stoica Mihai Bogdan 315CA (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#define NMAX 27
void scan_Q(int fr[NMAX]);///shows numer of aparitions(Q)
void delete_E(int fr[NMAX], long long *count_all_char);///delete frequency(E)
int cmmdc(int num1, int num2);///greatest common div
void scan_T(int fr[NMAX]);///first k elements in descending order(T)
int index_calculator(int fr[NMAX]);///max element index calculator
int error_case(int fr[NMAX], long long count_all_char, int count_error);///error case
