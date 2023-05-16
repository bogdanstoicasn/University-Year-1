// Copyright 2023-2024 Stoica Mihai-Bogdan (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "my_struct.h"
#include "utils.h"
#include "insert.h"

void insert_function(dictionary *dict, char *word)
{
	int index = word[0] - 'a';
	
	character_entry *entry = dict->data[index];

	for(int i = 0; i < entry->size; ++i) {
		word_entry *word_exis = entry->data[i];
		if(strcmp(word_exis->word, word) == 0) {
			word_exis->frequency++;
			return;
		}
	}

	word_entry *new_word = malloc(sizeof(word_entry));
	DIE(new_word == NULL, "Error allocating memory for new word");

	new_word->word = malloc(strlen(word) + 1);
	DIE(new_word->word == NULL, "Error allocating memory for new word");
	((char *)(new_word->word))[strlen(word)] = '\0';

	memcpy(new_word->word, word, strlen(word));
	new_word->frequency = 1;
	
	entry->size++;
	entry->data = realloc(entry->data, entry->size * sizeof(void *)); // crapa daca puneam (enrty->data ++)
	DIE(entry->data == NULL, "Error reallocating memory for entry data");
	entry->data[entry->size - 1] = new_word;

	qsort(entry->data, entry->size, sizeof(void *), compare_strings);
}