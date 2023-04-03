#include "interface.h"
#include "vma.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NMAX 128

int interface_handler(uint64_t *address, uint64_t *size, int8_t **pointer)
{
	char string[NMAX];

	scanf("%s", string);
	int ok;
	if (strlen(string) == 0)
		return 0;

	char *p = strtok(string, " ");

	if (strcmp(p, "ALLOC_ARENA") == 0)
		return arena_interface(size);
	(void)ok;
	if (strcmp(p, "PMAP") == 0)
		return pmap_interface();

	if (strcmp(p, "DEALLOC_ARENA") == 0)
		return dealloc_arena_interface();
	(void)ok;
	if (strcmp(p, "ALLOC_BLOCK") == 0)
		return alloc_block_interface(address, size);
	(void)ok;
	if (strcmp(p, "FREE_BLOCK") == 0)
		return free_block_interface(address);
	(void)ok;
	if (strcmp(p, "READ") == 0)
		return read_interface(address, size);
	(void)ok;
	if (strcmp(p, "WRITE") == 0)
		return write_interface(address, size, pointer);

	return 0;
}

void print_error(int ok)
{
	for (int i = 0; i < ok; ++i)
		printf("Invalid command. Please try again.\n");
}

int arena_interface(uint64_t *size)
{
	char string[NMAX];
	fgets(string, NMAX, stdin);

	for (size_t i = 0; i < strlen(string); i++)
		if (string[i] == '\n')
			string[i] = '\0';

	int ok = 0, spaces = 0;
	for (size_t i = 0; i < strlen(string); i++) {
		if (isalpha(string[i]) != 0)
			ok++;

		if (string[i] == ' ')
			spaces++;
	}

	if (spaces > 1) {
		print_error(spaces);
		return 0;
	}

	if (ok != 0) {
		printf("Invalid command. Please try again.\n");
		return 0;
	}

	uint64_t ch1 = -1;
	int cc = 0;
	char *p = strtok(string, " ");
	while (p) {
		if (cc == 0)
			ch1 = atoi(p);
		cc++;
		p = strtok(NULL, " ");
	}

	*size = ch1;
	return 1;
}

int pmap_interface(void)
{
	char string[NMAX];
	fgets(string, NMAX, stdin);

	for (size_t i = 0; i < strlen(string); i++)
		if (string[i] == '\n')
			string[i] = '\0';
	char *p = strtok(string, " ");
	int ok = 0;
	while (p) {
		ok++;
		printf("Invalid command. Please try again.\n");
		p = strtok(NULL, " ");
	}
	if (ok == 0)
		return 7;

	return 0;
} // works

int dealloc_arena_interface(void)
{
	char string[NMAX];
	fgets(string, NMAX, stdin);

	for (size_t i = 0; i < strlen(string); i++)
		if (string[i] == '\n')
			string[i] = '\0';

	char *p = strtok(string, " ");
	int ok = 0;
	while (p) {
		ok++;
		printf("Invalid command. Please try again.\n");
		p = strtok(NULL, " ");
	}
	if (ok == 0)
		return 2;

	return 0;
}

int alloc_block_interface(uint64_t *address, uint64_t *size)
{
	char string[NMAX];
	fgets(string, NMAX, stdin);

	for (size_t i = 0; i < strlen(string); i++)
		if (string[i] == '\n')
			string[i] = '\0';

	int ok = 0, spaces = 0;
	for (size_t i = 0; i < strlen(string); i++) {
		if (isalpha(string[i]) != 0)
			ok++;

		if (string[i] == ' ')
			spaces++;
	}

	if (spaces <= 1) {
		printf("Invalid command. Please try again.\n");
		return 0;
	}

	if (spaces > 2) {
		print_error(spaces);
		return 0;
	}

	if (ok != 0) {
		printf("Invalid command. Please try again.\n");
		printf("Invalid command. Please try again.\n");
		return 0;
	}
	uint64_t ch1 = -1, ch2 = -1;
	int cc = 0;
	char *p = strtok(string, " ");
	while (p) {
		if (cc == 0)
			ch1 = atoi(p);
		else if (cc == 1)
			ch2 = atoi(p);
		cc++;
		p = strtok(NULL, " ");
	}

	*address = ch1;
	*size = ch2;
	return 3;
}

int free_block_interface(uint64_t *address)
{
	char string[NMAX];
	fgets(string, NMAX, stdin);

	for (size_t i = 0; i < strlen(string); i++)
		if (string[i] == '\n')
			string[i] = '\0';

	int ok = 0, spaces = 0;
	for (size_t i = 0; i < strlen(string); i++) {
		if (isalpha(string[i]) != 0)
			ok++;

		if (string[i] == ' ')
			spaces++;
	}

	if (spaces > 1) {
		print_error(spaces);
		return 0;
	}

	if (ok != 0) {
		printf("Invalid command. Please try again.\n");
		return 0;
	}

	uint64_t ch1 = -1;
	int cc = 0;
	char *p = strtok(string, " ");
	while (p) {
		if (cc == 0)
			ch1 = atoi(p);
		cc++;
		p = strtok(NULL, " ");
	}

	*address = ch1;
	return 4;
} //works

int read_interface(uint64_t *address, uint64_t *size)
{
	char string[NMAX];
	fgets(string, NMAX, stdin);

	for (size_t i = 0; i < strlen(string); i++)
		if (string[i] == '\n')
			string[i] = '\0';

	int ok = 0, spaces = 0;
	for (size_t i = 0; i < strlen(string); i++) {
		if (isalpha(string[i]) != 0)
			ok++;

		if (string[i] == ' ')
			spaces++;
	}

	if (spaces <= 1) {
		printf("Invalid command. Please try again.\n");
		return 0;
	}

	if (spaces > 2) {
		print_error(spaces);
		return 0;
	}

	if (ok != 0) {
		printf("Invalid command. Please try again.\n");
		printf("Invalid command. Please try again.\n");
		return 0;
	}
	uint64_t ch1 = -1, ch2 = -1;
	int cc = 0;
	char *p = strtok(string, " ");
	while (p) {
		if (cc == 0)
			ch1 = atoi(p);
		else if (cc == 1)
			ch2 = atoi(p);
		cc++;
		p = strtok(NULL, " ");
	}

	*address = ch1;
	*size = ch2;

	return 5;
}

int write_interface(uint64_t *address, uint64_t *size, int8_t **pointer)
{
	char string[NMAX];
	scanf("%s", string);

	int ok = 0;
	for (size_t i = 0; i < strlen(string); ++i) {
		if (isalpha(string[i]) != 0)
			ok++;
	}

	if (ok != 0) {
		printf("Invalid command. Please try again.\n");
		return 0;
	}
	*address = atoi(string);

	scanf("%s", string);

	ok = 0;
	for (size_t i = 0; i < strlen(string); ++i) {
		if (isalpha(string[i]) != 0)
			ok++;
	}

	if (ok != 0) {
		printf("Invalid command. Please try again.\n");
		return 0;
	}
	*size = atoi(string);
	char c = getchar();
	*pointer = malloc(sizeof(int8_t) * (*size) + 1);
	for (uint64_t i = 0; i < *size; i++) {
		c = getchar();
		(*pointer)[i] = c; // Fixed indexing of pointer
	}
	(*pointer)[*size] = '\0'; // Fixed indexing of pointer
	return 6;
}
