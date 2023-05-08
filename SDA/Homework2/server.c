/* Copyright 2023 <315CA_StoicaMihaiBogdan> */
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "utils.h"
#include "server.h"
#include "load_balancer.h"

#define HMAX 100000
#define OOP 64

struct server_memory {
	unsigned int server_id;
	unsigned int replica_number;
	unsigned int hash;
	hashtable_t *ht;
};

// initialize server
server_memory *init_server_memory()
{
	server_memory *server = malloc(sizeof(server_memory));
	server->ht = ht_create(OOP, hash_function_key, compare_function_key, key_val_free_function);
	return server;
}

void server_store(server_memory *server, char *key, char *value) {
	(void)server;
	(void)key;
	(void)value;
}

char *server_retrieve(server_memory *server, char *key) 
{
	(void)server;
	(void)key;	
	return NULL;
}

void server_remove(server_memory *server, char *key)
{
	(void)server;
	(void)key;
}

// free server memory
void free_server_memory(server_memory *server)
{
	hashtable_t *ht = server->ht;
	for (int i = 0; i < OOP; i++) {
		linked_list_t *list = ht->buckets[i];
		// free list, node, info
		ll_node_t *node = list->head;
		while (node) {
			info *information = node->data;
			free(information->key);
			free(information->value);
			free(information);

			ll_node_t *aux = node;
			node = node->next;
			free(aux);
		}
		free(list);
	}
	free(ht->buckets);
	free(ht);
	free(server);
}
