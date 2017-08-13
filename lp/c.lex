%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c.tab.h"
/*
(\".*\"|\S)*		return TOKEN_SEQUENCE;
[^\n]*				return TOKEN_FILE;
*/
//%option prefix="pccc_lp_c" Removed because it causes issues.
%}



%option nounistd
%option never-interactive

%%



"/*".*"*/"			;
"//".*\n 			;
(auto|register|static|extern|typedef) 	return TOKEN_STORAGE_CLASS_SPECIFIER;
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
define				return TOKEN_DEFINE;
include				return TOKEN_INCLUDE;
(\=|\*\=|\/=|\%\=|\+\=|\-\=|\<\<\=|\>\>\=|\&\=|\^\=|\|\=) return TOKEN_ASSIGNMENT; // Split up.
"..."				return TOKEN_ELLIPSIS;
"||"				return TOKEN_LOGICAL_OR;
"&&"				return TOKEN_LOGICAL_AND;
"++"				return TOKEN_INCREMENT;
"--"				return TOKEN_DECREMENT;
"->"				return TOKEN_PTR_MEMBER;
"=="				return TOKEN_EQUAL;
">="				return TOKEN_GTE;
">>"				return TOKEN_RSHIFT;
"<<"				return TOKEN_LSHIFT;
"!="				return TOKEN_NOT_EQUAL;
"<="				return TOKEN_LTE;
(\&|\*|\+|\-|\~|\!)	return TOKEN_UNARY_OP; // Split up.
[a-zA-Z\_][a-zA-Z0-9\_]* return TOKEN_IDENTIFIER;
\".*\"				return TOKEN_STRING;
(0x|0|0X)?[0-9A-Fa-f]*(u|U|l|L)? return TOKEN_INTEGER;
L?\'(\\\'|\\[^\\\']*|.)\'	return TOKEN_CHARACTER;
[0-9]*\.?[0-9]*(e[0-9]*)?(f|F|l|L)?	return TOKEN_FLOATING;
. 					{}

%%