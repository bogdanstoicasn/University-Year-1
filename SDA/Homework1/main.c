#include "vma.h"
#include "interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_MAX 128

int main()
{
  	arena_t *arena = NULL;
  	int ok = -1, address = -1, size = -1;
  	char *pointer = NULL;
  	while (1) {
    	ok = interface_handler(&address, &size, pointer);
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
				// TO DO
				// error function
				alloc_block(arena, address, size);
				break;
			case 4:
				// TO DO
				// free block + error
				printf("%d == address to free\n",address);
				break;
			case 5:
				// TO DO
				// read + error
				printf("%d cu marime de %d\n",address, size);
				break;
			case 6:
				// TO DO 
				// write + error
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