// Copyright 2023-2024 Stoica Mihai-Bogdan (bogdanstoicasn@yahoo.com)
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "my_struct.h"
#include "utils.h"
#include "remove.h"

// function that searches for the position of the word in the dictionary
// and then removes it
void remove_function(dictionary *dict, char *word)
{
	int index = word[0] - 'a', ok = 0, position = 0;

	character_entry *entry = dict->data[index];

	int low = 0;
	int high = entry->size - 1;

	while (low <= high) {
		int mid = (low + high) / 2;
		word_entry *word_exis = entry->data[mid];

		int comparison = strcmp(word_exis->word, word);

		if (comparison == 0) {
			position = mid;
			ok = 1;
			break;
		} else if (comparison < 0) {
			low = mid + 1;
		} else {
			high = mid - 1;
		}
	}

	if (ok == 0)
		return;

	word_entry *word_exis = entry->data[position];

	for (int i = position; i < entry->size - 1; ++i)
		entry->data[i] = entry->data[i + 1];

	entry->size--;

	free(word_exis->word);
	free(word_exis);
}
