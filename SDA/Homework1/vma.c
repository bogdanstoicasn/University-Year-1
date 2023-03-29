#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vma.h"

arena_t *alloc_arena(const uint64_t size)
{
	arena_t *arena = malloc(sizeof(arena_t));
	arena->arena_size = size;
	arena->alloc_list = dll_create(sizeof(block_t));
	
	list_t *list_blocks = arena->alloc_list;
	
	list_blocks->start_address = 0;
	
	return arena;
}

void dealloc_arena(arena_t* arena)
{
    list_t *first_head = arena->alloc_list;
	
	dll_node_t *node = first_head->head;

	while (node != NULL) {
		block_t *block = node->data;
		list_t *second_list = block->miniblock_list;

		dll_free(&second_list);
		node = node->next;
	}
	dll_free(&arena->alloc_list);
	free(arena);
}// TERMINAT

int verify_address_final(arena_t *arena, const uint64_t address) // final address of node with our address
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

int verify_address_beginning(arena_t *arena, const uint64_t address) // start adress of node with our address
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


uint64_t position_identifier(list_t *list_blocks, const uint64_t address)
{
    uint64_t position = 0;
    dll_node_t *node = list_blocks->head;

    while (node != NULL) {
        block_t *block = node->data;
		
		if(address <= block->start_address) 
			return position;

		node = node->next;
		++position;
    }
	return position;
}

void
assign_for_alloc(miniblock_t *miniblock, block_t *block, const uint64_t address, const uint64_t size)
{
	block->size = size;
	block->start_address = address;
	
	miniblock->size =  size;
	miniblock->start_address = address;
	miniblock->perm = 6;
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
	block_t *block = malloc(sizeof(block_t));

	list_t *list_miniblocks;
	list_miniblocks = dll_create(sizeof(miniblock_t));
	block->miniblock_list = list_miniblocks;

	miniblock_t *miniblock = malloc(sizeof(miniblock_t));

	list_miniblocks->data_size = sizeof(miniblock_t);
	list_miniblocks->start_address = address;

	assign_for_alloc(miniblock, block, address, size);

	if (list_blocks->head == NULL) {
		dll_add_nth_node(list_blocks, 0, block);
		dll_add_nth_node(list_miniblocks, 0, miniblock);

		free(block);
		free(miniblock);
		return;

	} 

	uint64_t address2 = address + size;
	int overlap_begin = verify_address_beginning(arena, address2);
	int overlap_final = verify_address_final(arena, address);
	
	if (overlap_begin == 1 && overlap_final == 1) {
		/// de facut
		return;
	}

	if(overlap_begin == 1) {
		uint64_t position = position_identifier(list_blocks, address2);
		dll_node_t *node = get_node_by_poz(list_blocks, position); // node de pe pozitia IN CARE SE PETRECE CE AVEM NOI NEVOIE
		block_t *present = node->data;
		list_t *mini_list = present->miniblock_list;
		dll_add_nth_node(mini_list, 0, miniblock);
		present->size += size;
		present->start_address = address;
		
		free(list_miniblocks);
		free(block);
		free(miniblock);
		return;
	}

	if (overlap_final == 1) {
		uint64_t position = position_identifier(list_blocks, address);
		dll_node_t *node = get_node_by_poz(list_blocks, position - 1);
		block_t *present = node->data;
		list_t *mini_list = present->miniblock_list;
		dll_add_nth_node(mini_list, position + 1, miniblock);
		present->size += size;
		free(list_miniblocks);
		free(block);
		free(miniblock);
		return;
	}

	uint64_t position = position_identifier(list_blocks, address);
	
	dll_add_nth_node(list_blocks, position, block);
	dll_add_nth_node(list_miniblocks, 0, miniblock);

	free(block);
	free(miniblock);
}

void pmap(const arena_t *arena)
{
	printf("Total memory: %ld bytes\n", arena->arena_size);
	printf("Free memory: y bytes\n");
	printf("Number of allocated blocks: %d\n", arena->alloc_list->size);

	dll_node_t *current = arena->alloc_list->head;
	int nr_mini = 0;
	while (current != NULL) {
		block_t *block = current->data;
		list_t *list_mini = block->miniblock_list;
		nr_mini += list_mini->size;
		current = current->next;
	}

	printf("Number of allocated miniblocks: %d\n",nr_mini);

	int n = arena->alloc_list->size;
	current = arena->alloc_list->head;
	for (int i = 1; i <= n; ++i) {
		printf("Block %d begin\n", i);
		block_t *block = current->data;
		printf("Zone: %ld - %ld\n", block->start_address, block->start_address + block->size);
		
		list_t *mini_list = block->miniblock_list;
		dll_node_t *node = mini_list->head;
		for (uint64_t j = 1; j <= mini_list->size; ++j) {
			miniblock_t *mini = node->data;
			printf("Miniblock %ld:", j);
			printf("\t\t%ld\t\t-\t\t%ld\t\t|",mini->start_address, mini->start_address + mini->size);
			printf(" RW-\n");
			node = node->next;
		}
		current = current->next;
		printf("Block %d end\n", i);
	}
}
