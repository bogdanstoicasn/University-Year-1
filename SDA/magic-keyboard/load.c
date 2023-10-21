// Copyright 2023-2024 Stoica Mihai-Bogdan (bogdanstoicasn@yahoo.com)
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_struct.h"
#include "utils.h"
#include "insert.h"
#include "load.h"

#define MAX_STRING_SIZE 64
#define HALF_REGISTER_SIZE 16

// loads the words from the file into the dictionary
void load_function(dictionary *dict, char *file)
{
	FILE *fptr = fopen(file, "r");
	if (!fptr) {
		printf("File %s not found\n", file);
		return;
	}
	char word[MAX_STRING_SIZE];

	while (fscanf(fptr, "%s", word) != EOF)
		insert_function(dict, word);

	fclose(fptr);
}
