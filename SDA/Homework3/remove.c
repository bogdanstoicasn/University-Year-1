// Copyright 2023-2024 Stoica Mihai-Bogdan (bogdanstoicasn@yahoo.com)
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "my_struct.h"
#include "utils.h"
#include "remove.h"

void remove_function(dictionary *dict, char *word)
{
	int index = word[0] - 'a', ok = 0, position = 0;

	character_entry *entry = dict->data[index];

	for (int i = 0; i < entry->size; ++i) {
		word_entry *word_exis = entry->data[i];
		if (strcmp(word_exis->word, word) == 0) {
			position = i;
			ok = 1;
			break;
		}
	}
	// printf("position == %d si ok == %d\n", position, ok);
	
	if (ok == 0)
		return;

	word_entry *word_exis = entry->data[position];

	for (int i = position; i < entry->size - 1; ++i)
		entry->data[i] = entry->data[i + 1];

	entry->size--;

	free(word_exis->word);
	free(word_exis);
}
