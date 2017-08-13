#include "macros.h"
#include "types.h"
#include "lp.h"
#include "st.h"

#include "lp/c.tab.h"

// Debugger lexer/parser.
#ifdef DEBUG

void
pccc_lp_analyze(pccc_context *ctxt, pccc_buffer *buf){
	// Test to make sure the lexer/parser works.
	pccc_st_set(ctxt->symbols, "foo", "bar");
}

void
pccc_lp_tokenize(pccc_context *ctxt, pccc_buffer *buf){

}

pccc_lp pccc_lp_test = { &pccc_lp_analyze, &pccc_lp_tokenize };

#endif

pccc_lp pccc_lp_c = { &pccc_lp_c_analyze, &pccc_lp_c_tokenize };

void 
pccc_lp_c_analyze(pccc_context *ctxt, pccc_buffer *buf){
	// Load the buffer.
	//pccc_lp_c_scan_buffer(buf->contents, buf->len);

	// Analyze.
	//pccc_lp_cparse(ctxt);
}

void 
pccc_lp_c_tokenize(pccc_context *ctxt, pccc_buffer *buf){
	//pccc_lp_lex
}

pccc_lp * pccc_select_lp(char *fn){
	size_t len = strlen(fn);
	int i = len;
	while(i >= 0 && fn[i - 1] != '.') i--; // Find file extension.

	// Test multiple cases.
	char *fe = &fn[i];
	size_t fe_len = len - i;
	
	if (strncmp(fe, "c", fe_len) == 0 || strncmp(fe, "h", fe_len) == 0)
		return &pccc_lp_c;
	
	#ifdef DEBUG
	else if (strncmp(fe, "test", fe_len) == 0){
		PCCC_PRINTF("Chose test lp.");
		return &pccc_lp_test;
	}
	#endif

	return NULL;
}