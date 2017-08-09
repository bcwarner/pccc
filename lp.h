/*
Defines the properties any lexer/parser must have.
*/

#include "macro.h"

typedef struct pccc_lp {
	void (* analyze)(pccc_context *, pccc_buffer *); // Analyze a pointer to a buffer of size n.
	void (* tokenize)(pccc_context *, pccc_buffer *); 
} pccc_lp;

const *pccc_lp {
	analyze = pccc_lp_c_analyze;
	tokenize = pccc_lp_c_tokenize;
} pccc_lp_c;

void 
pccc_lp_c_analyze(pccc_context *ctxt, pccc_buffer * buf);

void 
pccc_lp_c_tokenize(pccc_context *ctxt, pccc_buffer * buf);
