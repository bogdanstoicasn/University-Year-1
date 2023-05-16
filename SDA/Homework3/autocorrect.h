// Copyright 2023-2024 Stoica Mihai-Bogdan (bogdanstoicasn@yahoo.com)
#ifndef SKEL_AUTOCORRECT_H
#define SKEL_AUTOCORRECT_H

void autocorrect_function(dictionary *dict, char *word, int n);

int compare_words_char(const char *a, const char *b, size_t size_a, size_t size_b);

#endif