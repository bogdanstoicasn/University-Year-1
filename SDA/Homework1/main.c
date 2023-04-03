#include "vma.h"
#include "interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#define STRING_MAX 128

int main(void)
{
	arena_t *arena = NULL;
	uint64_t ok = -1, address = -1, size = -1, errors = -1;
	int8_t *pointer = NULL;
	while (1) {
		ok = interface_handler(&address, &size, &pointer);
		switch (ok) {
		case 0:
				printf("Invalid command. Please try again.\n");
				break;
		case 1:
				arena = alloc_arena(size);
				break;
		case 2:
				dealloc_arena(arena);
				break;
		case 3:
				errors = alloc_block_perrror(arena, address, size);
				if (errors == 1)
					alloc_block(arena, address, size);
				break;
		case 4:
				errors = address_free_perror(arena, address);
				if (errors == 1)
					free_block(arena, address);
				// free block + error
				break;
		case 5:
				errors = address_read_perror(arena, address);
				if (errors == 1)
					read(arena, address, size);
				break;
		case 6:
				errors = address_write_perror(arena, address);
				if (errors == 1)
					write(arena, address, size, pointer);
				free(pointer);
				break;
		case 7:
				pmap(arena);
				break;
		}
		if (ok == 2)
			break;
	}
	return 0;
}
