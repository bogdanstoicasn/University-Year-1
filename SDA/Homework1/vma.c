
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

dll_node_t*
dll_remove_nth_node(list_t* list, unsigned int n)
{
	if (list->head == NULL || n >= list->size) {
		return NULL;
	}

	list->size--;

	dll_node_t *current = list->head;

	if (n == 0) {
		list->head = current->next;
		if (list->head != NULL) {
			list->head->prev = NULL;
		}
		return current;
	}

	for (unsigned int i = 0; i < n; i++) {
		current = current->next;
	}

	current->prev->next = current->next;

	if (current->next != NULL) {
		current->next->prev = current->prev;
	}

	return current;
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

void dealloc_arena(arena_t* arena)
{
    list_t *first_head = arena->alloc_list;
	// de facut
}

int verify_address(arena_t *arena, const uint64_t address)
{   
    list_t *list_blocks = arena->alloc_list;
    dll_node_t *node = list_blocks->head;
    while (node!= NULL) {
        block_t *block = node->data;
        
        uint64_t add = block->start_address;
        uint64_t size = block->size;

        if (add + size == address)
            return 1;
        
        node = node->next;
    }
    return 0;
}

int verify_address_final(arena_t *arena, const uint64_t address)
{
	list_t *temp = arena->alloc_list;
	dll_node_t *node = temp->head;
	while (node != NULL) {
		block_t *block = node->data;

		if (address == block->start_address)
			return 1;

		node = node->next;
	}
	return 0;
}

int verify_address_final(arena_t *arena, const uint64_t address)
{
	// de facut
}

uint64_t position_identifier(list_t *list_blocks, const uint64_t address)
{
    uint64_t position = 0;
    dll_node_t *node = list_blocks->head;

    while (node!= NULL) {
        block_t *block = node->data;
		if(address <= block->start_address) 
			return position;

		node = node->next;
		++position;
    }
	return position;
}


dll_node_t*
get_node(list_t *list, const uint64_t address)
{
	dll_node_t *node = list->head;
	while (node != NULL) {
		block_t *block = node->data;
		
		if (address == block->start_address)
			return node;

		node = node->next;
	}

	return NULL;
}

dll_node_t*
get_node_by_poz(list_t *list, int n)
{
	dll_node_t *node = list->head;

	int pos = 0;

	while (node != NULL) {
		if (n == pos)
			return node;
		
		node = node->next;
		++pos;
	}
	return NULL;
	
}

void alloc_block(arena_t* arena, const uint64_t address, const uint64_t size) 
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

	} else {
        if (verify_address(arena, address) == 0) {
			// alloc big blocky
			uint64_t position = position_identifier(arena->alloc_list, address);

			block_t *block = malloc(sizeof(block_t));

			block->start_address = address;
			block->size = size;
			
			list_t *list_miniblocks = malloc(sizeof(list_t));
			block->miniblock_list = list_miniblocks;

			dll_add_nth_node(arena->alloc_list, position, block);

			// alloc small blocky
			miniblock_t *miniblock = malloc(sizeof(miniblock_t));
			
			miniblock->start_address = address;
			miniblock->size = size;
			miniblock->perm = 6;

			dll_add_nth_node(list_miniblocks, 0, miniblock);
			
			// final address
			uint64_t second_address = address + size;
			// we use it to check to right of this blocky
			
			if (verify_address_final(arena, second_address) == 1) {
				// so we move the blocks from final address
				// of new block
				dll_node_t *node = get_node(arena->alloc_list, second_address);
				dll_node_t *delete = ((list_t*)((block_t*) node->data)->miniblock_list)->head;

				dll_node_t *current = ((list_t*)block->miniblock_list)->head;

				current->next = delete;
				delete->prev = current;

				((list_t*)block->miniblock_list)->size +=
					 ((list_t*)((block_t*) node->data)->miniblock_list)->size;

				((list_t*)((block_t*) node->data)->miniblock_list)->head = NULL;

				dll_node_t *new = get_node_by_poz(arena->alloc_list, position);
				
				// add to current size
				((block_t*)new->data)->size += ((block_t*)node->data)->size;

				dll_node_t *remove = dll_remove_nth_node(arena->alloc_list, 1 + position);
				// sa dau free sau nu????????????????????????????????????????????????????????????????????
			}
        } else {
			uint64_t position = position_identifier(arena, address) - 1;
			dll_node_t *node = get_node_by_poz(arena->alloc_list, position);

			block_t *block = node->data;
			list_t *list_miniblocks = block->miniblock_list;

			miniblock_t *miniblock = malloc(sizeof(miniblock_t));
			miniblock->start_address = address;
			miniblock->size = size;
			miniblock->perm = 6;

			dll_add_nth_node(list_miniblocks, list_miniblocks->size, miniblock);
			block->size += size;

			uint64_t address2 = address + size;

			if (verify_address_final(arena, address2) == 1) {
				dll_node_t *second_node = get_node(arena->alloc_list, address2);
				dll_node_t *delete = ((list_t*)((block_t*)second_node->data)->miniblock_list)->head;

				dll_node_t *current = list_miniblocks->head;
				while (current->next != NULL)
					current = current->next;

				current->next = delete;
				delete->prev = current;
				list_miniblocks += ((list_t*)((block_t*)second_node->data)->miniblock_list)->size;

				((list_t*)((block_t*)second_node->data)->miniblock_list)->head = NULL;

				((block_t*)node->data)->size += ((block_t*)second_node->data)->size;

				dll_node_t *remove = dll_remove_nth_node(arena->alloc_list, 1 + position);
				// sa dau free?????????????????????????????????????????????????????????
			}
		}
    }
}
