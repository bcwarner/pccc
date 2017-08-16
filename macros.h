// Name: PCCC (Portable Context-Cognizant Completer)

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PCCC_MALLOC(p, s) ((p *)malloc(sizeof(p) * s))

#define PCCC_BUFFER_STATIC 0 // Referenced file, will never change.
#define PCCC_BUFFER_DYNAMIC 1 // File being edited, may change.
#define PCCC_BUFFER_ANALYZED 2

#define PCCC_SUCCESS 0
#define PCCC_FAILURE -1

#define PCCC_LP_ALL -1 // States that lexer/parser should parse all bufs.

#define PCCC_RADIX 256 // Radix of string trie.

#ifdef DEBUG
	#define PCCC_PRINTF(str) printf("%s", str);
#else
	#define PCCC_PRINTF(str)
#endif

// C Regex

#define PCCC_C_LP_DEFINE "\\#define\\s*(\\S*)\\s*(\\S*)?" // We have define as a capturing group to differentiate between defines.
#define PCCC_C_LP_INCLUDE "\\#include\\s*[\"<]\\s*([a-zA-Z0-9.,\\\\\\/_]*)\\s*[\">]"