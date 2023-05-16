// Copyright 2023-2024 Stoica Mihai-Bogdan (bogdanstoicasn@yahoo.com)
#ifndef INTERFACE_H
#define INTERFACE_H

#define ALPHABET_SIZE 26

#define MAX_STRING_SIZE 64
#define REGISTER_SIZE 32
#define HALF_REGISTER_SIZE 16

int
interface_handler(char word[MAX_STRING_SIZE], int *n);

int
command_typo_insert(char string[MAX_STRING_SIZE], char word[MAX_STRING_SIZE]);

int
command_typo_load(char string[MAX_STRING_SIZE], char word[MAX_STRING_SIZE]);

int
command_typo_remove(char string[MAX_STRING_SIZE], char word[MAX_STRING_SIZE]);

int
command_typo_correct(char string[MAX_STRING_SIZE], char word[MAX_STRING_SIZE], int *n);

int
command_typo_complete(char string[MAX_STRING_SIZE], char word[MAX_STRING_SIZE], int *n);

#endif