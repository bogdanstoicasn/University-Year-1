#include "vma.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STRING_MAX 128
int main()
{
	char comm[STRING_MAX];
  arena_t *arena = NULL;
  while (1) {
    scanf("%s", comm);

    if (strcmp(comm, "ALLOC_ARENA") == 0) {
      int size;
      scanf("%d", &size);
      arena = alloc_arena(size);
    }

    if (strcmp(comm, "ALLOC_BLOCK") == 0) {
      int adr_start, dim;
      scanf("%d %d", &adr_start, &dim);

      alloc_block(arena, adr_start, dim);
    }

    if (strcmp(comm, "PRINT") == 0) {
      int n =
          ((list_t *)((block_t *)arena->alloc_list->head->data)->miniblock_list)
              ->size;
      int m = ((block_t *)arena->alloc_list->head->data)->size;
      printf("%d %d %d\n", arena->alloc_list->size, n, m);
    }
	  if (strcmp(comm,"FREE") == 0) {
		  dealloc_arena(arena);
		  break;
	  }
    if (strcmp(comm,"PMAP") == 0)
      pmap(arena);
  }
	return 0;
}