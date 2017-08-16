#include "macros.h"
#include "types.h"
#include "lp.h"
#include "st.h"

#include <sys/types.h>
#include <regex.h>

#include "lp/c.tab.h"

// Debugger lexer/parser.
#ifdef DEBUG

void
pccc_lp_analyze(pccc_context *ctxt, pccc_buffer *buf){
	// Test to make sure the lexer/parser works.
	/*pccc_symbol_def *def = PCCC_MALLOC(pccc_symbol_def, 1);
	def->name = PCCC_MALLOC(char, 4);
	strncpy(def->name, "foo", 3);
	def->name[3] = '\0';*/

	pccc_st_set(ctxt->symbols, "foo", "bar");
	pccc_st_set(ctxt->symbols, "foo", "bar");
	//pccc_st_set(ctxt->symbols, "bar", NULL);
	//pccc_st_set(ctxt->symbols, "ben", NULL);
	//free(def);
}

void
pccc_lp_tokenize(pccc_context *ctxt, pccc_buffer *buf){

}

pccc_lp pccc_lp_test = { &pccc_lp_analyze, &pccc_lp_tokenize };

#endif

pccc_lp pccc_lp_c = { &pccc_lp_c_analyze, &pccc_lp_c_tokenize };

regmatch_t**
pccc_lp_regex_match(const regex_t *reg, char *contents, size_t nmatches, int *matches){
	// Keep track of all matches made so far.
	regmatch_t **matches = PCCC_MALLOC(regmatch_t *, PCCC_REGEX_MATCHES);
	regmatch_t *temp_match = PCCC_MALLOC(regmatch_t, nmatches);
	int offset = 0;
	int actual_match = 0;

	// Loop until EOF or until no more matchesany match.
	while ((*section != EOF || *section != '\0') && regexec(reg, section + offset, nmatches, temp_match, REG_EXTENDED)){
		// Increase actual matches.
		actual_match++;
		// Realloc if necessary.
		if (actual_match % PCCC_REGEX_MATCHES == 1 && actual_match > 10){
			int remainder = actual_match % PCCC_REGEX_MATCHES;
			int new_size = (PCCC_REGEX_MATCHES - remainder + actual_match); // Create a new allocation block with an even size.
			matches = realloc(matches, new_size);
		}

		// Increment the section. Do this after correcting the regexes.
		int temp_end_offset = temp_match[0].rm_eo; 

		// Correct the offset of the match.
		for (int i = 0; i < nmatches; i++){
			if (temp_match[i].rm_so != -1){
				temp_match[i].rm_so += offset;
				temp_match[i].rm_eo += offset;
			}
		}

		// Copy matches over.
		matches[actual_match - 1] = temp_match;

		// Create a new pointer.
		temp_match = PCCC_MALLOC(regmatch_t, nmatches);

		offset += temp_end_offset;
	}

	*matches = actual_match;

	return temp_match;
}

char **
pccc_lp_c_headers(pccc_buffer *buf){
	pccc_linked_list *l = pccc_linked_list_create();
	
	// Get all matches.

	// Get the file name within the substring.

	// Copy the file name.

	// Check that the buffer doesn't already exist. USE THE FULL PATH

	// Fork and preprocess.

	// Create a new buffer from file.

	// Return.
}

void
pccc_lp_c_define(pccc_context *ctxt){
	// Compile the regex.
	static regex_t defreg;
	if (defreg.buffer == NULL)
		regcomp(PCCC_C_LP_DEFINE, &defreg, REG_EXTENDED);

	// Search for all defines.

	int m = 0;
	pccc_lp_regex_match()


	// Insert all matches into the symbol table.
}

void 
pccc_lp_c_analyze(pccc_context *ctxt, pccc_buffer *buf){
	// Load the buffer.



	// Preprocess the file. Isolate the include and defines.

	// Defines will be put into the st.

	// Include will be sent through cc.



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