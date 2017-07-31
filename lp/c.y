/*
Lexer for C
Uses the definition found in K&R C, 2nd Edition, pg. 234-239, with several adjustments.
*/
{%
	#include <stdio.h>
	#include <string.h>
	 
	void yyerror(const char *str)
	{
	        printf("error: %s\n",str);
	}
	 
	int yywrap()
	{
	        return 1;
	}
%}

%%

translation_unit: external_declaration
				| translation_unit external_declaration
				;

external_declaration: function_definition
				| declaration
				;

function_definition: declaration_specifiers declarator declaration_list compound_statement
				| declarator declaration_list compound_statement
				| declaration_specifiers declarator compound_statement
				| declarator declarator compound_statement
				; // Do stuff here for pccc

