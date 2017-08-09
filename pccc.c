#include "macro.h"
#include "st.h"
#include "buffer.h"

// Creates the context for the program. Takes an array of buffers.
pccc_context* 
pccc_init(pccc_buffer ** buf, int n){
	// Create the context.
	pccc_context * ctxt = PCCC_MALLOC(pccc_context, 1);
	ctxt->buffers = pccc_st_init();
	ctxt->parent_keys = pccc_st_init();
	
	// Move the buffers into the symbol table.
	for (int i = 0; i < n; i++){
		pccc_st_set(ctxt->buffers, buf[i]->name, buf[i]);
	}

	// Assign the appropriate lexer/parser.
}

// Analyzes a string. Returns an array with all possible symbols.
pccc_suggestions*
pccc_suggest(pccc_context* ctxt, char *s){
	// Call lexer/parser.

	// Tokenize the string.

	// If reversed tokens_1 == whitespace => suggest from a prefix.

	// If reversed tokens_1 suggests the 0th token is a chiled => suggest from a parent/child table.
}

// Analyzes a string without any preceding tokens. Returns an array with all possible symbols.
pccc_suggestions*
pccc_suggest_prefix(pccc_context* ctxt, char *s){
	// Search the trie for relevant symbols.

	// Return values.
}

// Adds a buffer
void
pccc_add_buffer(pccc_context *ctxt, char *name, char *contents, int len, int flags){
	// Initialize the buffer.
	pccc_buffer_init()

	// Add the buffer to the symbol table.
	pccc_st_set(cxt->buffers, buf->name, (void *)buf);
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

// Analyzes a buffer asynchronously. Returns if it succeeded. Note that the API will pay attention to the address and whether it has already been analyzed as a buffer.
void
pccc_analyze(pccc_context* ctxt, pccc_buffer *buf){
	// Create the thread.

	// Pass the analysis to the thread.
}

// Analyzes all buffers asynchronously. Returns if it succeeded. Note that the API will pay attention to the address and whether it has already been analyzed as a buffer.
void
pccc_analyze_all(pccc_context* ctxt){
	// Create the thread.

	// Pass the analysis to the thread.
}