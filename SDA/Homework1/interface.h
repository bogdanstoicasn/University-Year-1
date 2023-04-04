// Copyright 2023-2024 Stoica Mihai-Bogdan (bogdanstoicasn@yahoo.com)
#include <inttypes.h>

int
interface_handler(uint64_t *address, uint64_t *size, int8_t **pointer);

int
arena_interface(uint64_t *size);

int
pmap_interface(void);

int
dealloc_arena_interface(void);

int
alloc_block_interface(uint64_t *address, uint64_t *size);

int
free_block_interface(uint64_t *address);

int
read_interface(uint64_t *address, uint64_t *size);

int
write_interface(uint64_t *address, uint64_t *size, int8_t **pointer);
