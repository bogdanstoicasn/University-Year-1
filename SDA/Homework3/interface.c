// Copyright 2023-2024 Stoica Mihai-Bogdan (bogdanstoicasn@yahoo.com)
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "interface.h"

#define ALPHABET_SIZE 26

#define MAX_STRING_SIZE 64
#define HALF_REGISTER_SIZE 16

// function that gets the command from the user and calls the corresponding
// function
int interface_handler(char word[MAX_STRING_SIZE], int *n)
{
	int ok = 0;
	char comm[MAX_STRING_SIZE], copy_comm[MAX_STRING_SIZE];
	fgets(comm, MAX_STRING_SIZE, stdin);

	for (size_t i = 0; i < strlen(comm); ++i) {
		if (comm[i] == '\n') {
			comm[i] = '\0';
			break;
		}
	}

	strcpy(copy_comm, comm);

	char *token = strtok(comm, " ");

	if (strcmp(token, "INSERT") == 0) {
		ok = command_typo_insert(copy_comm, word);
		return ok;
	}

	if (strcmp(token, "EXIT") == 0)
		return 6;

	if (strcmp(token, "LOAD") == 0) {
		ok = command_typo_load(copy_comm, word);
		return ok;
	}

	if (strcmp(token, "REMOVE") == 0) {
		ok = command_typo_remove(copy_comm, word);
		return ok;
	}

	if (strcmp(token, "AUTOCORRECT") == 0) {
		ok = command_typo_correct(copy_comm, word, n);
		return ok;
	}

	if (strcmp(token, "AUTOCOMPLETE") == 0) {
		ok = command_typo_complete(copy_comm, word, n);
		return ok;
	}

	return 0;
}

int command_typo_insert(char string[MAX_STRING_SIZE],
						char word[MAX_STRING_SIZE])
{
	char *token = strtok(string, " ");
	token = strtok(NULL, " ");

	strcpy(word, token);

	return 1;
}

int command_typo_load(char string[MAX_STRING_SIZE], char word[MAX_STRING_SIZE])
{
	char *token = strtok(string, " ");
	token = strtok(NULL, " ");

	strcpy(word, token);

	return 2;
}

int command_typo_remove(char string[MAX_STRING_SIZE],
						char word[MAX_STRING_SIZE])
{
	char *token = strtok(string, " ");
	token = strtok(NULL, " ");

	strcpy(word, token);

	return 3;
}

int command_typo_correct(char string[MAX_STRING_SIZE],
						 char word[MAX_STRING_SIZE], int *n)
{
	char *token = strtok(string, " ");
	token = strtok(NULL, " ");

	strcpy(word, token);

	token = strtok(NULL, " ");

	*n = atoi(token);

	return 4;
}

int command_typo_complete(char string[MAX_STRING_SIZE],
						  char word[MAX_STRING_SIZE], int *n)
{
	char *token = strtok(string, " ");
	token = strtok(NULL, " ");

	strcpy(word, token);

	token = strtok(NULL, " ");

	*n = atoi(token);

	return 5;
}
