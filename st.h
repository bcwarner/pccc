/*
Binary Symbol Table
We use this since it is simpler to implement than a hash table, yet still suitable for our need of storing thousands of keys.
Use the prefix pccc_st_ since we may want to change the symbol table type later, but don't want to necessarily break any dependencies.
*/

#include <stdio.h>
#include <string.h>

#include "macros.h"

typedef struct pccc_st_node {
	char * key;
	void * val;
	pccc_st_node* l;
	pccc_st_node* r;
	// int N; Not applicable
} pccc_st_node;

typedef struct pccc_st {
	pccc_st_node* root;
	// int N; Not applicable
} pccc_st;

pccc_st* 
pccc_st_init();

pccc_st_node* // Double pointer gives access to the node in memory, but the actual reference as well.
pccc_st_search(pccc_st* t, char * key);

pccc_st_node*
pccc_st_search_node(pccc_st_node* node, char * key, size_t len);

void
pccc_st_set(pccc_st *t, char *key, void *val);

pccc_st_node*
pccc_st_set_node(pccc_st_node* node, char * key, void * val);





