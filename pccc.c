#include "macro.h"
#include "st.h"

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

// Analyzes a string. Returns a ST with all possible symbols.
pccc_st*
pccc_suggest(pccc_context* ctxt, char *s);

// Analyzes a string without any preceding tokens. Returns a ST with all possible symbols.
pccc_st*
pccc_suggest_any(pccc_context* ctxt, char *s);

// Analyzes a buffer. Returns if it succeeded. Note that the API will pay attention to the address and whether it has already been analyzed as a buffer.
int
pccc_analyze(pccc_context* ctxt, pccc_buffer * buf);

// Analyzes all buffers. Returns if it succeeded. Note that the API will pay attention to the address and whether it has already been analyzed as a buffer.
int
pccc_analyze_all(pccc_context* ctxt);
