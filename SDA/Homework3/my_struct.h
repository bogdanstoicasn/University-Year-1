// Copyright 2023-2024 Stoica Mihai-Bogdan (bogdanstoicasn@yahoo.com)
#ifndef MY_STRUCT_H
#define MY_STRUCT_H

typedef struct {
	void **data;
	int size;
} dictionary;

typedef struct {
	void **data;
	int size;
} character_entry;

typedef struct {
	void *word;
	int frequency;
} word_entry;

dictionary *create_dictionary(void);

void dictionary_destructor(dictionary *dict);

int compare_strings(const void *a, const void *b);

void dictionary_printer(dictionary *dict);

#endif
