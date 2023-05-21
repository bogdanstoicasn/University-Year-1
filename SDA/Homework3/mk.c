// Copyright 2023-2024 Stoica Mihai-Bogdan (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interface.h"
#include "my_struct.h"
#include "utils.h"
#include "insert.h"
#include "remove.h"
#include "load.h"
#include "autocomplete.h"
#include "autocorrect.h"

#define ALPHABET_SIZE 26

#define MAX_STRING_SIZE 64
#define HALF_REGISTER_SIZE 16

int main(void)
{
	int command_number, n;
	char word[MAX_STRING_SIZE];

	dictionary *dict = create_dictionary();

	while (1) {
		command_number = interface_handler(word, &n);
		switch (command_number) {
		case 0:
			// "You shouldn't be here :))
			break;
		case 1:
			insert_function(dict, word);
			break;
		case 2:
			load_function(dict, word);
			// printf("File %s successfully loaded\n", word);
			break;
		case 3:
			remove_function(dict, word);
			break;
		case 4:
			autocorrect_function(dict, word, n);
			break;
		case 5:
			autocomplete_function(dict, word, n);
			break;
		case 6:
			dictionary_destructor(dict);
			return 0;
		}
	}
	return 0;
}
