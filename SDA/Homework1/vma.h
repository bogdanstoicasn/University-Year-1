#pragma once
#include <inttypes.h>
#include <stddef.h>

typedef struct dll_node_t {
	void *data;
	struct dll_node_t *prev, *next;
} dll_node_t;

typedef struct {
	dll_node_t *head;
	unsigned int data_size;
	unsigned int size;
} list_t;

typedef struct {
	uint64_t start_address;
	size_t size;
	void *miniblock_list;
} block_t;

typedef struct {
	uint64_t start_address;
	size_t size;
	uint8_t perm;
	void *rw_buffer;
} miniblock_t;

typedef struct {
	uint64_t arena_size;
	list_t *alloc_list;
} arena_t;

arena_t *alloc_arena(const uint64_t size);
void dealloc_arena(arena_t *arena);

void
alloc_block(arena_t *arena, const uint64_t address, const uint64_t size);

void
free_block(arena_t *arena, const uint64_t address);

void
read(arena_t *arena, uint64_t address, uint64_t size);

void
write(arena_t *arena, const uint64_t address,  const uint64_t size,
	  int8_t *data);

void
pmap(const arena_t *arena);

void
mprotect(arena_t *arena, uint64_t address, int8_t *permission);

// my functions

uint64_t
position_identifier(list_t *list_blocks, const uint64_t address);

int
free_block_verifier(arena_t *arena, const uint64_t address);

int
alloc_block_perrror(arena_t *arena, const uint64_t address,
					const uint64_t size);

int
address_free_perror(arena_t *arena, const uint64_t address);

int
address_read_perror(arena_t *arena, const uint64_t address);

int
address_write_perror(arena_t *arena, const uint64_t address);

// dll_node functions

list_t*
dll_create(unsigned int data_size);

void
dll_add_nth_node(list_t *list, unsigned int n, const void *data);

dll_node_t*
dll_remove_nth_node(list_t *list, unsigned int n);

void
dll_free(list_t **pp_list);

dll_node_t*
get_node(list_t *list, const uint64_t address);

dll_node_t*
get_node_by_poz(list_t *list, int n);
