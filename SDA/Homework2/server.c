/* Copyright 2023 <> */
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "utils.h"
#include "server.h"
#include "load_balancer.h"
#define HMAX 100000
struct server_memory {
	unsigned int server_id;
	unsigned int replica_number;
	unsigned int hash;
	hashtable_t *ht;
};

server_memory *init_server_memory()
{
	server_memory *server = malloc(sizeof(server_memory));
	server->ht = ht_create(64, hash_function_key, compare_function_key, key_val_free_function);
	return server;
}

void server_store(server_memory *server, char *key, char *value) {
	(void)server;
	(void)key;
	(void)value;
}

char *server_retrieve(server_memory *server, char *key) {
	/* TODO 3 */
	return NULL;
}

void server_remove(server_memory *server, char *key) {
	/* TODO 4 */
}

void free_server_memory(server_memory *server) {
	/* TODO 5 */
}
