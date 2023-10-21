// Copyright 2023-2024 Stoica Mihai-Bogdan (bogdanstoicasn@yahoo.com)
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_struct.h"
#include "utils.h"

#define ALPHABET_SIZE 26

// function that allocates memory for the dictionary
dictionary *create_dictionary(void)
{
	dictionary *dict = malloc(sizeof(dictionary));
	DIE(!dict, "Error allocating memory for dictionary");

	dict->data = calloc(ALPHABET_SIZE, sizeof(void *));
	DIE(!dict->data, "Error allocating memory for dictionary data");

	dict->size = 0;

	for (int i = 0; i < ALPHABET_SIZE; ++i) {
		character_entry *entry = malloc(sizeof(character_entry));
		DIE(!entry, "Error allocating memory for character entry");

		entry->data = calloc(1, sizeof(void *));
		DIE(!entry->data, "Error allocating memory for character entry data");

		entry->size = 0;

		dict->data[i] = entry;
	}

	return dict;
}

// function that compares two words
int compare_strings(const void *a, const void *b)
{
	const char *str1 = ((word_entry *)*((void **)a))->word;
	const char *str2 = ((word_entry *)*((void **)b))->word;
	return strcmp(str1, str2);
}

// function used for showing the words in the dictionary
void dictionary_printer(dictionary *dict)
{
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (dict->data[i]) {
			character_entry *entry = dict->data[i];
			printf("Litera %c/%d\n\n", i + 'a', entry->size);
			for (int j = 0; j < entry->size; j++) {
				word_entry *word = entry->data[j];
				char *word_str = (char *)word->word;
				printf("%s %d\n", word_str, word->frequency);
			}
			printf("\n");
		}
	}
}

// function that frees the memory allocated for the dictionary
void dictionary_destructor(dictionary *dict)
{
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (dict->data[i]) {
			character_entry *entry = (character_entry *)dict->data[i];
			for (int j = 0; j < entry->size; j++) {
				word_entry *word = (word_entry *)entry->data[j];
				free(word->word);
				free(word);
			}
			free(entry->data);
			free(entry);
		}
	}

	free(dict->data);
	free(dict);
}
