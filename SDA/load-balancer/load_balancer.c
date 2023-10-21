/* Copyright 2023 <315CA_StoicaMihaiBogdan> */
#include <stdlib.h>
#include <string.h>

#include "server.h"
#include "load_balancer.h"
#include "hash.h"
#include "utils.h"

#define HMAX 100000
#define OOP 64
// load balancer structure
struct load_balancer {
	void **server;
	unsigned int size;
};

// server structure
struct server_memory {
	int server_id;
	unsigned int replica_number;
	unsigned int hash;
	hashtable_t *ht;
};

// hash function for servers
unsigned int hash_function_servers(void *a)
{
    unsigned int uint_a = *((unsigned int *)a);

    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = (uint_a >> 16u) ^ uint_a;
    return uint_a;
}

// hash function for keys
unsigned int hash_function_key(void *a)
{
    unsigned char *puchar_a = (unsigned char *)a;
    unsigned int hash = 5381;
    int c;

    while ((c = *puchar_a++))
        hash = ((hash << 5u) + hash) + c;

    return hash;
}

// initialize load balancer
load_balancer *init_load_balancer() 
{
	load_balancer *main = malloc(sizeof(load_balancer));
	
	main->server = calloc(1, sizeof(void*));
	DIE(main->server == NULL, "calloc");
	main->size = 0;

	return main;
}

// here the magic takes place
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
	
	//alloc 3 servers
	main->server[main->size - 3] = init_server_memory();
	main->server[main->size - 2] = init_server_memory();
	main->server[main->size - 1] = init_server_memory();
	
	server_memory *server1 = main->server[main->size - 3];
	server_memory *server2 = main->server[main->size - 2];
	server_memory *server3 = main->server[main->size - 1];

	// put the data in servers
	server1->server_id = server_id;
	server2->server_id = server_id;
	server3->server_id = server_id;

	server1->replica_number = 0;
	server2->replica_number = 1;
	server3->replica_number = 2;

	server1->hash = hash1;
	server2->hash = hash2;
	server3->hash = hash3;

	// sort the servers
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

	unsigned int index1, index2, index3;
	// remap the products
	int ok = 0;
	for (unsigned int i = 0; i < main->size; i++) {
		server_memory *server = main->server[i];
		if (ok == 0 && server->server_id == server_id) {
			index1 = i;
			ok = 1;
			continue;
		}
		if (ok == 1 && server->server_id == server_id) {
			index2 = i;
			ok = 2;
			continue;
		}
		if (ok == 2 && server->server_id == server_id) {
			index3 = i;
			ok = 3;
			continue;
		}
	}

	if (main->size == 3)
		return;
	// get the servers where we need to remap the products
	index1 = checker_for_index(main, index1);
	index2 = checker_for_index(main, index2);
	index3 = checker_for_index(main, index3);

	server1 = main->server[index1];
	server2 = main->server[index2];
	server3 = main->server[index3];

	hashtable_t *ht1 = server1->ht;
	hashtable_t *ht2 = server2->ht;
	hashtable_t *ht3 = server3->ht;

	// we set the pointer to new ht
	server1->ht = ht_create(OOP, hash_function_key, compare_function_key, key_val_free_function);
	DIE(server1->ht == NULL, "ht_create");
	server2->ht = ht_create(OOP, hash_function_key, compare_function_key, key_val_free_function);
	DIE(server2->ht == NULL, "ht_create");
	server3->ht = ht_create(OOP, hash_function_key, compare_function_key, key_val_free_function);
	DIE(server3->ht == NULL, "ht_create");

	// we remap the products
	for (int i = 0; i < OOP; ++i) {
		linked_list_t *list = ht1->buckets[i];
		ll_node_t *node = list->head;
		while (node) {
			info *information = node->data;
			char *key = information->key;
			char *value = information->value;
			loader_store(main, key, value, &server_id);
			node = node->next;
		}
	}

	for (int i = 0; i < OOP; ++i) {
		linked_list_t *list = ht2->buckets[i];
		ll_node_t *node = list->head;
		while (node) {
			info *information = node->data;
			char *key = information->key;
			char *value = information->value;
			loader_store(main, key, value, &server_id);
			node = node->next;
		}
	}

	for (int i = 0; i < OOP; ++i) {
		linked_list_t *list = ht3->buckets[i];
		ll_node_t *node = list->head;
		while (node) {
			info *information = node->data;
			char *key = information->key;
			char *value = information->value;
			loader_store(main, key, value, &server_id);
			node = node->next;
		}
	}
	// free old hashtables
	ht_free(ht1);
	ht_free(ht2);
	ht_free(ht3);
}

void loader_remove_server(load_balancer *main, int server_id)
{	// determine the coresponding servers(indexes)
	int ok = 0;
	unsigned int index1, index2, index3;
	server_memory *server1, *server2, *server3;
	for (unsigned int i = 0; i < main->size; i++) {
		server_memory *server = main->server[i];
		if (ok == 0 && server->server_id == server_id) {
			server1 = server;
			index1 = i;
			ok = 1;
			continue;
		}
		if (ok == 1 && server->server_id == server_id) {
			server2 = server;
			index2 = i;
			ok = 2;
			continue;
		}
		if (ok == 2 && server->server_id == server_id) {
			server3 = server;
			index3 = i;
			ok = 3;
			continue;
		}
	}
	// remove the server from the load balancer
	for (unsigned int i = index1; i < main->size - 1; i++)
		main->server[i] = main->server[i + 1];
	main->size--;
	for (unsigned int i = index2 - 1 ; i < main->size - 1; i++)
		main->server[i] = main->server[i + 1];
	main->size--;
	for (unsigned int i = index3 - 2; i < main->size - 1; i++)
		main->server[i] = main->server[i + 1];
	main->size--;
	main->server = realloc(main->server, main->size * sizeof(server_memory *));
	DIE(main->server == NULL, "realloc");
	hashtable_t *ht1 = server1->ht;

	// we remap the products from deleted servers
	for (int i = 0; i < OOP; ++i) {
		linked_list_t *list = ht1->buckets[i];
		ll_node_t *node = list->head;
		while (node) {
			info *information = node->data;
			char *key = information->key;
			char *value = information->value;
			loader_store(main, key, value, &server_id);
			node = node->next;
		}
	}

	hashtable_t *ht2 = server2->ht;
	for (int i = 0; i < OOP; ++i) {
		linked_list_t *list = ht2->buckets[i];
		ll_node_t *node = list->head;
		while (node) {
			info *information = node->data;
			char *key = information->key;
			char *value = information->value;
			loader_store(main, key, value, &server_id);
			node = node->next;
		}
	}

	hashtable_t *ht3 = server3->ht;
	for (int i = 0; i < OOP; ++i) {
		linked_list_t *list = ht3->buckets[i];
		ll_node_t *node = list->head;
		while (node) {
			info *information = node->data;
			char *key = information->key;
			char *value = information->value;
			loader_store(main, key, value, &server_id);
			node = node->next;
		}
	}
	free_server_memory(server1);
	free_server_memory(server2);
	free_server_memory(server3);
}

// store product in the coresponding server
void loader_store(load_balancer *main, char *key, char *value, int *server_id)
{
	unsigned int hash = hash_function_key(key);
	unsigned int index = hash_determine_cyclic(main, hash);
	server_memory *server = main->server[index];
	*server_id = server->server_id;

	unsigned int key_len = strlen(key) / sizeof(char);
	unsigned int value_len = strlen(value) / sizeof(char);

	ht_put(server->ht, key, key_len, value, value_len);
}

// function to check if a product is in the load balancer
char *loader_retrieve(load_balancer *main, char *key, int *server_id)
{

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
	for (unsigned int i = 0; i < main->size; i++) {
		server_memory *server = main->server[i];
		hashtable_t *ht = server->ht;
		for (int i = 0; i <OOP; i++) {
			linked_list_t *list = ht->buckets[i];
			// free list, node and information
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

// simulate a cyclic vector
unsigned int hash_determine_cyclic(load_balancer *main, unsigned int hash)
{
	server_memory *sv = main->server[main->size - 1];
	if (sv->hash < hash)
		return 0;
	for (unsigned int i = 0; i < main->size; i++) {
		server_memory *server = main->server[i];
		if (hash <= server->hash) {
			return i;
		}
	}
	return 0;
}

// function that returns the next index
unsigned int checker_for_index(load_balancer *main, unsigned int index)
{
	if (index == main->size - 1)
		return 0;
	return index + 1;
}