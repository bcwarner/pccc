// Name: PCCC (Portable Context-Cognizant Completer)

#include <stdlib.h>

#define PCCC_MALLOC(p, s) (p *)malloc(sizeof(p) * s)

#define PCCC_BUFFER_STATIC 0; // Referenced file, will never change.
#define PCCC_BUFFER_DYNAMIC 1; // File being edited, may change.
#define PCCC_BUFFER_ANALYZED 2;

#define PCCC_SUCCESS 0;
#define PCCC_FAILURE -1;

#define PCCC_LP_ALL -1; // States that lexer/parser should parse all bufs.



