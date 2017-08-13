/*
Binary Symbol Table
We use this since it is simpler to implement than a hash table, yet still suitable for our need of storing thousands of keys.
Use the prefix pccc_st_ since we may want to change the symbol table type later, but don't want to necessarily break any dependencies.
*/

#include <stdio.h>
#include <string.h>

#include "macros.h"
#include "types.h"

pccc_linked_list*
pccc_linked_list_create();

void
pccc_linked_list_add(pccc_linked_list *list, void * val, size_t size);

void **
pccc_linked_list_array(pccc_linked_list *list);

pccc_st* 
pccc_st_init();

pccc_st_node* // Double pointer gives access to the node in memory, but the actual reference as well.
pccc_st_search(pccc_st* t, char * key);

pccc_st_node*
pccc_st_search_node(pccc_st_node* node, char *key, size_t len, unsigned int i);

void
pccc_st_set(pccc_st *t, char *key, void *val);

pccc_st_node*
pccc_st_set_node(pccc_st_node* node, char * key, void * val, size_t len, unsigned int i);

pccc_linked_list* // Do not change to *void[] since endpoint needs to know size of array.
pccc_st_search_prefix(pccc_st* t, char * key);

void
pccc_st_search_prefix_node(pccc_st_node *node, char *key, size_t len, pccc_linked_list *list);


