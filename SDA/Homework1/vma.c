#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vma.h"

arena_t 
*alloc_arena(const uint64_t size)
{
	arena_t *arena = malloc(sizeof(arena_t));
	arena->arena_size = size;
	arena->alloc_list = dll_create(sizeof(block_t));
	
	return arena;
}

void 
dealloc_arena(arena_t* arena)
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

int 
verify_address_final(arena_t *arena, const uint64_t address) // final address of node with our address
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

int 
verify_address_beginning(arena_t *arena, const uint64_t address) // start adress of node with our address
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


uint64_t 
position_identifier(list_t *list_blocks, const uint64_t address)
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

void 
alloc_block(arena_t* arena, const uint64_t address, const uint64_t size) 
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
	// IES 0 CAND ADRESA 2 este == adresa 1
	if (overlap_begin == 1 && overlap_final == 1) {
		uint64_t position1 = position_identifier(list_blocks, address);
		uint64_t position2 = position_identifier(list_blocks, address2);
		
		dll_node_t *node = get_node_by_poz(list_blocks, position1 - 1);
		dll_node_t *next_node = get_node_by_poz(list_blocks, position2);

		// we set the miniblock to beginning of next block
		block_t *next_block = next_node->data;
		list_t *next_mini_list = next_block->miniblock_list;
		dll_add_nth_node(next_mini_list, 0, miniblock);
		next_block->size += size;
		next_block->start_address = address;

		// we then combine the previous miniblock list with the next one
		block_t *present = node->data;
		list_t *mini_list = present->miniblock_list;
		dll_node_t *last = get_node_by_poz(mini_list, mini_list->size - 1);
		last->next = next_mini_list->head;
		next_mini_list->head->prev = last;

		mini_list->size +=next_mini_list->size;
		present->size += next_block->size;

		free(next_mini_list);
		free(next_block);
		free(list_miniblocks);
		free(miniblock);
		free(block);
		dll_remove_nth_node(list_blocks, position2);
		free(next_node);
		return;
	}

	if(overlap_begin == 1) {
		uint64_t position = position_identifier(list_blocks, address2);
		dll_node_t *node = get_node_by_poz(list_blocks, position); 
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
		if (position <= 0) position = 1;
		dll_node_t *node = get_node_by_poz(list_blocks, position - 1);
		block_t *present = node->data;
		list_t *mini_list = present->miniblock_list;
		dll_add_nth_node(mini_list, mini_list->size, miniblock);
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


int
free_block_verifier(arena_t* arena, const uint64_t address)
{
	dll_node_t *node = arena->alloc_list->head;

	while (node != NULL) {
		block_t *block = node->data;
		
		if (block->start_address == address)
			return 0;

		node = node->next;
	}

	node = arena->alloc_list->head;
	while (node != NULL) {
		block_t *block = node->data;
		list_t *lis_mini = block->miniblock_list;
		dll_node_t *curr = lis_mini->head;
		uint64_t sum = 0;
		while (curr != NULL) {
			miniblock_t *mini = curr->data;
			if (address == mini->start_address) {
				sum = mini->size + mini->start_address;
			}
			curr = curr->next;
		}
		if (sum == block->size + block->start_address)
			return 2;
		node = node->next;
	}

	return 1;
}


void
free_block(arena_t* arena, const uint64_t address)
{
	int position = free_block_verifier(arena, address);
	list_t *list_blocks = arena->alloc_list;
	uint64_t position1 = position_identifier(list_blocks, address);
	if (position == 0) {
		dll_node_t *node = get_node_by_poz(list_blocks, position1);
		block_t *present = node->data;
		list_t *mini_list = present->miniblock_list;
		dll_node_t *delete = dll_remove_nth_node(mini_list, 0);
		if (mini_list->head == NULL) {
			dll_node_t *remover = dll_remove_nth_node(arena->alloc_list, position1);
			free(mini_list);
			free(remover);
			free(present);
			free(delete->data);
		free(delete);
		return;
		}
		miniblock_t *ofo = delete->data;
		present->start_address = ofo->start_address + ofo->size;
		present->size -= ofo->size;
		free(delete->data);
		free(delete);
		return;
	}

	if (position == 1) {
		dll_node_t *node = get_node_by_poz(list_blocks, position1 - 1);
		block_t *block = node->data;
		list_t *list_mini = block->miniblock_list;
		dll_node_t *curr = list_mini->head;
		while (curr != NULL) {
			miniblock_t *miniblock = curr->data;
			if (miniblock->start_address == address)
				break;
			curr = curr->next;
		}
		list_mini->size--;
		block->size = ((miniblock_t*)curr->data)->start_address - block->start_address;
		dll_node_t *previous = curr->prev;
		dll_node_t *next = curr->next;

		return;
	}
	if (position == 2) {
		dll_node_t *node = get_node_by_poz(list_blocks, position1 - 1); //de vzt
		block_t *block = node->data;
		list_t *list_mini = block->miniblock_list;
		dll_node_t *delete = dll_remove_nth_node(list_mini, list_mini->size - 1);
		block->size -= ((miniblock_t*)delete->data)->size;
		free(delete->data);
		free(delete);
		return;
	}
	return;
}

void 
pmap(const arena_t *arena)
{
	printf("Total memory: 0x%lX bytes\n", arena->arena_size);

	dll_node_t *node_free = arena->alloc_list->head;
  	uint64_t dim = 0;
  	while (node_free != NULL) {
		block_t *block = node_free->data;
    	dim += block->size;
    	node_free = node_free->next;
  	}
	dim = arena->arena_size - dim;
	printf("Free memory: 0x%lX bytes\n", dim);

	printf("Number of allocated blocks: %u\n", arena->alloc_list->size);

	dll_node_t *current = arena->alloc_list->head;
	uint64_t nr_mini = 0;
	while (current != NULL) {
		block_t *block = current->data;
		list_t *list_mini = block->miniblock_list;
		nr_mini += list_mini->size;
		current = current->next;
	}
	printf("Number of allocated miniblocks: %ld\n", nr_mini);
	if (nr_mini != 0) 
		printf("\n");

	uint64_t n = arena->alloc_list->size;
	current = arena->alloc_list->head;
	for (uint64_t i = 1; i <= n; ++i) {
		printf("Block %ld begin\n", i);
		block_t *block = current->data;
		printf("Zone: 0x%lX - 0x%lX\n", block->start_address, block->start_address + block->size);
		
		list_t *mini_list = block->miniblock_list;
		dll_node_t *node = mini_list->head;
		for (uint64_t j = 1; j <= mini_list->size; ++j) {
			miniblock_t *mini = node->data;
			printf("Miniblock %ld:", j);
			printf("\t\t0x%lX\t\t-\t\t0x%lX\t\t|",mini->start_address, mini->start_address + mini->size);
			printf(" RW-\n");
			node = node->next;
		}

		current = current->next;

		printf("Block %ld end\n", i);
		if (i < n) 
			printf("\n");
	}
}

int 
alloc_block_perrror(arena_t *arena, const uint64_t address, const uint64_t size)
{
	if (address >= arena->arena_size) {
		printf("The allocated address is outside the size of arena\n");
		return 0;
	}
	if (size > arena->arena_size) {
		printf("The allocated address is outside the size of arena\n");
		return 0;
	}
	if (size + address > arena->arena_size) {
		printf("The end address is past the size of the arena\n");
		return 0;
	}

	dll_node_t *current = arena->alloc_list->head;
	while (current != NULL) {
		block_t *block = current->data;
		uint64_t sum = size + address;
		uint64_t lower_limit = block->start_address;
		uint64_t upper_limit = block->size + lower_limit;

		if (address > lower_limit && address < upper_limit) {
			printf("This zone was already allocated.\n");
			return 0;
		}

		if (sum > lower_limit && sum < upper_limit) {
			printf("This zone was already allocated.\n");
			return 0;
		}

		if (block->start_address < sum && block->start_address > address) {
			printf("This zone was already allocated.\n");
			return 0;
		}
		if (address == block->start_address && sum == upper_limit) {
			printf("This zone was already allocated.\n");
			return 0;
		}
		current = current->next;
	}
	return 1;
}

int 
address_free_perror(arena_t *arena, const uint64_t address)
{
	if (address >= arena->arena_size) {
		printf("Invalid address for free.\n");
		return 0;
	}


	dll_node_t *current = arena->alloc_list->head;
	while (current != NULL) {
		block_t *block = current->data;
		list_t *list_mini = block->miniblock_list;
		dll_node_t *node_mini = list_mini->head;

		while (node_mini != NULL) {
			miniblock_t *miniblock = node_mini->data;
			if (miniblock->start_address == address) {
				return 1;
			}
			node_mini = node_mini->next;
		}
		current = current->next;
	}

	printf("Invalid address for free.\n");
	return 0;
}

int 
address_read_perror(arena_t *arena, const uint64_t address)
{
	if (arena->arena_size <= address) {
		printf("Invalid address for read.\n");
		return 0;
	}


	dll_node_t *current = arena->alloc_list->head;
	while (current != NULL) {
		block_t *block = current->data;
		uint64_t lower_limit = block->start_address;
		uint64_t upper_limit = block->start_address + block->size;

		if (address >= lower_limit && address <= upper_limit) {
			return 1;
		}

		current = current->next;
	}

	printf("Invalid address for read.\n");
	return 0;
}

int
address_write_perror(arena_t *arena, const uint64_t address)
{
	if (arena->arena_size <= address) {
		printf("Invalid address for write.\n");
		return 0;
	}


	dll_node_t *current = arena->alloc_list->head;
	while (current != NULL) {
		block_t *block = current->data;
		uint64_t lower_limit = block->start_address;
		uint64_t upper_limit = block->start_address + block->size;

		if (address >= lower_limit && address <= upper_limit) {
			return 1;
		}

		current = current->next;
	}

	printf("Invalid address for write.\n");
	return 0;
}