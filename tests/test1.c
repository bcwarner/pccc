#include <stdio.h>
#include "../pccc.h"
#include "../types.h"
#include "../buffer.h"
#include "../st.h"
#include "../lp.h"

#include <unistd.h>

int
main(int argc, char *argv[]){
	// Load a buffer.
	char *str1 = "foo = bar";
	char *str2 = "bar = foo";
	pccc_buffer * buf1 = pccc_buffer_init("test.test", str1, strlen(str1), 0);
	pccc_buffer * buf2 = pccc_buffer_init("test2.test", str2, strlen(str2), 0);
	printf("Successfully created the buffers.\n");

	// Create a context.
	// Adding buf1 and buf2
	pccc_buffer ** ba = (pccc_buffer **)malloc(sizeof(pccc_buffer **) * 2);
	ba[0] = buf1;
	ba[1] = buf2;

	printf("Initializing the context.\n");
	pccc_context *ctxt = pccc_init(ba, 2);

	pccc_lp * lp = pccc_select_lp(buf1->name);
	if (lp != NULL)
		printf("Successful choice!\n");
	else
		printf("Failed.\n");

	// Analyze the content.
	printf("Analyzing the context's buffers.\n");
	pccc_analyze(ctxt, buf1);

	sleep(2);

	// Iterate through the data.

	printf("Searching for all symbols.\n");
	pccc_linked_list *l = pccc_st_search_prefix(ctxt->symbols, "");
	char **array = (char **)pccc_linked_list_array(l);

	for (int i = 0; i < l->N; i++)
		printf("Result %d: \t %s\n", i, array[i]);

	// Collect the data.
	//pccc_suggest(ctxt, "foo");

	// Print the output.

	return 0;
}