/*Alexander Olesen
 *1671836
 *CMPT 399
 */

%{
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "Tree.h"
#define  YYERROR_VERBOSE
int result;
static int yylex ();
void yyerror(char const *);
TreeBranch *head;
TreeBranch *tmp;
%}

%token T_ENDFILE //1
%token T_ERROR   //2
%token T_AND     //3
%token T_OR      //4
%token T_NOT     //5
%token T_IF      //6
%token T_THEN    //7
%token T_ELSE    //8
%token T_WHILE   //9
%token T_DO      //10
%token T_BEGIN   //11
%token T_END     //12
%token T_CASE    //13
%token T_ARRAY   //14
%token T_INT     //15
%token T_CONS    //16
%token T_VAR     //17
%token T_OF      //18
%token T_READ		 //19
%token T_WRITE	 //20
%token T_INBET   //21
%token T_ADD     //22
%token T_SUB     //23
%token T_MUL     //24
%token T_DIV     //25
%token T_MOD     //26
%token T_SHL     //27
%token T_SHR     //28
%token T_EQ      //29
%token T_GTHAN   //30
%token T_LTHAN   //31
%token T_GEQ     //32
%token T_LEQ     //33
%token T_LPAREN  //34
%token T_RPAREN  //35
%token T_COMMA   //36
%token T_PERIOD  //37
%token T_COLON   //38
%token T_SEMCLN  //39
%token T_ASSIGN  //40
%token T_NUM     //41
%token T_ID      //42
%token T_NL      //43

%nonassoc LOWER_THAN_ELSE
%nonassoc T_ELSE

%debug
%error-verbose
%locations


%union {
	char * string;
	float	num;
	struct _TreeBranch *Branch;
	}
%type <Branch> program
%type <Branch> block
%type <Branch> const_section
%type <Branch> const_item
%type <Branch> const_decl
%type <Branch> var_section
%type <Branch> var_decls
%type <Branch> var_decl
%type <Branch> var_list
%type <Branch> type
%type <Branch> statement_list
%type <Branch> statement
%type <Branch> null_statement
%type <Branch> assign_statement
%type <Branch> block_statement
%type <Branch> if_statement
%type <Branch> while_statement
%type <Branch> condition
%type <Branch> case_statement
%type <Branch> case_list
%type <Branch> case
%type <Branch> const_list
%type <Branch> write_statement
%type <Branch> write_list
%type <Branch> read_statement
%type <Branch> read_list
%type <Branch> expression
%type <Branch> simple_expression
%type <Branch> operator_term
%type <Branch> term
%type <Branch> factor

%type <string> rel_operator
%type <string> unary_operator
%type <string> add_sub_or
%type <string> other_op

%type <Branch> identifier
%type <Branch> constant
%type <Branch> simple_name
%%

program						: block T_PERIOD
									;

block 						: const_section var_section T_BEGIN statement_list T_END
									;

const_section 		: T_CONS const_item
									| {$$ = NULL;}
									;

const_item				: const_decl const_item
									| const_decl
									;

const_decl				: T_ID T_EQ T_NUM
									;

var_section				: T_VAR var_decls
									| {$$ = NULL;}
									;

var_decls					: var_decls var_decl
									| var_decl
									;

var_decl					: var_list T_COLON type T_SEMCLN
									;

var_list					: var_list T_COMMA T_ID
									|	T_ID
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

if_statement			: T_IF condition T_THEN statement %prec LOWER_THAN_ELSE
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

simple_expression	: unary_operator term operator_term
									| unary_operator term
									| term operator_term
									| term
									;

operator_term			: add_sub_or term operator_term
										{
											$$ = initializeTreeBranch();
											setTreeBranch($$, $2, NULL, NULL, NULL, "exp", "op",
																			$1);
											if($3->sibling != NULL)
											{	
												$3 = $3->sibling;
											}
											while(tmp->sibling != NULL)
											{	
												$3 = $3->sibling;
											}
											$3->sibling = $$;
										}
									| add_sub_or term
										{
											$$ = initializeTreeBranch();
											setTreeBranch($$, $2, NULL, NULL, NULL, "exp", "op",
																			$1);
										}
									;

term							: term other_op factor
										{
											$$ = initializeTreeBranch();
											setTreeBranch($$, $1, $2, NULL, NULL, "exp", "op",
																			$2);
										}
									| factor
										{$$ == $1;}
									;

factor						: constant
										{
											$$ = initializeTreeBranch();
											setTreeBranch($$, NULL, NULL, NULL, NULL, "exp", "const",
																			$1);
										}
									| identifier
										{
											$$ = initializeTreeBranch();
											setTreeBranch($$, NULL, NULL, NULL, NULL, "exp", "id",
																			$1);
										}
									| T_NOT factor
										{
											$$ = initializeTreeBranch();
											setTreeBranch($$, $1, NULL, NULL, NULL, "exp", "NOT",
																			NULL);
										}
									| T_LPAREN expression T_RPAREN
										{
											$$ = initializeTreeBranch();
											setTreeBranch($$, $1, NULL, NULL, NULL, "exp", "exp",
																			NULL);
										}
									;

rel_operator			: T_EQ
										{$$ = "=";}
									| T_INBET
										{$$ = "<>";}
									| T_GTHAN
										{$$ = ">";}
									| T_LTHAN
										{$$ = "<";}
									| T_GEQ
										{$$ = ">=";}
									| T_LEQ
										{$$ = "<=";}
									;

unary_operator		: T_ADD
										{$$ = "+";}
									| T_SUB
										{$$ = "-";}
									;

add_sub_or				: T_ADD
										{$$ = "+";}
									| T_SUB
										{$$ = "-";}
									| T_OR
										{$$ = "OR";}
									;

other_op					: T_MUL
										{$$ = "*";}
									| T_DIV
										{$$ = "DIV";}
									| T_MOD
										{$$ = "MOD";}
									| T_AND
										{$$ = "ADD";}
									| T_SHL
										{$$ = "SHL";}
									| T_SHR
										{$$ = "SHR";}
									;

identifier				: simple_name
										{$$ = $1;}
									| simple_name T_LPAREN expression T_RPAREN
										{
											$$ = initializeTreeBranch();
											setTreeBranch($$, $1, $3, NULL, NULL, NULL, NULL,
																		NULL);
										}
									;

constant					: T_NUM
										{
											$$ = initializeTreeBranch();
											setTreeBranch($$, NULL, NULL, NULL, NULL, "exp", "id",
																		/*to be added later*/NULL);
										}
									;

/*variable				: simple_name T_LPAREN expression T_RPAREN
									| simple_name
									;*/

simple_name				: T_ID
										{
											$$ = initializeTreeBranch();
											setTreeBranch($$, NULL, NULL, NULL, NULL, "exp", "id",
																		/*to be added later*/NULL);
										}
									;
/*
setTreeBranch(*Branch, *cA, *cB, *cC, *sib, type, subtype, attribute);
*/
%%

int parse ()
{
    if (yyparse() == 0)
			printf ("Parse succeeded: %d\n", result);
    else
	 		printf ("Parse failed.\n");
		if(verboflag > 1)
		{
			//printTree();
		}
		return 0;
}

static int yylex(void)
{ return getToken(); }


void yyerror(char const *str)
{
    printf ("%s\n", str);
}
