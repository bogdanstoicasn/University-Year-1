#### Copyright 2023-2024 Stoica Mihai-Bogdan (bogdanstoicasn@yahoo.com)
#### GNU GENERAL PUBLIC LICENSE 
#### Version 3, 29 June 2007

# **Virtual Memory Allocator**

## Description

This is a README file that contains details and explanations
about the "Virtual Memory Allocator" project.

## Specifics

The files included in this project are: *.c: vma.c, interface.c, my_struct.c,
main.c
The project also contains the header files "interface.h" and "vma.h".
The code is compiled using the 1999 standard.
There may be constant definitions, such as**NMAX**with a value of 128.

## Virtual Memory Allocator

The topic addressed is the construction of a virtual memory allocation
simulator, without using arrays.
The theme includes two doubly linked lists, one inside the other,
with all the links between them made using generic structures.

The key points of the project are:

> 1. Virtual allocation of the arena ("**ALLOC_ARENA**")

> 2. Allocation of memory blocks ("**ALLOC_BLOCK**")

> 3. Release of a single block ("**FREE_BLOCK**")

> 4. Writing to a specific address ("**WRITE**")

> 5. Reading from a specific address ("**READ**")

> 6. Release of all resources ("**DEALLOC_ARENA**")

The first easy problems were encountered in "**ALLOC_BLOCK**".
For correct memory allocation, we handled five cases:

> 1. The list is empty.

> 2. The blocks do not overlap.

> 3. The blocks overlap at the beginning (we release memory for "block").

> 4. The blocks overlap at the end (we release memory for "block").

> 5. The block overlaps 
both at the beginning and at the end (we keep only "miniblock").

A difficult part of implementing the project was releasing a single "miniblock".
The possible cases are:

> 1. The miniblock is at the beginning of the list.

> 2. The miniblock is at the end of the list.

> 3. The miniblock is in the middle of the list.

When we separate a "miniblock" from the middle, we had to create
a new memory area in the "block" list for the following "miniblocks".
Then we transferred the modified data (to the old "block")
and the new data (in the new "block").

Writing to a specific address had issues because 
we didn't check the existence of a next "miniblock" correctly. 
We solved it with a simple conditional structure.

A peculiarity of the read operation is that we ignore the "0" characters.

## Resources

-> "Understanding Virtual Memory" by Joel S. Hirschhorn 

-> "Building a Memory Allocator" by Eli Bendersky

-> [Computerphile](https://youtube.com/@Computerphile)

-> [David Black-Schaffer](https://www.youtube.com/playlist?list=PLiwt1iVUib9s2Uo5BeYmwkDFUh70fJPxX)
