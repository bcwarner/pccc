%{
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

    Lexer for C. 
    Uses the definition found in K&R C, 2nd Edition, Appendix A, with several adjustments.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c.tab.h"
#include "../types.h"
#include "../pccc.h"
/*
(\".*\"|\S)*		return TOKEN_SEQUENCE;
[^\n]*				return TOKEN_FILE;
*/

#define YY_DECL int yylex(pccc_context *ctxt)

#define YY_USER_ACTION pccc_lp_coffset += yyleng;

int typedefc;
int typedefo;

int pccc_lp_coffset;

%}

%option prefix="pccc_lp_c"

%%

"/*".*"*/"			;
"//".*\n 			;
(auto|register|static|extern) return TOKEN_STORAGE_CLASS_SPECIFIER;
typedef 			{ typedefc++; typedefo = 1; return TOKEN_STORAGE_CLASS_SPECIFIER; } // Trick to skip full-blown analysis.
(void|char|short|int|long|float|double|signed|unsigned)	return TOKEN_TYPE_SPECIFIER;
(const|volatile)	return TOKEN_TYPE_QUALIFIER;
(struct|union)		return TOKEN_STRUCT_OR_UNION;
enum				return TOKEN_ENUM;
case				return TOKEN_CASE;
default				return TOKEN_DEFAULT;
if					return TOKEN_IF;
switch				return TOKEN_SWITCH;
else				return TOKEN_ELSE;
do 					return TOKEN_DO;
while				return TOKEN_WHILE;
for					return TOKEN_FOR;
goto				return TOKEN_GOTO;
continue			return TOKEN_CONTINUE;
break				return TOKEN_BREAK;
return				return TOKEN_RETURN;
sizeof				return TOKEN_SIZEOF;
(\=|\*\=|\/=|\%\=|\+\=|\-\=|\<\<\=|\>\>\=|\&\=|\^\=|\|\=) return TOKEN_ASSIGNMENT; // Split up.s
"!="				return TOKEN_NOT_EQUAL;
"<="				return TOKEN_LTE;
[a-zA-Z\_][a-zA-Z0-9\_]* { pccc_lp_clval.str_val = pccc_lp_ctext; printf("Found identifier %s\n", pccc_lp_ctext); pccc_st_set(ctxt->symbols, pccc_lp_ctext, pccc_lp_ctext); if(typedefc > 0 || typedefo == 0) { typedefc = 0; return TOKEN_IDENTIFIER; } else { typedefo = 0; return TOKEN_TYPE_SPECIFIER; } }
\".*\"				return TOKEN_STRING;
(0x|0|0X)?[0-9A-Fa-f]*(u|U|l|L)? return TOKEN_INTEGER;
L?\'(\\\'|\\[^\\\']*|.)\'	return TOKEN_CHARACTER;
[0-9]*\.?[0-9]*(e[0-9]*)?(f|F|l|L)?	return TOKEN_FLOATING;
. 					{}

%%