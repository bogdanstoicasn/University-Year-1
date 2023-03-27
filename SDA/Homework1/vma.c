
#include <stdlib.h>
#include <string.h>
#include "vma.h"


list_t*
dll_create(unsigned int data_size)
{
	list_t *new_node = malloc(sizeof(list_t));

    new_node->head = NULL;

    new_node->data_size = data_size;

    new_node->size = 0;
    
    return new_node; 
}

void
dll_add_nth_node(list_t* list, unsigned int n, const void* data)
{
    dll_node_t *new_node = malloc(sizeof(dll_node_t));

    new_node->data = malloc(list->data_size);
    memcpy(new_node->data, data, list->data_size);

    new_node->prev = NULL;
    new_node->next = NULL;

    dll_node_t* current = list->head;
      
    if (n > list->size) 
        n = list->size;

    if (n == 0) {
        if (list->size != 0) {
            new_node->next = list->head;
            list->head->prev = new_node;
            list->head = new_node;
        } else {
            list->head = new_node;
        }
    } else {
        for (unsigned int i = 0; i < n - 1; i++) 
            current = current->next;
        
        dll_node_t *temp = current->next;
        
        new_node->next = temp;
        current->next = new_node;
        
        new_node->prev = current;
        if (temp != NULL) {
            temp->prev = new_node;
        }
    }

    list->size++;
}


arena_t *alloc_arena(const uint64_t size)
{
	arena_t *arena = malloc(sizeof(arena_t));
	arena->arena_size = size;
	arena->alloc_list = dll_create(sizeof(block_t));
	
	list_t *list_blocks = arena->alloc_list;
	
	list_blocks->start_address = 0;
	list_blocks->end_address = size;

	return arena;
}

void alloc_block(arena_t* arena, const uint64_t address, const uint64_t size) // sa incerc sa leg direct blocul de minibloc
{
	if (arena == NULL)
		return;

	if (address + size > arena->arena_size)
		return;
	
	list_t *list_blocks = arena->alloc_list;

	if (list_blocks->head == NULL) {
		block_t *block = malloc(sizeof(block_t));

		list_t *list_miniblocks = malloc(sizeof(list_t));
		block->miniblock_list = list_miniblocks;

		miniblock_t *miniblock = malloc(sizeof(miniblock_t));

		// setting the list->head which points to miniblocks
		list_miniblocks->data_size = sizeof(miniblock_t);
		list_miniblocks->end_address = size + address;
		list_miniblocks->start_address = address;

		block->size = size;
		miniblock->size =size;

		block->start_address = address;
		miniblock->start_address = address;
		
		// rw buffer sa vad ce e de facut cu el
		miniblock->perm = 6;
		
		dll_add_nth_node(list_blocks, 0, block);
		dll_add_nth_node(list_miniblocks, 0, miniblock);


	}
}
