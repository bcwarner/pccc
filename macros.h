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
// Name: PCCC (Portable Context-Cognizant Completer)

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>

#define PCCC_MALLOC(p, s) ((p *)calloc(sizeof(p), s))

#define PCCC_BUFFER_STATIC 0 // Referenced file, will never change.
#define PCCC_BUFFER_DYNAMIC 1 // File being edited, may change.
#define PCCC_BUFFER_ANALYZED 2
#define PCCC_BUFFER_LOCAL 4
#define PCCC_BUFFER_PREALLOC 8 // Used for when a buffer has already been allocated by the library.

#define PCCC_BUFFER_MALLOC 4096

#define PCCC_SUCCESS 0 
#define PCCC_FAILURE -1

#define PCCC_LP_ALL -1 // States that lexer/parser should parse all bufs.

#define PCCC_RADIX 256 // Radix of string trie.

#ifdef DEBUG
	#define PCCC_PRINTF(str, ...) syslog(LOG_DEBUG, str, __VA_ARGS__); //syslog(LOG_DEBUG, str, __VA_ARGS__);
#else
	#define PCCC_PRINTF(str, ...)
#endif

// C Regex

#define PCCC_C_LP_DEFINE "\\#define[[:space:]]*([^[:space:]]*)[\\t ]*([^\\n\\r\\f\\#]*)[\\n\\r\\f\\t ]" // We have define as a capturing group to differentiate between defines. TODO: Make it work for n-parameter defines.
#define PCCC_C_LP_INCLUDE "\\#include[[:space:]]*[\\\"<][[:space:]]*([a-zA-Z0-9.,\\\\\\/_]*)[[:space:]]*[\\\">]"

#define PCCC_REGEX_MATCHES 10 // Number of matches that it should be expanded by.
#define PCCC_REGEX_LEN(m) (m.rm_eo - m.rm_so)

#define PCCC_FOLDER_PATH '/'
#define PCCC_PATH_PREFIX "PATH" PCCC_FOLDER_PATH
#define PCCC_ENV_VAR {"PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin", NULL}

