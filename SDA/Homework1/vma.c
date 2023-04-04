// Copyright 2023-2024 Stoica Mihai-Bogdan (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vma.h"

// Allocates arena :))
arena_t*
alloc_arena(const uint64_t size)
{
	arena_t *arena = malloc(sizeof(arena_t));
	arena->arena_size = size;
	arena->alloc_list = dll_create(sizeof(block_t));

	return arena;
}

// Deallocates arena
void
dealloc_arena(arena_t *arena)
{
	list_t *first_head = arena->alloc_list;

	dll_node_t *node = first_head->head;

	while (node) {
		// Current block
		block_t *block = node->data;
		list_t *second_list = block->miniblock_list;
		dll_node_t *current = second_list->head;
		// Free for rw_buffer
		while (current) {
			miniblock_t *mini = current->data;
			if (mini->rw_buffer)
				free(mini->rw_buffer);

			current = current->next;
		}
		// Free data from block
		dll_free(&second_list);
		node = node->next;
	}
	dll_free(&arena->alloc_list);
	free(arena);
}

// Checks if address is equal to end address of block
int
verify_address_final(arena_t *arena, const uint64_t address)
{
	list_t *list_blocks = arena->alloc_list;
	dll_node_t *node = list_blocks->head;
	while (node) {
		block_t *block = node->data;

		uint64_t add = block->start_address;
		uint64_t size = block->size;

		if (add + size == address)
			return 1;

		node = node->next;
	}
	return 0;
}

// Checks if address is equal to beginning address of the block
int
verify_address_beginning(arena_t *arena, const uint64_t address)
{
	list_t *temp = arena->alloc_list;
	dll_node_t *node = temp->head;
	while (node) {
		block_t *block = node->data;

		if (address == block->start_address)
			return 1;

		node = node->next;
	}
	return 0;
}

// Identifies position by address(may or may not work as intended)
uint64_t
position_identifier(list_t *list_blocks, const uint64_t address)
{
	uint64_t position = 0;
	dll_node_t *node = list_blocks->head;

	while (node) {
		block_t *block = node->data;

		if (address <= block->start_address)
			return position--;

		node = node->next;
		++position;// ++position
	}
	return position;
}

// Useless function to reduce lines for "alloc_block"
void
assign_for_alloc(miniblock_t *miniblock, block_t *block,
				 const uint64_t address, const uint64_t size)
{
	block->size = size;
	block->start_address = address;

	miniblock->size =  size;
	miniblock->start_address = address;
	miniblock->perm = 6;
}

// Get address of a node/block by the position in list
dll_node_t*
get_node_by_poz(list_t *list, int n)
{
	dll_node_t *node = list->head;

	int pos = 0;
	while (node) {
		if (n == pos)
			return node;

		node = node->next;
		++pos;
	}
	return NULL;
}

// Alloc the block
// First we use the heap and alloc minblock, block, and the list of blocks
// We have 5 cases: empty list, overlap at the beginning/final
// with another/both block, different locations
void
alloc_block(arena_t *arena, const uint64_t address, const uint64_t size)
{
	if (address + size > arena->arena_size || !arena)
		return;

	list_t *list_blocks = arena->alloc_list;
	block_t *block = malloc(sizeof(block_t));
	list_t *list_miniblocks;
	list_miniblocks = dll_create(sizeof(miniblock_t));
	block->miniblock_list = list_miniblocks;
	miniblock_t *miniblock = malloc(sizeof(miniblock_t));
	miniblock->rw_buffer = NULL;
	list_miniblocks->data_size = sizeof(miniblock_t);
	assign_for_alloc(miniblock, block, address, size);
	if (!list_blocks->head) {
		dll_add_nth_node(list_blocks, 0, block);
		dll_add_nth_node(list_miniblocks, 0, miniblock);
		free(block);
		free(miniblock);
		return;
	}
	uint64_t address2 = address + size;
	int overlap_begin = verify_address_beginning(arena, address2);
	int overlap_final = verify_address_final(arena, address);
	// When block is between 2 blocks
	// We unite them and free last block completely(don't free mini)
	if (overlap_begin == 1 && overlap_final == 1) {
		uint64_t position1 = position_identifier(list_blocks, address);
		uint64_t position2 = position_identifier(list_blocks, address2);

		dll_node_t *node = get_node_by_poz(list_blocks, position1 - 1);
		dll_node_t *next_node = get_node_by_poz(list_blocks, position2);
		// We set the miniblock to beginning of next block
		block_t *next_block = next_node->data;
		list_t *next_mini_list = next_block->miniblock_list;
		dll_add_nth_node(next_mini_list, 0, miniblock);
		next_block->size += size;
		next_block->start_address = address;
		// We then combine the previous miniblock list with the next one
		block_t *present = node->data;
		list_t *mini_list = present->miniblock_list;
		dll_node_t *last = get_node_by_poz(mini_list, mini_list->size - 1);
		last->next = next_mini_list->head;
		next_mini_list->head->prev = last;

		mini_list->size += next_mini_list->size;
		present->size += next_block->size;

		free(next_mini_list); free(next_block);
		free(list_miniblocks); free(miniblock);
		free(block);
		dll_remove_nth_node(list_blocks, position2); free(next_node);
		return;
	}
	if (overlap_begin == 1) {
		uint64_t position = position_identifier(list_blocks, address2);
		dll_node_t *node = get_node_by_poz(list_blocks, position);
		block_t *present = node->data;
		list_t *mini_list = present->miniblock_list;
		dll_add_nth_node(mini_list, 0, miniblock);
		present->size += size;
		present->start_address = address;
		free(list_miniblocks); free(block); free(miniblock);
		return;
	}
	if (overlap_final == 1) {
		uint64_t position = position_identifier(list_blocks, address);
		if (position <= 0)
			position = 1;
		dll_node_t *node = get_node_by_poz(list_blocks, position - 1);
		block_t *present = node->data;
		list_t *mini_list = present->miniblock_list;
		dll_add_nth_node(mini_list, mini_list->size, miniblock);
		present->size += size;
		free(list_miniblocks); free(block); free(miniblock);
		return;
	}
	uint64_t position = position_identifier(list_blocks, address);
	dll_add_nth_node(list_blocks, position, block);
	dll_add_nth_node(list_miniblocks, 0, miniblock);
	free(block); free(miniblock);
}

// Checks whether we can free the miniblock
int
free_block_verifier(arena_t *arena, const uint64_t address)
{
	dll_node_t *node = arena->alloc_list->head;

	// Enter each block
	while (node) {
		block_t *block = node->data;

		if (block->start_address == address)
			return 0;

		node = node->next;
	}

	// Check address of miniblock
	node = arena->alloc_list->head;
	while (node) {
		block_t *block = node->data;
		list_t *lis_mini = block->miniblock_list;
		dll_node_t *curr = lis_mini->head;
		uint64_t sum = 0;
		while (curr) {
			miniblock_t *mini = curr->data;
			if (address == mini->start_address)
				sum = mini->size + mini->start_address;
			curr = curr->next;
		}
		if (sum == block->size + block->start_address)
			return 2;
		node = node->next;
	}

	return 1;
}

// Function that frees the block
// We have 3 cases: middle(1), final(2), beginning(0) position in block
void
free_block(arena_t *arena, const uint64_t address)
{
	int position = free_block_verifier(arena, address);
	list_t *list_blocks = arena->alloc_list;
	uint64_t position1 = position_identifier(list_blocks, address);
	if (position == 0) {// beginning of block
		dll_node_t *node = get_node_by_poz(list_blocks, position1);
		block_t *present = node->data;
		list_t *mini_list = present->miniblock_list;
		dll_node_t *delete = dll_remove_nth_node(mini_list, 0);
		if (!mini_list->head) {
			dll_node_t *remover = dll_remove_nth_node(arena->alloc_list,
													  position1);
			free(mini_list);
			free(remover); free(present);
			free(delete->data); free(delete);
		return;
		}
		miniblock_t *ofo = delete->data;
		present->start_address = ofo->start_address + ofo->size;
		present->size -= ofo->size;
		free(delete->data); free(delete);
		return;
	}
	if (position == 1) { // middle of block
		dll_node_t *node = get_node_by_poz(list_blocks, position1 - 1);
		block_t *block = node->data;
		list_t *list_mini = block->miniblock_list;
		dll_node_t *curr = list_mini->head;
		// get position in mini
		while (curr) {
			miniblock_t *miniblock = curr->data;
			if (miniblock->start_address == address)
				break;
			curr = curr->next;
		}
		block->size = address - block->start_address;
		dll_node_t *previous = curr->prev;
		previous->next = NULL;
		dll_node_t *next = curr->next;
		dll_node_t *second_next = next->next;
		dll_node_t *travel = next;
		uint64_t ok = 1, dimension = ((miniblock_t *)(next->data))->size;
		while (travel->next) {
			++ok;
			dimension += ((miniblock_t *)(travel->data))->size;
			travel = travel->next;
		}

		list_mini->size = list_mini->size - ok - 1;
		// Create the new nodes
		alloc_block(arena, ((miniblock_t *)(next->data))->start_address,
					((miniblock_t *)(next->data))->size);
		dll_node_t *ofo = get_node_by_poz(list_blocks, position1);
		block_t *new_block = ofo->data;
		list_t *new_list = new_block->miniblock_list;
		dll_node_t *new_node = new_list->head;
		if (ok == 1)
			new_list->size = 1;
		else
			new_list->size += ok - 1;
		free(next->data); free(next);
		free(curr->data); free(curr);
		new_node->next = second_next;
		if (!second_next)
			return;

		second_next->prev = new_node;
		new_block->size = dimension;
		return;
	}
	if (position == 2) {// final of block
		dll_node_t *node = get_node_by_poz(list_blocks, position1 - 1);
		block_t *block = node->data;
		list_t *list_mini = block->miniblock_list;
		dll_node_t *delete = dll_remove_nth_node(list_mini,
												 list_mini->size - 1);
		block->size -= ((miniblock_t *)delete->data)->size;
		free(delete->data); free(delete);
		return;
	}
}

// Write function
// After 1 day I realised i needed to check if exist a next miniblock :()
void
write(arena_t *arena, const uint64_t address,  const uint64_t size,
	  int8_t *data)
{	// *We get the block where we need to write
	dll_node_t *node = arena->alloc_list->head;
	uint64_t poz = 0;
	while (node->next) {
		block_t *block = node->data;
		uint64_t upper = block->size + block->start_address;
		uint64_t lower = block->start_address;
		if (address < upper && address >= lower)
			break;
		poz++;
		node = node->next;
	}
	uint64_t fin = address + size;
	block_t *block = node->data;
	uint64_t bll_size = block->size, bll_address = block->start_address;
	list_t *list_mini = block->miniblock_list;
	dll_node_t *miniblock = list_mini->head;
	miniblock_t *mini = miniblock->data;
	uint64_t j = 0;
	// position aquired*
	// Error case
	if (block->size + block->start_address < size + address) {
		uint64_t dim = block->size +
						block->start_address
						- address;
		printf("Warning: size was bigger than the block size. ");
		printf("Writing %lu characters.\n", dim);
	}
	// Size + address >= fin, that means we make fin end address of block
	if (bll_address + bll_size <= fin)
		fin = bll_address + bll_size;

	(void)0;// Useless (indentation error so we put here;0)

	for (uint64_t i = address; i < fin; i++) {
		if (i >= mini->size + mini->start_address) {
			// Move to next miniblock
			miniblock = miniblock->next;
			if (!miniblock)
				return;
			mini = miniblock->data;
		}

		// Allocate new buffer if necessary
		if (!mini->rw_buffer)
			mini->rw_buffer = calloc(mini->size, sizeof(int8_t));

		// Write to buffer if miniblock contains memory region
		((int8_t *)mini->rw_buffer)[i - mini->start_address] = data[j++];
	}
}

// Read function
void
read(arena_t *arena, uint64_t address, uint64_t size)
{
	dll_node_t *node = arena->alloc_list->head;
	uint64_t poz = 0;
	while (node->next) {
		block_t *block = node->data;
		uint64_t upper = block->size + block->start_address;
		uint64_t lower = block->start_address;
		if (address < upper && address >= lower)
			break;
		poz++;
		node = node->next;
	}

	uint64_t add = address;
	uint64_t fin = address + size;
	block_t *block = node->data;
	list_t *list_mini = block->miniblock_list;
	dll_node_t *miniblock = list_mini->head;

	while (miniblock) {
		miniblock_t *mini = miniblock->data;
		uint64_t upper = mini->size + mini->start_address;
		uint64_t lower = mini->start_address;
		if (address < upper && address >= lower)
			break;

		miniblock = miniblock->next;
	}
	// We got the miniblock from which we beginn to read
	if (block->size +
		block->start_address <
		size + address) {
		uint64_t dim = block->size +
					   block->start_address -
						address;
		printf("Warning: size was bigger than the block size. ");
		printf("Reading %lu characters.\n", dim);
	}

	if (block->size + block->start_address <= fin)
		fin = block->size + block->start_address;
	// Start of reading
	miniblock_t *mini = miniblock->data;
	for (uint64_t i = add; i < fin; i++) {
		if (i >= mini->size + mini->start_address) {
			// Move to next miniblock
			miniblock = miniblock->next;
			mini = miniblock->data;
		}
		// Print if it's not NULL
		if (((int8_t *)mini->rw_buffer)[i - mini->start_address] != 0)
			printf("%c", ((int8_t *)mini->rw_buffer)[i - mini->start_address]);
	}
	printf("\n");
}

// Output to PMAP
// No explanations needed
void
pmap(const arena_t *arena)
{
	printf("Total memory: 0x%lX bytes\n", arena->arena_size);

	dll_node_t *node_free = arena->alloc_list->head;
	uint64_t dim = 0;
	while (node_free) {
		block_t *block = node_free->data;
		dim += block->size;
		node_free = node_free->next;
	}

	dim = arena->arena_size - dim;
	printf("Free memory: 0x%lX bytes\n", dim);

	printf("Number of allocated blocks: %u\n", arena->alloc_list->size);

	dll_node_t *current = arena->alloc_list->head;
	uint64_t nr_mini = 0;
	while (current) {
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
		printf("Zone: 0x%lX - 0x%lX\n", block->start_address,
			   block->start_address + block->size);

		list_t *mini_list = block->miniblock_list;
		dll_node_t *node = mini_list->head;
		for (uint64_t j = 1; j <= mini_list->size; ++j) {
			miniblock_t *mini = node->data;
			printf("Miniblock %ld:", j);
			printf("\t\t0x%lX\t\t-\t\t0x%lX\t\t|", mini->start_address,
				   mini->start_address + mini->size);
			printf(" RW-\n");
			node = node->next;
		}

		current = current->next;

		printf("Block %ld end\n", i);
		if (i < n)
			printf("\n");
	}
}

// FROM HERE ARE FUNCTIONS MADE FOR ERROR CASES IN ADDRESS, SIZE
// Checks if the size, address are correct(are valid)
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
	while (current) {
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

// Checks if address is start address of miniblock else error
int
address_free_perror(arena_t *arena, const uint64_t address)
{
	if (address >= arena->arena_size) {
		printf("Invalid address for free.\n");
		return 0;
	}

	dll_node_t *current = arena->alloc_list->head;
	while (current) {
		block_t *block = current->data;
		list_t *list_mini = block->miniblock_list;
		dll_node_t *node_mini = list_mini->head;

		while (node_mini) {
			miniblock_t *miniblock = node_mini->data;
			if (miniblock->start_address == address)
				return 1;
			node_mini = node_mini->next;
		}
		current = current->next;
	}

	printf("Invalid address for free.\n");
	return 0;
}

// Checks if address exists(between start and end address)
int
address_read_perror(arena_t *arena, const uint64_t address)
{
	if (arena->arena_size <= address) {
		printf("Invalid address for read.\n");
		return 0;
	}

	dll_node_t *current = arena->alloc_list->head;
	while (current) {
		block_t *block = current->data;
		uint64_t lower_limit = block->start_address;
		uint64_t upper_limit = block->start_address + block->size;

		if (address >= lower_limit && address < upper_limit)
			return 1;

		current = current->next;
	}

	printf("Invalid address for read.\n");
	return 0;
}

// Same as read
int
address_write_perror(arena_t *arena, const uint64_t address)
{
	if (arena->arena_size <= address) {
		printf("Invalid address for write.\n");
		return 0;
	}

	dll_node_t *current = arena->alloc_list->head;
	while (current) {
		block_t *block = current->data;
		uint64_t lower_limit = block->start_address;
		uint64_t upper_limit = block->start_address + block->size;

		if (address >= lower_limit && address < upper_limit)
			return 1;

		current = current->next;
	}

	printf("Invalid address for write.\n");
	return 0;
}
