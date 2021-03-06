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
*/
#pragma once

#include "macros.h"
#include "types.h"

typedef struct pccc_buffer {
	char * contents;
	char * name;
	size_t len;
	unsigned int flags;
	pthread_mutex_t *mutex; // Used for locking the analysis of the buffer.
} pccc_buffer;

typedef struct pccc_linked_list_node {
	void *val;
	struct pccc_linked_list_node *next; 
} pccc_linked_list_node; 

typedef struct pccc_linked_list {
	pccc_linked_list_node *first;
	pccc_linked_list_node *last;
	int N;
} pccc_linked_list;

typedef struct pccc_st_node {
	void * val;
	struct pccc_st_node *next[PCCC_RADIX];
} pccc_st_node;

typedef struct pccc_st {
	pccc_st_node* root;
	// int N; Not applicable
	pthread_mutex_t *mutex;
} pccc_st;

typedef struct pccc_context {
	pccc_st* buffers;
	pccc_st* symbols;
	pccc_st* threads;
} pccc_context;

/*typedef struct pccc_symbol_def {
	char *name;
	char *parent_name;
	char *inherits_name;
	char *definition;
	pccc_linked_list *children_names;
} pccc_symbol_def;*/

typedef struct pccc_suggestions {
	char **suggestions; 
	int N;
} pccc_suggestions;

typedef struct pccc_lp {
	void (* analyze)(pccc_context *, pccc_buffer *); // Analyze a pointer to a buffer of size n.
	void (* tokenize)(pccc_context *, pccc_buffer *); // Not sure what to do here yet, need to analyze yylex.
} pccc_lp;