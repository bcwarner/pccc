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
#include "../macros.h"
#include "../types.h"
#include "../pccc.h"
#include "../buffer.h"

#include <stdio.h>

const char *fi = //"#include <stdio.h>\n"
"#include \"lp/c.tab.h\"\n" // This seems to cause an error.
"#define A B 10\n"
"#define PCCC_TEST\n\n"
"int main(int argc, char *argv[]){\n"
"	#define C 9\n"
"}";

int main(int argc, char *argv[]){
	// Create a context.
	pccc_context *ctxt = pccc_init(NULL, 0);

	// Load an imaginary C file.

	pccc_buffer *result = pccc_get_buffer(ctxt, "foo.bar");
	printf("Result of failed search: %p\n", result);

	pccc_buffer *buf = pccc_add_new_buffer(ctxt, "/home/bcwarner/srcs/senior-project/test.c", fi, strlen(fi), PCCC_BUFFER_STATIC);

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

