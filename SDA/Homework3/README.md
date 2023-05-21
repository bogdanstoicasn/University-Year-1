#### Copyright 2023-2024 Stoica Mihai-Bogdan 315CA (bogdanstoicasn@yahoo.com)
#### GNU GENERAL PUBLIC LICENSE 
#### Version 3, 29 June 2007

# Magic Keyboard

## Description

This is a README file that contains details and explanations
about the "Magic Keyboard" project.

# Specifics

The files contained in this project are as follows:

> autocomplete.c, autocorrect.c, insert.c, interafce.c, load.c, my_struct.c,
remove.c, mk.c

> autocomplete.h, autocorrect.h, insert.h, interafce.h, load.h, my_struct.h,
remove.c, utils.h

There may be constant definitions such as **MAX_STRING_SIZE** with a value of 64

## Magic Keyboard

We propose to implement a word correction/completion system for a user.
Our goal is to efficiently use data structures to perform operations
as quickly as possible.

The structure looks like this:

> **DICTIONARY**->**CHARACTER_ENTRY**->**WORD_ENTRY**

The 'dictionary' structure holds an array of pointers, each element representing
a specific entry for each character in the english alphabet. Each entry is
then connected to the 'character_entry' structure. The structure mentioned
earlier holds again an array of pointers, each element of that array
being connected to another structure called 'word_entry', which holds
the word and it's frequency.

The implementation that I used, is not very quick, but the words are
in ascending order and each word is in it's specific character entry.
This thing ensures that we can access the word we want in a timely manner (binary search).

Functions:

> **insert**: we calculate the index, then use binary search to see if word
already exists, if not we add to the vector using 'qsort'

> **load**: reading words from a file

> **remove**: removing word from it's specific character entry using binary search

> **autocorrect**: we print on screen all the words that differ from our
chosen word by 'k' characters

> **autocomplete**: show most frequent word or shortest or smallest lexicographic

> **exit**: we free all the allocated memory

## Resources

-> [MACROS](https://gcc.gnu.org/onlinedocs/cpp/Macros.html)

-> [DEFINE](https://learn.microsoft.com/en-us/cpp/preprocessor/hash-define-directive-c-cpp?view=msvc-170)

