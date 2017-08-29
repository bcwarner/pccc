
#include <stdio.h>
#include <string.h>

#include "macros.h"
#include "types.h"
#include "st.h"

// Change this to a TST.
// Implementation based on the description of Algorithms, 4th ed. by Sedgewick and Wayne.
// Linked list implementation to help with collection.

pccc_linked_list*
pccc_linked_list_create(){
	return PCCC_MALLOC(pccc_linked_list, 1);
}

void
pccc_linked_list_add(pccc_linked_list *list, void * val, size_t size){
	// Allocate the node.
	pccc_linked_list_node *node = PCCC_MALLOC(pccc_linked_list_node, 1);
	
	// Allocate the memory for the item.
	node->val = calloc(sizeof(void), size);
	memcpy(node->val, val, size);

	// Extend the linked list. Increment N.
	if (list->first == NULL)
		list->first = node;
	if (list->last != NULL)
		list->last->next = node;
	list->last = node;
	list->N++;
}

// Don't worry about length since its accessible from N.
void **
pccc_linked_list_array(pccc_linked_list *list){
	void **array = PCCC_MALLOC(void *, list->N);

	pccc_linked_list_node *node = list->first;
	int c = 0;

	if (node == NULL){
		return array; // Return early since there is nothing to add.
	}

	do { // Traverse the nodes and add them to the array.
		array[c++] = node->val; // Assign the node, then increment.
		node = node->next; // Goto next node.
	} while (node != NULL);
	return array;
}

// Converts a linked list of symbols into an array of symbols encapsulated in the pccc_suggestions struct.
pccc_suggestions *
pccc_suggestionize(pccc_linked_list *list){
	pccc_suggestions *sug = PCCC_MALLOC(pccc_suggestions, 1);
	sug->suggestions = (char **)pccc_linked_list_array(list);
	sug->N = list->N;
	return sug;
}

pccc_st*
pccc_st_init(){
	// Allocate the root, then the table.
	pccc_st* r = PCCC_MALLOC(pccc_st, 1);
	r->mutex = PCCC_MALLOC(pthread_mutex_t, 1);
	pthread_mutex_init(r->mutex, NULL);
	return r;
}

pccc_st_node*
pccc_st_search(pccc_st* t, char * key){
	PCCC_PRINTF("Searching for key %s", key);
	// Copy the key since it is freed quickly.
	//size_t sk = strlen(key);
	//char *k = PCCC_MALLOC(char, sk);
	//strncpy(k, key, sk);
	pthread_mutex_lock(t->mutex);
	pccc_st_node *r = pccc_st_search_node(t->root, key, strlen(key), 0);
	pthread_mutex_unlock(t->mutex);
	return r;
}

pccc_st_node*
pccc_st_search_node(pccc_st_node* node, char *key, size_t len, unsigned int i){
	PCCC_PRINTF("Searching node with key '%s' at position %u with max index %lu. Value of node val: %p", key, i, len - 1, node);
	if (node == NULL){ // Empty, return.
		return NULL;
	}
	if (len == i) // Match.
		return node;
	PCCC_PRINTF("Next node = key[%u]", (unsigned int)key[i]);
	return pccc_st_search_node(node->next[(unsigned int)key[i]], key, len, i + 1);
}

pccc_linked_list* // Do not change to *void[] since endpoint needs to know size of array.
pccc_st_search_prefix(pccc_st* t, char * key){
	// Create the search queue.
	pccc_linked_list *l = pccc_linked_list_create();

	// Search
	pccc_st_node *start = pccc_st_search(t, key);
	pthread_mutex_lock(t->mutex);
	pccc_st_search_prefix_node(start, key, strlen(key), l);
	pthread_mutex_unlock(t->mutex);

	return l;
}

void
pccc_st_search_prefix_node(pccc_st_node *node, char *key, size_t len, pccc_linked_list *list){
	if (node == NULL)
		return;
	if (node->val != NULL){
		PCCC_PRINTF("Adding key to linked list: %s\n", key);
		pccc_linked_list_add(list, key, len + 1);
	} // Add the node to the queue.
	// Iterate through the next array and find matches. Create new strings for them.
	size_t new_strlen = len + 1;
	for (int c = 0; c < PCCC_RADIX; c++){
		char *new_str = PCCC_MALLOC(char, new_strlen + 1);
		strncpy(new_str, key, len);
		new_str[len] = (char)(0xFF & c); // Recurse. Mask and cast.

		pccc_st_search_prefix_node(node->next[c], new_str, new_strlen, list);

		// Free the memory.

		free(new_str);
	}
}

void 
pccc_st_set(pccc_st *t, char *key, void *val){
	printf("Setting value for key '%s'\n", key);
	pthread_mutex_lock(t->mutex);
	t->root = pccc_st_set_node(t->root, key, val, strlen(key), 0);
	pthread_mutex_unlock(t->mutex);
}

// Potential error with passing pointers.
pccc_st_node*
pccc_st_set_node(pccc_st_node* node, char * key, void * val, size_t len, unsigned int i){
	PCCC_PRINTF("Searching for key %s at position %d", key, i);
	if (node == NULL){
		node = PCCC_MALLOC(pccc_st_node, 1);
		PCCC_PRINTF("Allocating new node for node %p", node);
		for (int i = 0; i < PCCC_RADIX; i++)
			node->next[i] = NULL;
	}
	if (i == len){
		PCCC_PRINTF("Found node. Setting to value %p", val);
		node->val = val;
		return node;
	}

	node->next[(int)key[i]] = pccc_st_set_node(node->next[(int)key[i]], key, val, len, i + 1); // Keep going recursively.
	return node;
}

/*

Aug 24 22:07:21 ubuntu plugin_host: Searching for key /home/bcwarner/srcs/senior-project/tests/test_sublime.c
Aug 24 22:07:21 ubuntu plugin_host: Searching node with key '/home/bcwarner/srcs/senior-project/tests/test_sublime.c' at position 0 with max index 54. Value of node val: (nil)
Aug 24 22:07:21 ubuntu plugin_host: Next node = key[47]

*/