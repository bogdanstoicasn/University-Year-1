// Copyright 2023-2024 Stoica Mihai-Bogdan (bogdanstoicasn@yahoo.com)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "my_struct.h"

#define ALPHABET_SIZE 26

// function that compares two words and returns the number of
// different characters
int compare_words_char(const char *a, const char *b,
					   size_t size_a, size_t size_b)
{
	if (size_a != size_b)
		return -1;

	int ok = 0;
	for (size_t i = 0; i < size_a; i++)
		if (a[i] != b[i])
			ok++;

	return ok;
}

// autocorrect function
void autocorrect_function(dictionary *dict, char *word, int n)
{
	int truth = 0;
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		character_entry *entry = dict->data[i];
		for (int j = 0; j < entry->size; j++) {
			word_entry *word_exis = entry->data[j];
			char *word_str = word_exis->word;
			int ok = compare_words_char(word, word_str, strlen(word),
										strlen(word_str));
			if (ok <= n && ok > -1) {
				printf("%s\n", word_str);
				truth = 1;
			}
		}
	}
	if (truth == 0)
		printf("No words found\n");
}
