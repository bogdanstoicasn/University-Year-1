

#define NMAX 64

int interface_handler(int *address, int *size, char *pointer);

int arena_interface(char copy_string[NMAX], int *size);

int pmap_interface(char copy_string[NMAX]);

int dealloc_arena_interface(char copy_string[NMAX]);

int alloc_block_interface(char copy_string[NMAX], int *address, int *size);

int free_block_interface(char copy_string[NMAX], int *address);

int read_interface(char copy_string[NMAX], int *address, int *size);