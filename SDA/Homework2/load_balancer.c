/* Copyright 2023 <> */
#include <stdlib.h>
#include <string.h>

#include "server.h"
#include "load_balancer.h"
#include "hash.h"
#include "utils.h"

#define HMAX 100000
struct load_balancer {
	void **server;
	unsigned int size;
};
struct server_memory {
	unsigned int server_id;
	unsigned int replica_number;
	unsigned int hash;
	hashtable_t *ht;
};

unsigned int hash_function_servers(void *a) {
    unsigned int uint_a = *((unsigned int *)a);

    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = (uint_a >> 16u) ^ uint_a;
    return uint_a;
}

unsigned int hash_function_key(void *a) {
    unsigned char *puchar_a = (unsigned char *)a;
    unsigned int hash = 5381;
    int c;

    while ((c = *puchar_a++))
        hash = ((hash << 5u) + hash) + c;

    return hash;
}

load_balancer *init_load_balancer() 
{
	load_balancer *main = malloc(sizeof(load_balancer));
	
	main->server = calloc(1, sizeof(void*));
	DIE(main->server == NULL, "calloc");
	main->size = 0;

	return main;
}

void loader_add_server(load_balancer *main, int server_id) 
{
    if (main->size == HMAX + 1)
		return;
	
	unsigned int hash1 = hash_function_servers(&server_id);
	
	unsigned int hash2 = server_id + 100000;
	hash2 = hash_function_servers(&hash2);
	
	unsigned int hash3 = server_id + 200000;
	hash3 = hash_function_servers(&hash3);

	main->size += 3;
	main->server = realloc(main->server, main->size * sizeof(void*));

	main->server[main->size - 3] = init_server_memory();
	main->server[main->size - 2] = init_server_memory();
	main->server[main->size - 1] = init_server_memory();
	
	server_memory *server1 = main->server[main->size - 3];
	server_memory *server2 = main->server[main->size - 2];
	server_memory *server3 = main->server[main->size - 1];

	server1->server_id = server_id;
	server2->server_id = server_id;
	server3->server_id = server_id;

	server1->replica_number = 0;
	server2->replica_number = 1;
	server3->replica_number = 2;

	server1->hash = hash1;
	server2->hash = hash2;
	server3->hash = hash3;

	for (unsigned int i = 0; i < main->size - 1; i++) {
		for (unsigned int j = i + 1; j < main->size; j++) {
			server_memory *server_tmp1 = main->server[i];
			server_memory *server_tmp2 = main->server[j];
			if (server_tmp1->hash > server_tmp2->hash) {
				void *aux = main->server[i];
				main->server[i] = main->server[j];
				main->server[j] = aux;
			}
		}
	}
	// sa fac remaparea cand se adauga server nou
	// unsigned int index1, index2, index3;
	// for (unsigned int i = 0; i < main->size; i++) {
	// 	server_memory *servus = main->server[i];
	// 	if (servus->server_id == server_id && servus->replica_number == 0)
	// 		index1 = i;
	// 	if (servus->server_id == server_id && servus->replica_number == 1)
	// 		index2 = i;
	// 	if (servus->server_id == server_id && servus->replica_number == 2)
	// 		index3 = i;
	// }
	// if (main->size == 3)
	// 	return;
	// index1 = checker_for_index(main, index1);
	// index2 = checker_for_index(main, index2);
	// index3 = checker_for_index(main, index3);

	// server1 = main->server[index1];
	// server2 = main->server[index2];
	// server3 = main->server[index3];
	
	// hashtable_t *ht = server1->ht;
	// for (int i = 0; i < 64; ++i) {
	// 	linked_list_t *list = ht->buckets[i];
	// 	ll_node_t *node = list->head;
	// 	while (node) {
	// 		info *information = node->data;
	// 		unsigned int hash = hash_function_key(information->key);
	// 		unsigned int index = hash_determine_cyclic(main, hash);

	// 	}
	// }
}

void loader_remove_server(load_balancer *main, int server_id) 
{
    /* TODO 3 */
}

void loader_store(load_balancer *main, char *key, char *value, int *server_id) 
{
	unsigned int hash = hash_function_key(key);
	unsigned int index = hash_determine_cyclic(main, hash);
	server_memory *server = main->server[index];
	*server_id = server->server_id;
	// printf("hash == %u\n", hash);
	unsigned int key_len = strlen(key) / sizeof(char);
	unsigned int value_len = strlen(value) / sizeof(char);

	ht_put(server->ht, key, key_len, value, value_len); //si gata da nu merge
}

char *loader_retrieve(load_balancer *main, char *key, int *server_id) 
{
	//return NULL;
	unsigned int hash = hash_function_key(key);
	unsigned int index = hash_determine_cyclic(main, hash);
	server_memory *sv = main->server[index];
	*server_id = sv->server_id;
	hashtable_t *ht = sv->ht;
	int ok = ht_has_key(ht, key);
	if (ok == 0) 
		return NULL;

	return ht_has_key_pointer(ht, key);
}

void free_load_balancer(load_balancer *main) 
{
	// for (unsigned int i = 0; i < main->size; i++) {
	// 	server_memory *server_tmp = main->server[i];
	// 	printf("id==%u replica==%u hash==%u\n", server_tmp->server_id, server_tmp->replica_number, server_tmp->hash);
	// }
	for (unsigned int i = 0; i < main->size; i++) {
		server_memory *server = main->server[i];
		hashtable_t *ht = server->ht;
		for (int i = 0; i < 64; i++) {
			linked_list_t *list = ht->buckets[i];
			// dam free la lista, node si data, data->key, data->value
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
	free(main->server);
	free(main);
}

unsigned int hash_determine_cyclic(load_balancer *main, unsigned int hash)
{
	server_memory *sv = main->server[main->size - 1];
	if (sv->hash < hash) {
		server_memory *first_sv = main->server[0];
		return 0;
	}
	for (unsigned int i = 0; i < main->size; i++) {
		server_memory *server = main->server[i];
		if (hash <= server->hash) {
			return i;
		}
	}
	return 0;
}
unsigned int checker_for_index(load_balancer *main, unsigned int index)
{
	if (index == main->size - 1)
		return 0;
	return index + 1;
}