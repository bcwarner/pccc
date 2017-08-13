/*
Lexer for C
Uses the definition found in K&R C, 2nd Edition, pg. 234-239, with several adjustments.
Crossreferenced against: https://www.lysator.liu.se/c/ANSI-C-grammar-y.html
*/
%{
	//#pragma once
	//#include <stdio.h>
	//#include <string.h>
	//#include <stdlib.h>
	#include "../pccc.h"
	#include "../types.h"
	 
	/*void main(){
		pccc_lp_cparse();
	}*/

	void yyerror(const char *str)
	{
	        printf("error: %s\n",str);
	}
	 
	int yywrap()
	{
	        return 1;
	}
%}


// Debugger info.
%define parse.error verbose
%define parse.trace 
%printer { fprintf(stdout, "Parsed: %s", $$); } <*>

// yyparse info
//%parse-param {pccc_context *ctxt}
//%define api.prefix {pccc_lp_c}

%token TOKEN_STORAGE_CLASS_SPECIFIER;
%token TOKEN_TYPE_SPECIFIER;
%token TOKEN_TYPE_QUALIFIER;
%token TOKEN_STRUCT_OR_UNION;
%token TOKEN_ENUM;
%token TOKEN_ELLIPSIS;
%token TOKEN_CASE;
%token TOKEN_DEFAULT;
%token TOKEN_IF;
%token TOKEN_ELSE;
%token TOKEN_DO;
%token TOKEN_WHILE;
%token TOKEN_SWITCH;
%token TOKEN_FOR;
%token TOKEN_GOTO;
%token TOKEN_CONTINUE
%token TOKEN_BREAK;
%token TOKEN_RETURN;
%token TOKEN_ASSIGNMENT;
%token TOKEN_LOGICAL_OR;
%token TOKEN_LOGICAL_AND;
%token TOKEN_INCREMENT;
%token TOKEN_DECREMENT;
%token TOKEN_SIZEOF;
%token TOKEN_PTR_MEMBER;
%token TOKEN_DEFINE;
%token TOKEN_SEQUENCE;
%token TOKEN_INCLUDE;
%token TOKEN_FILE;
%token TOKEN_UNARY_OP;
%token TOKEN_IDENTIFIER;
%token TOKEN_STRING;
%token TOKEN_INTEGER;
%token TOKEN_CHARACTER;
%token TOKEN_FLOATING;
%token TOKEN_EQUAL;
%token TOKEN_GTE;
%token TOKEN_RSHIFT;
%token TOKEN_LSHIFT;
%token TOKEN_NOT_EQUAL;
%token TOKEN_LTE;

%start translation_unit

%%

constant: TOKEN_INTEGER
				| TOKEN_CHARACTER
				| TOKEN_FLOATING // Enumeration is covered by int.
				;

primary_expression: TOKEN_IDENTIFIER
				| constant
				| TOKEN_STRING
				| '(' expression ')'
				;

postfix_expression: primary_expression
				| postfix_expression '[' expression ']'
				| postfix_expression '(' argument_expression_list ')'
				| postfix_expression '(' ')'
				| postfix_expression '.' TOKEN_IDENTIFIER
				| postfix_expression TOKEN_PTR_MEMBER TOKEN_IDENTIFIER
				| postfix_expression TOKEN_INCREMENT
				| postfix_expression TOKEN_DECREMENT
				;

argument_expression_list: assignment_expression
				| argument_expression_list ',' assignment_expression
				;

unary_expression: postfix_expression
				| TOKEN_INCREMENT unary_expression
				| TOKEN_DECREMENT unary_expression
				| TOKEN_UNARY_OP cast_expression
				| TOKEN_SIZEOF unary_expression
				| TOKEN_SIZEOF '(' type_name ')'
				;

cast_expression: unary_expression
				| '(' type_name ')' cast_expression
				;

multiplicative_expression: cast_expression
				| multiplicative_expression '*' cast_expression
				| multiplicative_expression '/' cast_expression
				| multiplicative_expression '%' cast_expression
				;


additive_expression: multiplicative_expression
				| additive_expression '+' multiplicative_expression
				| additive_expression '-' multiplicative_expression
				;

shift_expression: additive_expression
				| shift_expression TOKEN_LSHIFT additive_expression
				| shift_expression TOKEN_RSHIFT additive_expression
				;

relational_expression: shift_expression
				| relational_expression '<' shift_expression
				| relational_expression '>' shift_expression
				| relational_expression TOKEN_LTE shift_expression
				| relational_expression TOKEN_GTE shift_expression
				;

equality_expression: relational_expression
				| equality_expression TOKEN_EQUAL relational_expression
				| equality_expression TOKEN_NOT_EQUAL relational_expression
				;

AND_expression: equality_expression
				| AND_expression '&' equality_expression
				;

exclusive_OR_expression: AND_expression
				| exclusive_OR_expression '^' AND_expression
				;

inclusive_OR_expression: exclusive_OR_expression
				| inclusive_OR_expression '|' exclusive_OR_expression
				;

logical_AND_expression: inclusive_OR_expression
				| logical_AND_expression TOKEN_LOGICAL_AND inclusive_OR_expression
				;

logical_OR_expression: logical_AND_expression
				| logical_OR_expression TOKEN_LOGICAL_OR logical_AND_expression
				;


conditional_expression: logical_OR_expression
				| logical_OR_expression '?' expression ':' conditional_expression
				;

assignment_expression: conditional_expression
				| unary_expression TOKEN_ASSIGNMENT assignment_expression
				;

expression: assignment_expression
				| expression ',' assignment_expression
				;

constant_expression: conditional_expression;

declaration: declaration_specifiers init_declarator_list
				| declaration_specifiers
				;

declaration_specifiers: TOKEN_STORAGE_CLASS_SPECIFIER declaration_specifiers
				| TOKEN_STORAGE_CLASS_SPECIFIER
				| type_specifier declaration_specifiers
				| type_specifier
				| type_qualifier declaration_specifiers
				| type_qualifier
				;

init_declarator_list: init_declarator
				| init_declarator_list ',' init_declarator
				;

init_declarator: declarator
				| declarator '=' initializer
				;

type_specifier: TOKEN_TYPE_SPECIFIER
				| struct_or_union_specifier
				| enum_specifier
				| TOKEN_IDENTIFIER
				;

struct_or_union_specifier: TOKEN_STRUCT_OR_UNION TOKEN_IDENTIFIER '{' struct_declaration_list '}'
				| TOKEN_IDENTIFIER '{' struct_declaration_list '}'
				| TOKEN_STRUCT_OR_UNION TOKEN_IDENTIFIER
				;

struct_declaration_list: struct_declaration
				| struct_declaration_list struct_declaration
				;

struct_declaration: specifier_qualifer_list struct_declarator_list ';' ;

specifier_qualifer_list: type_specifier specifier_qualifer_list
				| type_specifier
				| type_qualifier specifier_qualifer_list
				| type_qualifier
				;
struct_declarator_list: struct_declarator
				| struct_declarator_list ',' struct_declarator
				;

struct_declarator: declarator
				| declarator ':' constant_expression
				| ':' constant_expression
				;

enum_specifier: TOKEN_ENUM TOKEN_IDENTIFIER '{' enumerator_list '}'
				| TOKEN_ENUM '{' enumerator_list '}'
				| TOKEN_ENUM TOKEN_IDENTIFIER
				;

enumerator_list: enumerator
				| enumerator_list ',' enumerator
				;

enumerator:	TOKEN_IDENTIFIER
				| TOKEN_IDENTIFIER '=' constant_expression
				;

type_qualifier: TOKEN_TYPE_QUALIFIER
				;

declarator: pointer direct_declarator
				| direct_declarator
				;

direct_declarator: TOKEN_IDENTIFIER
				| '(' declarator ')'
				| direct_declarator '[' constant_expression ']'
				| direct_declarator '[' ']'
				| direct_declarator '(' parameter_type_list ')'
				| direct_declarator '(' identifier_list ')'
				| direct_declarator '(' ')'
				;

pointer: '*' type_qualifier_list
				| '*' 
				| '*' type_qualifier_list pointer
				| '*' pointer
				;

type_qualifier_list: type_qualifier
				| type_qualifier_list type_qualifier
				;

parameter_type_list: parameter_list
				| parameter_list ',' TOKEN_ELLIPSIS
				;

parameter_list: parameter_declaration
				| parameter_list ',' parameter_declaration
				;

parameter_declaration: declaration_specifiers declarator
				| declaration_specifiers abstract_declarator
				| declaration_specifiers
				;

identifier_list: TOKEN_IDENTIFIER
				| identifier_list ',' TOKEN_IDENTIFIER
				;

type_name: specifier_qualifer_list abstract_declarator
				| specifier_qualifer_list
				;

abstract_declarator: pointer
				| pointer direct_abstract_declarator
				| direct_abstract_declarator
				;

direct_abstract_declarator: '(' abstract_declarator ')'
				| direct_abstract_declarator '[' constant_expression ']'
				| direct_abstract_declarator '[' ']'
				| '[' constant_expression ']'
				| '[' ']'
				;

initializer: assignment_expression
				| '{' initializer_list '}'
				| '{' initializer_list ',' '}'
				;

initializer_list: initializer
				| initializer_list ',' initializer
				;

statement: labeled_statement
				| expression_statement
				| compound_statement
				| selection_statement
				| iteration_statement
				| jump_statement
				;

labeled_statement: TOKEN_IDENTIFIER ':' statement
				| TOKEN_CASE constant_expression ':' statement
				| TOKEN_DEFAULT ':' statement
				;

compound_statement: '{' declaration_list statement_list '}'
				| '{' statement_list '}'
				| '{' declaration_list '}'
				| '{' '}'
				;

declaration_list: declaration
				| declaration_list declaration
				;

statement_list: statement
				| statement_list statement
				;

expression_statement: expression ';'
				| ';'
				;

selection_statement: TOKEN_IF '(' expression ')' statement
				| TOKEN_IF '(' expression ')' statement TOKEN_ELSE statement
				| TOKEN_SWITCH '(' expression ')' statement
				;

iteration_statement: TOKEN_WHILE '(' expression ')' statement
				| TOKEN_DO statement TOKEN_WHILE '(' expression ')' ;
				| TOKEN_FOR '(' expression ';' expression ';' expression ')' statement
				| TOKEN_FOR '(' expression ';' expression ';' ')' statement
				| TOKEN_FOR '(' expression ';' ';' ')' statement
				| TOKEN_FOR '(' expression ';' ';' expression ')' statement
				| TOKEN_FOR '('  ';' expression ';' expression ')' statement
				| TOKEN_FOR '(' ';' ';' expression ')' statement
				| TOKEN_FOR '(' ';' expression ';' ')' statement
				| TOKEN_FOR '(' ';' ';' ')' statement
				;

jump_statement: TOKEN_GOTO TOKEN_IDENTIFIER ';'
				| TOKEN_CONTINUE ';'
				| TOKEN_BREAK ';'
				| TOKEN_RETURN expression ';'
				| TOKEN_RETURN ';'

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


/*
control_line: '#' define
			| '#' include
			;

define: TOKEN_DEFINE TOKEN_SEQUENCE TOKEN_SEQUENCE;

include: TOKEN_INCLUDE '<' TOKEN_FILE '>'
			| TOKEN_INCLUDE '"' TOKEN_FILE '"'
			;
*/