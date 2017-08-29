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
#include "macros.h"
#include "types.h"
#include "buffer.h"
#include "st.h"
#include "lp.h"
#include "pccc.h"

#include <stdlib.h>
#include <pthread.h>

// Creates the context for the program. Takes an array of buffers.
pccc_context* 
pccc_init(pccc_buffer ** buf, int n){
	// Create the context.
	pccc_context * ctxt = PCCC_MALLOC(pccc_context, 1);
	ctxt->buffers = pccc_st_init();
	ctxt->threads = pccc_st_init();
	ctxt->symbols = pccc_st_init();

	// Move the buffers into the symbol table.
	for (int i = 0; i < n; i++){
		pccc_st_set(ctxt->buffers, buf[i]->name, buf[i]);
	}

	// Assign the appropriate lexer/parser.
	return ctxt;
}

// Analyzes a string. Returns an array with all possible symbols.
pccc_suggestions*
pccc_suggest(pccc_context* ctxt, char *s){
	// Call lexer/parser.

	// Tokenize the string.

	// If reversed tokens_1 == whitespace => suggest from a prefix.

	// If reversed tokens_1 suggests the 0th token is a child => suggest from a parent/child table.
	return NULL;
}

// Analyzes a string without any preceding tokens. Returns an array with all possible symbols.
pccc_suggestions*
pccc_suggest_prefix(pccc_context* ctxt, char *s){
	// We don't need to lock here

	// Search the trie for relevant symbols.
	pccc_linked_list *l = pccc_st_search_prefix(ctxt->symbols, s);

	// Convert the values from a linked list into a series of suggestions.
	pccc_suggestions *r = pccc_suggestionize(l);

	// Return values.
	return r;
	
}

// Adds a buffer
void
pccc_add_buffer(pccc_context *ctxt, pccc_buffer *buf){
	// Initialize the buffer.

	// Add the buffer to the symbol table.
	pccc_st_set(ctxt->buffers, buf->name, (void *)buf);
}

pccc_buffer*
pccc_add_new_buffer(pccc_context *ctxt, char *name, char *contents, int len, int flags){
	pccc_buffer *buf = pccc_buffer_init(name, contents, len, flags);
	PCCC_PRINTF("Created new buffer %s with size of %d. Sample: %*.s", name, len, 50, contents);
	pccc_st_set(ctxt->buffers, buf->name, (void *)buf);
	return buf;
}

pccc_buffer* 
pccc_get_buffer(pccc_context *ctxt, char *name){
	return (pccc_buffer *)pccc_st_search(ctxt->buffers, name);
}

void
pccc_update_buffer(pccc_context *ctxt, char *name, char *contents, int len){
	// Get the buffer.
	pccc_buffer *b = pccc_get_buffer(ctxt, name);

	// Update the buffer contents.
	pccc_buffer_update(b, contents, len);
}

struct pccc_analysis_args {
	pccc_context *ctxt;
	pccc_buffer *buf;
};

void*
pccc_analyze_async(void *a){
	// Lock the buffer.
	struct pccc_analysis_args *args = (struct pccc_analysis_args *)a;
	pthread_mutex_lock(args->buf->mutex);

	PCCC_PRINTF("Locked mutex for %s.", args->buf->name);

	// Select an analyzer based upon file name.

	pccc_lp * lp = pccc_select_lp(args->buf->name); 

	// Call the parser using these args.
	lp->analyze(args->ctxt, args->buf);

	PCCC_PRINTF("Finished analysis for %s.", args->buf->name);

	// Exit the thread.
	pthread_mutex_unlock(args->buf->mutex);
	return NULL;
}

// Analyzes a buffer asynchronously. Returns if it succeeded. Note that the API will pay attention to the address and whether it has already been analyzed as a buffer.
int
pccc_analyze(pccc_context* ctxt, pccc_buffer *buf){
	// Create the thread.
	pthread_t *thread = PCCC_MALLOC(pthread_t, 1);
	struct pccc_analysis_args *args = PCCC_MALLOC(struct pccc_analysis_args, 1);
	args->ctxt = ctxt;
	args->buf = buf;

	pccc_st_set(ctxt->threads, buf->name, thread); // Add the thread to the threads ST.
	pthread_create(thread, NULL, &pccc_analyze_async, (void *)args);// Pass the analysis to the thread.
	return PCCC_SUCCESS;
}

// Analyzes all buffers asynchronously. Returns if it succeeded. Note that the API will pay attention to the address and whether it has already been analyzed as a buffer.
int
pccc_analyze_all(pccc_context* ctxt){
	// Create the thread.

	// Pass the analysis to the thread.
	return PCCC_SUCCESS;
}