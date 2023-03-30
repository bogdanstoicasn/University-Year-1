#include "interface.h"
#include "vma.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NMAX 64

int interface_handler(int *address, int *size, char *pointer)
{
    char string[NMAX], copy_string[NMAX];
	
    fgets(string, NMAX, stdin);
	
    for (size_t i = 0; i < strlen(string); i++)
		if (string[i] == '\n')
			string[i] = '\0';

	if (strlen(string) == 0)
		return 0;

	strcpy(copy_string, string);

    char *p =strtok(string, " ");

    if (strcmp(p, "ALLOC_ARENA") == 0)
        return arena_interface(copy_string, size);

    if (strcmp(p, "PMAP") == 0)
        return pmap_interface(copy_string);

    if (strcmp(p, "DEALLOC_ARENA") == 0)
        return dealloc_arena_interface(copy_string);
    
    if (strcmp(p, "ALLOC_BLOCK") == 0)
        return alloc_block_interface(copy_string, address, size);

    if (strcmp(p, "FREE_BLOCK") == 0)
        return free_block_interface(copy_string, address);
    return 0;
}

int arena_interface(char copy_string[NMAX], int *size)
{
    char *p = strtok(copy_string, " ");
	int ok = 0;
	while (p) {
		++ok;
        if (ok == 2)
            *size = atoi(p); 
		p = strtok(NULL, " ");
	}

    if (ok <= 1 || ok >= 3)
        return 0;

    if (*size <= 0)
        return 0;

    return 1;
}

int pmap_interface(char copy_string[NMAX])
{
    char *p = strtok(copy_string, " ");
    int ok = 0;
    while (p) {
        ++ok;
        p = strtok(NULL, " ");
    }
    if (ok != 1) 
        return 0;

    return 7;
}

int dealloc_arena_interface(char copy_string[NMAX])
{
    char *p = strtok(copy_string, " ");
    int ok = 0;
    while (p) {
        ++ok;
        p = strtok(NULL, " ");
    }
    if (ok != 1) 
        return 0;

    return 2;
}

int alloc_block_interface(char copy_string[NMAX], int *address, int *size)
{
    char *p = strtok(copy_string, " ");
	int contor = 0, ch1 = -1, ch2 = -1, ok = 0;
	while (p) {
		if (p[0] >= 'A' && p[0] <= 'Z') {
			ok++;
		} else {
			if (contor == 0)
				ch1 = atoi(p);

			if (contor == 1)
				ch2 = atoi(p);

			contor++;
		}
	p = strtok(NULL, " ");
	}

	if (contor != 2)
        return 0;

    if (ch2 == 0)
        return 0;
    
    
    *address = ch1;
	*size = ch2;

	return 3;
}

int free_block_interface(char copy_string[NMAX], int *address)
{
    char *p = strtok(copy_string, " ");
	int ok = 0;
	while (p) {
		++ok;
        if (ok == 2)
            *address = atoi(p); 
		p = strtok(NULL, " ");
	}

    if (ok <= 1 || ok >=3)
        return 0;

    if (*address <= 0)
        return 0;

    return 4;
}