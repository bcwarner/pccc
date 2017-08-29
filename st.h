/*
    PCCC: A portable library for context-cognizant completions.
    Copyright (C) 2017  bcwarner

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

Search Trie Symbol Table
We use this since it is simpler to implement than a hash table, yet still suitable for our need of storing thousands of keys.
Use the prefix pccc_st_ since we may want to change the symbol table type later, but don't want to necessarily break any dependencies.
Implementation based on the description of Algorithms, 4th ed. by Sedgewick and Wayne. Their implementation is licensed under the GPLv3 and any conditions added under section 7 of the GPLv3.
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

pccc_suggestions *
pccc_suggestionize(pccc_linked_list *list);

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


