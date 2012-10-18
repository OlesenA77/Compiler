/*Alexander Olesen
 *1671836
 *CMPT 399
 */

%{
#include <stdio.h>
#include "globals.h"

int result;
static int yylex ();
void yyerror(char const *);
%}

%token T_ENDFILE
%token T_ERROR
%token T_AND T_OR T_NOT
%token T_IF T_THEN T_ELSE
%token T_WHILE T_DO
%token T_BEGIN T_END
%token T_CASE
%token T_ARRAY T_INT T_CONS T_VAR
%token T_OF
%token T_READ T_WRITE
%token T_INBET
%token T_ADD T_SUB T_MUL T_DIV T_MOD T_SHL T_SHR
%token T_EQ T_GTHAN T_LTHAN T_GEQ T_LEQ
%token T_LPAREN T_RPAREN T_COMMA T_PERIOD
%token T_COLON T_SEMCLN T_ASSIGN
%token T_NUM
%token T_ID
%token T_NL

%nonassoc LOWER_THAN_ELSE
%nonassoc T_ELSE

%%

program						: block
									;

block 						: const_section var_section T_BEGIN statement_list T_END
									;

const_section 		: T_CONS const_item
									| /*empty*/
									;

const_item				: const_decl const_item
									| const_decl
									;

const_decl				: T_ID T_EQ T_NUM
									;

var_section				: T_VAR var_decls
									| /*empty*/
									;

var_decls					: var_decls var_decl
									| var_decl
									;

var_decl					: var_list T_COLON type T_SEMCLN
									;

var_list					: var_list T_COMMA T_ID
									|	T_ID
										{
											/*$$ to a newIDfunction*/
										}
									;

type							: T_ARRAY T_LPAREN constant T_RPAREN T_OF T_INT
									| T_INT
									;

statement_list		: statement_list T_SEMCLN statement
									| statement
									;

statement					: null_statement
									| assign_statement
									| block_statement
									| if_statement
									| while_statement
									| case_statement
									| write_statement
									| read_statement
									;

null_statement  	:
									;

assign_statement	: identifier T_ASSIGN expression
									;

block_statement 	: block
									;

if_statement			: T_IF condition T_THEN statement T_ELSE %prec LOWER_THAN_ELSE
									| T_IF condition T_THEN statement T_ELSE statement
									;

while_statement 	: T_WHILE condition T_DO statement
									;

condition					: expression
									;

case_statement		: T_CASE expression T_OF case_list T_ELSE statement T_END
									| T_CASE expression T_OF case_list T_END
									;

case_list					: case_list T_SEMCLN case
									| case
									;

case							: const_list T_COLON statement
									;

const_list				: const_list T_COMMA constant
									| constant
									;

write_statement		: T_WRITE T_LPAREN write_list T_RPAREN
									;

write_list				: write_list T_COMMA expression
									| expression
									;

read_statement		: T_READ T_LPAREN read_list T_RPAREN
									;

read_list					: read_list T_COMMA identifier
									| identifier
									;

expression				: simple_expression rel_operator simple_expression
									| simple_expression
									;

simple_expression	: unary_operator term add_sub_or
									| unary_operator term
									| term operator_term
									| term
									;

operator_term			: add_sub_or term operator_term
									| add_sub_or term
									;

term							: factor operator_factor
									| factor
									;

operator_factor		: factor other_op
									| factor
									;

factor						: constant
									| identifier
									| T_NOT factor
									| T_LPAREN expression T_RPAREN
									;

rel_operator			: T_EQ
									| T_INBET
									| T_GTHAN
									| T_LTHAN
									| T_GEQ
									| T_LEQ
									;

unary_operator		: T_ADD
									| T_SUB
									;

add_sub_or				: T_ADD
									| T_SUB
									| T_OR
									;

other_op					: T_MUL
									| T_DIV
									| T_MOD
									| T_AND
									| T_SHL
									| T_SHR
									;

identifier				: simple_name
									| simple_name T_LPAREN expression T_RPAREN
									;

constant					: T_NUM
									;

/*variable				: simple_name T_LPAREN expression T_RPAREN
									| simple_name
									;*/

simple_name				: T_ID
									;

%%

int parse ()
{
    if (yyparse () == 0)
	printf ("Parse succeeded: %d\n", result);
    else
	printf ("Parse failed.\n");

    return 0;
}

static int yylex ()
{
    return (getToken ());
}

void yyerror(char const *str)
{
    printf (">>%s<<\n", str);
}
