#include "../macros.h"
#include "../types.h"
#include "../pccc.h"
#include "../buffer.h"

#include <stdio.h>

const char *fi = "#include <stdio.h>\n"
//"#include \"lp/c.tab.h\"\n" // This seems to cause an error.
"#define A B 10\n"
"#define PCCC_TEST\n\n"
"int main(int argc, char *argv[]){\n"
"	#define C 9\n"
"}";

int main(int argc, char *argv[]){
	// Create a context.
	pccc_context *ctxt = pccc_init(NULL, 0);

	// Load an imaginary C file.
	pccc_buffer *buf = pccc_buffer_init("~/srcs/senior-project/test.c", fi, strlen(fi), PCCC_BUFFER_STATIC);

	pccc_add_buffer(ctxt, buf);

	// Preprocess.

	pccc_analyze(ctxt, buf);

	sleep(1);

	/*	printf("Search for a symbol:");
	char s[10];
	scanf("%9s", s);*/
	pccc_suggestions *sug = pccc_suggest_prefix(ctxt, "");

	for (int i = 0; i < sug->N; i++)
		printf("Result %d: \t %s\n", i, sug->suggestions[i]);
}

