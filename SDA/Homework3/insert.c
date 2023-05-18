// Copyright 2023-2024 Stoica Mihai-Bogdan (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "my_struct.h"
#include "utils.h"
#include "insert.h"

void insert_function(dictionary *dict, char *word) {
    int index = word[0] - 'a';

    character_entry *entry = dict->data[index];

    // Perform binary search to check if the word already exists
    int left = 0;
    int right = entry->size - 1;

    while (left <= right) {
        int mid = (left + right) / 2;
        word_entry *word_exis = entry->data[mid];
        int cmp = strcmp(word_exis->word, word);

        if (cmp == 0) {
            word_exis->frequency++;
            return;
        } else if (cmp < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
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
