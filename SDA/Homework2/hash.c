#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "server.h"
#include "utils.h"
#include "load_balancer.h"

#define MAX_STRING_SIZE	256
#define HMAX 100000

linked_list_t *ll_create(unsigned int data_size)
{
    linked_list_t* ll;

    ll = malloc(sizeof(*ll));

    ll->head = NULL;
    ll->data_size = data_size;
    ll->size = 0;

    return ll;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Daca n >= nr_noduri, noul nod se adauga la finalul listei. Daca
 * n < 0, eroare.
 */
void ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
    ll_node_t *prev, *curr;
    ll_node_t* new_node;

    if (!list) {
        return;
    }

    /* n >= list->size inseamna adaugarea unui nou nod la finalul listei. */
    if (n > list->size) {
        n = list->size;
    }

    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    new_node = malloc(sizeof(*new_node));
    new_node->data = malloc(list->data_size);
    memcpy(new_node->data, new_data, list->data_size);

    new_node->next = curr;
    if (prev == NULL) {
        /* Adica n == 0. */
        list->head = new_node;
    } else {
        prev->next = new_node;
    }

    list->size++;
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se elimina nodul de
 * la finalul listei. Daca n < 0, eroare. Functia intoarce un pointer spre acest
 * nod proaspat eliminat din lista. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */
ll_node_t *ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
    ll_node_t *prev, *curr;

    if (!list || !list->head) {
        return NULL;
    }

    /* n >= list->size - 1 inseamna eliminarea nodului de la finalul listei. */
    if (n > list->size - 1) {
        n = list->size - 1;
    }

    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    if (prev == NULL) {
        /* Adica n == 0. */
        list->head = curr->next;
    } else {
        prev->next = curr->next;
    }

    list->size--;

    return curr;
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int ll_get_size(linked_list_t* list)
{
     if (!list) {
        return -1;
    }

    return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista si actualizeaza la
 * NULL valoarea pointerului la care pointeaza argumentul (argumentul este un
 * pointer la un pointer).
 */
void ll_free(linked_list_t** pp_list)
{
    ll_node_t* currNode;

    if (!pp_list || !*pp_list) {
        return;
    }

    while (ll_get_size(*pp_list) > 0) {
        currNode = ll_remove_nth_node(*pp_list, 0);
        free(currNode->data);
        currNode->data = NULL;
        free(currNode);
        currNode = NULL;
    }

    free(*pp_list);
    *pp_list = NULL;
}

/*
 * Functii de comparare a cheilor:
 */
int compare_function_servers(void *a, void *b)
{
	int int_a = *((int *)a);
	int int_b = *((int *)b);

	if (int_a == int_b) {
		return 0;
	} else if (int_a < int_b) {
		return -1;
	} else {
		return 1;
	}
}

int compare_function_key(void *a, void *b)
{
	char *str_a = (char *)a;
	char *str_b = (char *)b;
	return strcmp(str_a, str_b);
}

void key_val_free_function(void *data) 
{
	
    if(!data)
        return;
    info *information = (info*) data;
    
    if (information->key)
        free(information->key);
    if(information->value)
        free(information->value);
    
    free(information);
}
/*
 * Functie apelata dupa alocarea unui hashtable pentru a-l initializa.
 * Trebuie alocate si initializate si listele inlantuite.
 */
hashtable_t *ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*),
		void (*key_val_free_function)(void*)) // modificata, nu alocam ll_list, scapam de for
{
	hashtable_t *table = malloc(sizeof(hashtable_t));
    table->buckets = calloc(hmax, sizeof(linked_list_t));
    table->hmax = hmax;
    table->size = 0;

    for (unsigned int i = 0; i < hmax; ++i) {
        table->buckets[i] = ll_create(sizeof(info));
    }
    
    table->key_val_free_function = key_val_free_function;
    table->hash_function = hash_function;
    table->compare_function = compare_function;
    
    return table;
}

/*
 * Functie care intoarce:
 * 1, daca pentru cheia key a fost asociata anterior o valoare in hashtable
 * folosind functia put;
 * 0, altfel.
 */
int ht_has_key(hashtable_t *ht, void *key)
{
	__uint32_t index = ht->hash_function(key) % ht->hmax;
    linked_list_t *entry_in = ht->buckets[index];

    if(entry_in == NULL)
        return 0;
    
    ll_node_t *node = entry_in->head;
    
    while(node != NULL) {
        info *information = node->data;
        
        if(ht->compare_function(key, information->key) == 0) 
            return 1;
        
        node = node->next;
    }

	return 0;
}
void *ht_has_key_pointer(hashtable_t *ht, void *key)
{
	__uint32_t index = ht->hash_function(key) % ht->hmax;
    linked_list_t *entry_in = ht->buckets[index];

    if(entry_in == NULL)
        return NULL;
    
    ll_node_t *node = entry_in->head;
    
    while(node != NULL) {
        info *information = node->data;
        
        if(ht->compare_function(key, information->key) == 0) 
            return information->value;
        
        node = node->next;
    }

	return NULL;
}
/*
 * Atentie! Desi cheia este trimisa ca un void pointer (deoarece nu se impune
 * tipul ei), in momentul in care se creeaza o noua intrare in hashtable (in
 * cazul in care cheia nu se gaseste deja in ht), trebuie creata o copie a
 * valorii la care pointeaza key si adresa acestei copii trebuie salvata in
 * structura info asociata intrarii din ht. Pentru a sti cati octeti trebuie
 * alocati si copiati, folositi parametrul key_size.
 *
 * Motivatie:
 * Este nevoie sa copiem valoarea la care pointeaza key deoarece dupa un apel
 * put(ht, key_actual, value_actual), valoarea la care pointeaza key_actual
 * poate fi alterata (de ex: *key_actual++). Daca am folosi direct adresa
 * pointerului key_actual, practic s-ar modifica din afara hashtable-ului cheia
 * unei intrari din hashtable. Nu ne dorim acest lucru, fiindca exista riscul sa
 * ajungem in situatia in care nu mai stim la ce cheie este inregistrata o
 * anumita valoare.
 */
void ht_put(hashtable_t *ht, void *key, unsigned int key_size,
    void *value, unsigned int value_size)
{
    __uint32_t index = ht->hash_function(key) % ht->hmax;

    ht->size++;

    info *information = malloc(sizeof(info));
    information->key = malloc(key_size + 1); // Add space for null terminator
    information->value = malloc(value_size + 1); // Add space for null terminator

    memcpy(information->key, key, key_size);
    ((char*)(information->key))[key_size] = '\0'; // Add null terminator

    memcpy(information->value, value, value_size);
    ((char*)(information->value))[value_size] = '\0'; // Add null terminator

    ll_add_nth_node(ht->buckets[index], ht->buckets[index]->size, information);

    free(information);
}


/*
 * Procedura care elimina din hashtable intrarea asociata cheii key.
 * Atentie! Trebuie avuta grija la eliberarea intregii memorii folosite pentru o
 * intrare din hashtable (adica memoria pentru copia lui key --vezi observatia
 * de la procedura put--, pentru structura info si pentru structura Node din
 * lista inlantuita).
 */
void ht_remove_entry(hashtable_t *ht, void *key)
{
	__uint32_t index = ht->hash_function(key) % ht->hmax;
    linked_list_t *entry = ht->buckets[index];
    
    ll_node_t *curr = entry->head;
    __uint32_t poz = 0;

    while( curr != NULL) {
        info *data = curr->data;
        if(ht->compare_function(data->key,key) == 0) {
            ll_node_t *remove = ll_remove_nth_node(entry,poz);
            info *information = remove->data;
            free(information->key);
            free(information->value);
            free(information);
            free(remove->data);
            free(remove);
            ht->size--;
            return;
        }
        curr=curr->next;
        poz++;
    }
    
}

/*
 * Procedura care elibereaza memoria folosita de toate intrarile din hashtable,
 * dupa care elibereaza si memoria folosita pentru a stoca structura hashtable.
 */
void ht_free(hashtable_t *ht) // modifcata pt server ca sa fie inainte de ll_list!!!!!!!!!!
{	
    for (unsigned int i = 0 ;i < ht->hmax; ++i) {
        ll_node_t *node = ht->buckets[i]->head;
        while (node != NULL)
        {
            info *data = node->data;
            free(data->key);
            free(data->value);
            //free(data);
            node = node->next;
        }
        ll_free(&ht->buckets[i]);
        
    }

    free(ht->buckets);
    free(ht);
}

unsigned int ht_get_size(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->size;
}

unsigned int ht_get_hmax(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->hmax;
}