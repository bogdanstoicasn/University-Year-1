
#include <inttypes.h>
#define NMAX 128


int interface_handler(uint64_t *address, uint64_t *size, int8_t **pointer);

int arena_interface(uint64_t *size);

int pmap_interface();

int dealloc_arena_interface();

int alloc_block_interface(uint64_t *address, uint64_t *size);

int free_block_interface(uint64_t *address);

int read_interface(uint64_t *address, uint64_t *size);

int
write_interface(uint64_t *address, uint64_t *size, int8_t **pointer);