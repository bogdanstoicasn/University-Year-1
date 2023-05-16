// Copyright 2023-2024 Stoica Mihai-Bogdan (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "my_struct.h"
#include "autocomplete.h"


void autocomplete_function(dictionary *dict, char *prefix, int n)
{
	int index = prefix[0] - 'a', frecq = 0, ok = 0;
	size_t min_size = __INT32_MAX__;
	char *little_word = NULL, *freq_word = NULL, *lexico_word = NULL;
	character_entry *entry = dict->data[index];

	for (int i = 0; i <entry->size; ++i) {
		word_entry *word = entry->data[i];
		char *word_str = word->word;
		if (strncmp(word_str, prefix, strlen(prefix)) == 0) {
			// shortest word
			if (strlen(word_str) < min_size) {
				min_size = strlen(word_str);
				little_word = word_str;
			}

			if (word->frequency > frecq) {
				frecq = word->frequency;
				freq_word = word_str;
			}
			if (ok == 0) {
				++ok;
				lexico_word = word_str;
			}
		}
	}

	if (ok == 0) {
		printf("No words found\n");
		if (n == 0) {
			printf("No words found\n");
			printf("No words found\n");
			return;
		}
		return;
	}
	if (n == 1) {
		printf("%s\n", lexico_word);
		return;
	}

	if (n == 2) {
		printf("%s\n", little_word);
		return;
	}

	if (n == 3) {
		printf("%s\n", freq_word);
		return;
	}

	if (n == 0) {
		printf("%s\n", lexico_word);
		printf("%s\n", little_word);
		printf("%s\n", freq_word);
		return;
	}
}
