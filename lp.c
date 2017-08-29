#include "macros.h"
#include "types.h"
#include "lp.h"
#include "st.h"
#include "buffer.h"
#include "pccc.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <regex.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

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
pccc_lp_regex_match(const regex_t *reg, char *contents, size_t nmatches, int *omatches){
	// Keep track of all matches made so far.
	regmatch_t **matches = PCCC_MALLOC(regmatch_t *, PCCC_REGEX_MATCHES);
	regmatch_t *temp_match = PCCC_MALLOC(regmatch_t, nmatches);
	char *off_contents = contents;
	int offset = 0;
	int actual_match = 0;

	// Loop until EOF or until no more matchesany match.
	while ((*off_contents != EOF && *off_contents != '\0') && regexec(reg, off_contents, nmatches, temp_match, REG_EXTENDED) == 0){
		// Increase actual matches.
		actual_match++; 
		PCCC_PRINTF("Regex match #%d found\n", actual_match);
		// Realloc if necessary.
		if (actual_match % PCCC_REGEX_MATCHES == 1 && actual_match > 10){
			int remainder = actual_match % PCCC_REGEX_MATCHES;
			int new_size = (PCCC_REGEX_MATCHES - remainder + actual_match); // Create a new allocation block with an even size.
			matches = realloc(matches, new_size);
			PCCC_PRINTF("Reallocated the match buffer to %u bytes\n", new_size);
		}

		// Increment the section. Do this after correcting the regexes.
		int temp_end_offset = temp_match[nmatches - 1].rm_eo; 
		PCCC_PRINTF("Base offset: %u bytes\n", temp_end_offset);

		// Correct the offset of the match.
		for (int i = 0; i < nmatches; i++){
			if (temp_match[i].rm_so != -1){
				temp_match[i].rm_so += offset;
				temp_match[i].rm_eo += offset;
				PCCC_PRINTF("New start offset of match %d: %u-%u bytes\nString match: %.*s\n", i, temp_match[i].rm_so, temp_match[i].rm_eo, (temp_match[i].rm_eo - temp_match[i].rm_so), contents + temp_match[i].rm_so);
			}
		}

		// Copy matches over.
		matches[actual_match - 1] = temp_match;

		// Create a new pointer.
		temp_match = PCCC_MALLOC(regmatch_t, nmatches);

		offset += temp_end_offset;
		PCCC_PRINTF("Increased offset to: %u\n", offset);
		off_contents = contents + offset;
	}

	PCCC_PRINTF("Matches found: %d", actual_match);

	*omatches = actual_match;

	return matches;
}

void
pccc_lp_c_headers(pccc_context *ctxt, pccc_buffer *buf){
	// Compile the regex.
	static regex_t headreg;
	if (headreg.__buffer == NULL)
		regcomp(&headreg, PCCC_C_LP_INCLUDE, REG_EXTENDED);

	// Get all matches.
	int m = 0;
	regmatch_t** matches = pccc_lp_regex_match(&headreg, buf->contents, 2, &m);

	// Get the file name within the substring.
	for (int i = 0; i < m; i++){

		// Get the file name.
		// Copy the file name.
		regmatch_t *match = matches[i];
		int flen = PCCC_REGEX_LEN(match[1]);
		char *fn = PCCC_MALLOC(char, flen + 1);
		strncpy(fn, buf->contents + match[1].rm_so, match[1].rm_eo - match[1].rm_so);
		fn[flen] = '\0';

		PCCC_PRINTF("Copied file name [%d-%d]: %s\n", match[1].rm_so, match[1].rm_eo, fn);

		if (pccc_st_search(ctxt->buffers, fn) != NULL) { // Return since this file is already in use.
			PCCC_PRINTF("File %s already exists.", fn);
			continue;
		}

		// Determine the flags.
		int flags = 0;
		short local = 0;
		char *folder;
		size_t folderlen;
		PCCC_PRINTF("Type of include: %c\n", *(buf->contents + match[0].rm_eo - 1));
		if (*(buf->contents + match[0].rm_eo - 1) == '\"'){
			// Find the prefix.

			if (access(fn, F_OK | R_OK) != 0){
				PCCC_PRINTF("Error: File %s is not accessible.\n", fn)
			}
			PCCC_PRINTF("File %s exists\n", fn);

			folder = pccc_buffer_get_folder(buf->name, &folderlen);
			PCCC_PRINTF("Containing folder: %s\n", folder);
			local = 2; // Set this so that the include statement string will be expanded to escape characters.
		} // Otherwise, prepend a constant prefix

		int input[2]; // For echoing into the child.
		int output[2]; // For getting the preprocessed result.

		pipe(input);
		pipe(output);

		// Get the include statement.
		int ilen = PCCC_REGEX_LEN(match[0]);
		char *is = PCCC_MALLOC(char, ilen + 1);
		strncpy(is, buf->contents + match[0].rm_so, match[0].rm_eo - match[0].rm_so);
		
		// Do this to escape quotes

		if (local != 0){
			char *new_str = PCCC_MALLOC(char, ilen + local + 1);


			// Create two separate tracks.
			int j = 0;
			for (int i = 0; i < ilen; i++){
				if (is[i] == '\"'){
					new_str[j++] = '\\';
					new_str[j++] = '\"';
					continue;
				} else
					new_str[j++] = is[i];
				PCCC_PRINTF("String so far: %.*s\n", i, new_str);
			}

			PCCC_PRINTF("New string: %s\n", new_str);

			free(is);
			is = new_str;
		}

		is[ilen + local] = '\0';
		PCCC_PRINTF("Include match: %s\n", is);

		int arg2len = 27 + ilen + local + 1;
		// Strange memory mismanagement is occurring, apparently from another thread. Will be removed later.
		char *arg2 = PCCC_MALLOC(char, arg2len);
		// Create the final command.
		snprintf(arg2, arg2len, "echo \"%s\" | gcc -E -P -x c -", is); // This command is 30 characters long.

		PCCC_PRINTF("Argument passed to execlp: %s\n", arg2);

		// Fork and preprocess. Pipe the result to a buffer.
		pid_t cpre = fork();

		if (cpre == 0){
			static char * env[] = PCCC_ENV_VAR;
			//close(input[1]); // Close the ends of the pipe we aren't concerned with.
			close(output[0]);
			PCCC_PRINTF("Closed {%d}.\n", output[0]);
			if (folder != NULL){
				chdir(folder);
				PCCC_PRINTF("Changed folder to: %s\n", folder);
			}
			//dup2(stdin, fd[0]); // Shift the pipes so they work upon exec.
			dup2(output[1], STDOUT_FILENO); // FIX THIS SO IT WORKS
			close(output[1]);
			//PCCC_PRINTF("Dup2'd %d to stdout\n", output[1]);

			char *loc = "/bin/bash";
			char *c = "-c";
			char *argv[] = {
				loc,
				c,
				arg2,
				NULL
			};

			//PCCC_PRINTF("Argument being passed: %s\n", arg2);
			if (execve("/bin/bash", argv, env) == -1)
				PCCC_PRINTF("Failed to execve. Errno: %d\n", errno);
		} else {
			int status;
			//close(input[0]); // Close the uninmportant side, and echo into the program.
			waitpid(cpre, &status, 0);
			PCCC_PRINTF("Result of fork: %d\n", status);

			close(output[1]);
			// Get the size of the buffer.

			// Continuously read until the number of bytes read is less than the size of a block.
			int blocks = 1;
			int nbytes = 0;
			char *buf = PCCC_MALLOC(char, PCCC_BUFFER_MALLOC);

			PCCC_PRINTF("Succesfully allocated a buffer. Address: %p", buf);

			while ((nbytes = read(output[0], buf + ((blocks - 1) * PCCC_BUFFER_MALLOC), PCCC_BUFFER_MALLOC)) == PCCC_BUFFER_MALLOC){
				buf = realloc(buf, PCCC_BUFFER_MALLOC * (++blocks));
				PCCC_PRINTF("Reallocated to %d bytes", PCCC_BUFFER_MALLOC * (blocks));
			}

    		PCCC_PRINTF("Output: %.*s\n", blocks * PCCC_BUFFER_MALLOC, buf);

			pccc_buffer *new_buf = pccc_buffer_init(fn, buf, PCCC_BUFFER_MALLOC * blocks, PCCC_BUFFER_STATIC | PCCC_BUFFER_PREALLOC);
			pccc_add_buffer(ctxt, new_buf);

			PCCC_PRINTF("Starting analysis of new buffer: %s", new_buf->name);
			pccc_lp_c_analyze(ctxt, new_buf);
			PCCC_PRINTF("Finished analysis of new buffer: %s", new_buf->name);

			close(input[1]);
		}

		// Check that the buffer doesn't already exist. USE THE FULL PATH
		
		// Create a new buffer from file.

		free(arg2);
	}

	// Return.
}

void
pccc_lp_c_define(pccc_context *ctxt, pccc_buffer *buf){
	// Compile the regex.
	static regex_t defreg;
	if (defreg.__buffer == NULL)
		regcomp(&defreg, PCCC_C_LP_DEFINE, REG_EXTENDED);

	// Search for all defines.

	int m = 0;
	regmatch_t **matches = pccc_lp_regex_match(&defreg, buf->contents, 3, &m);

	// Insert all matches into the symbol table.

	for (int i = 0; i < m; i++){
		regmatch_t *match = matches[i];

		if (match[1].rm_so == -1)
			continue;

		// Copy the contents of the key from the buffer.
		size_t sk = PCCC_REGEX_LEN(match[1]);
		char *key = PCCC_MALLOC(char, sk + 1);
		strncpy(key, buf->contents + match[1].rm_so, sk);
		key[sk] = '\0';
		PCCC_PRINTF("Found key: %s", key);

		char *val;
		// Copy the contents of the value from the buffer.
		if (match[2].rm_so != -1){ // Check the last match since it is optional.
			size_t sv = PCCC_REGEX_LEN(match[2]);
			val = PCCC_MALLOC(char, sv + 1);
			strncpy(val, buf->contents + match[2].rm_so, sv);
			val[sv] = '\0';
			PCCC_PRINTF("Found value: %s", val);
		} else {
			val = PCCC_MALLOC(char, 1);
			val[0] = '\0';
		}

		// Store the item into the symbol table.
		pccc_st_set(ctxt->symbols, key, val);
	}
}

void 
pccc_lp_c_analyze(pccc_context *ctxt, pccc_buffer *buf){
	// Load the buffer.



	// Preprocess the file. Isolate the include and defines.

	// Defines will be put into the st.
	pccc_lp_c_define(ctxt, buf);
	// Include will be sent through cc.
	pccc_lp_c_headers(ctxt, buf);


	// Load the buffer.

	pccc_lp_c_scan_buffer(buf->contents, buf->len);

	// Analyze. Replace this with yyparse later.
	pccc_lp_cparse(ctxt);
}

void 
pccc_lp_c_tokenize(pccc_context *ctxt, pccc_buffer *buf){
	//pccc_lp_lex

}

void
pccc_lp_default_fn(pccc_context *ctxt, pccc_buffer *buf){
	return;
}

pccc_lp pccc_lp_default = { &pccc_lp_default_fn, &pccc_lp_default_fn };

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
		//PCCC_PRINTF("Chose test lp.");
		return &pccc_lp_test;
	}
	#endif

	else
		return &pccc_lp_default;

	return NULL;
}