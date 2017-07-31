#include <stdio.h>
#include <string.h>

#include "macros.h"
#include "st.h"

pccc_st*
pccc_st_init(){
	// Allocate the root, then the table.
	pccc_st* r = PCCC_MALLOC(pccc_st, 1);
}

pccc_st_node*
pccc_st_search(pccc_st* t, char * key){
	return pccc_st_search_node(t->root, key, strlen(key));
}

pccc_st_node*
pccc_st_search_node(pccc_st_node* node, char *key, size_t len){
	int cmp = strncmp(key, node->key, len);
	if (cmp == 0)
		return node;
	else if (cmp < 0)
		return pccc_st_search_node(node->l, key, len);
	else
		return pccc_st_search_node(node->r, key, len);
}

void 
pccc_st_set(pccc_st *t, char *key, void *val){
	pccc_st_set_node(t->root, key, val);
}

// Potential error with passing pointers.
pccc_st_node*
pccc_st_set_node(pccc_st_node* node, char * key, void * val){
	if (node == NULL){
		node = PCCC_MALLOC(pccc_st_node, 1);
		node->key = key;
		node->val = val;
		return node;
	}

	int cmp = strncmp(key, node->key, len);
	if (cmp == 0){
		node->val = val;
		return node;
	}
	else if (cmp < 0)
		node->l = pccc_st_set_node(node->l, key, len);
	else
		node->r = pccc_st_set_node(node->r, key, len);
}