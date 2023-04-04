// Copyright 2023-2024 Stoica Mihai-Bogdan (bogdanstoicasn@yahoo.com)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vma.h"

// Create list
list_t*
dll_create(unsigned int data_size)
{
	list_t *new_node = malloc(sizeof(list_t));

	new_node->head = NULL;

	new_node->data_size = data_size;

	new_node->size = 0;

	return new_node;
}

// Get node by address
dll_node_t*
get_node(list_t *list, const uint64_t address)
{
	dll_node_t *node = list->head;
	while (node) {
		block_t *block = node->data;

		if (address == block->start_address)
			return node;

		node = node->next;
	}

	return NULL;
}

// Adds a node in a doubly linked list(non cyclic)
void
dll_add_nth_node(list_t *list, unsigned int n, const void *data)
{
	dll_node_t *new_node = malloc(sizeof(dll_node_t));

	new_node->data = malloc(list->data_size);
	memcpy(new_node->data, data, list->data_size);

	new_node->prev = NULL;
	new_node->next = NULL;

	dll_node_t *current = list->head;

	if (n > list->size)
		n = list->size;

	if (n == 0) {
		if (list->size != 0) {
			new_node->next = list->head;
			list->head->prev = new_node;
			list->head = new_node;
		} else {
			list->head = new_node;
		}
	} else {
		for (unsigned int i = 0; i < n - 1; i++)
			current = current->next;

		dll_node_t *temp = current->next;

		new_node->next = temp;
		current->next = new_node;

		new_node->prev = current;
		if (temp)
			temp->prev = new_node;
	}
	list->size++;
}

// Removes a node from a doubly linked list(non cyclic)
dll_node_t*
dll_remove_nth_node(list_t *list, unsigned int n)
{
	if (!list->head || n >= list->size)
		return NULL;

	list->size--;

	dll_node_t *current = list->head;

	if (n == 0) {
		list->head = current->next;
		if (list->head)
			list->head->prev = NULL;
		return current;
	}

	for (unsigned int i = 0; i < n; i++)
		current = current->next;

	current->prev->next = current->next;

	if (current->next)
		current->next->prev = current->prev;

	return current;
}

// Frees all the data from a doubly linked list
void
dll_free(list_t **pp_list)
{
	dll_node_t *current = (*pp_list)->head;
	int n = (*pp_list)->size;
	while (n) {
		dll_node_t *temp = current;
		current = current->next;
		free(temp->data);
		free(temp);
		n--;
	}
	free(*pp_list);
	*pp_list = NULL;
}
