/*Alexander Olesen
 *1671836
 *CMPT 399
 */

%{
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include "tree.h"
#define  YYERROR_VERBOSE
#define  YYDEBUG 1
int result;
static int yylex ();
void yyerror(char const *);
TreeBranch *head;
static TreeBranch *tmp;	
%}
%union {
	char * string;
	int	num;
	struct _TreeBranch *Branch;
	}
/*comment on the comments, these do not reference the
  actual values bison assigns tokens*/
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
%token T_READ    //19
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
%token <num>	T_NUM     //41
%token <string> T_ID      //42
%token T_NL      //43

%nonassoc LOWER_THAN_ELSE
%nonassoc T_ELSE

%debug
%locations

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
%type <string> add_sub
%type <string> or
%type <string> other_op
%type <string> and

%type <Branch> identifier
%type <Branch> constant
%type <Branch> simple_name
%%

program			: block T_PERIOD
			  {head = $1;}
		        ;

block 			: const_section var_section T_BEGIN statement_list T_END
			  {
			    $$ = initializeTreeBranch();
			    setTreeBranch($$, $1, $2, $4, NULL, "stmt", "block", 
					  NULL);
			  setEnum($$, new_blk);
              }
                        ;

const_section 		: T_CONS const_item
                          {$$ = $2;}
                        | {$$ = NULL;}
                        ;

const_item	  : const_item const_decl
                {
			    	tmp = $1;
			    	while(tmp->sibling != NULL)
			      	{
						tmp = tmp->sibling;
			      	}
			    	tmp->sibling = $2;
			  	}
              | const_decl
			  	{$$ = $1;}
              ;

const_decl	    : identifier T_ASSIGN constant T_SEMCLN
                  {
			      	$$ = initializeTreeBranch();
			    	setTreeBranch($$, $1, $3, NULL, NULL, "const", "decl",
					  NULL);
					setEnum($$, const_decl);
			      }
                ;

var_section  		: T_VAR var_decls
                          {$$ = $2;}
                        | {$$ = NULL;}
                        ;

var_decls		: var_decls var_decl
                {
              		TreeBranch *tmp = $1;
              		while(tmp->sibling)
              		tmp = tmp->sibling;
              	  
              		tmp->sibling = $2;
              	  
			   		$$ = $1;
			  	}
                | var_decl
			    {
			    	$$ = initializeTreeBranch();
			    	setTreeBranch($$, $1, NULL, NULL, NULL, "sect", "var",
					  NULL);
			  	}
                        ;
var_decl     : var_list T_COLON type T_SEMCLN
             {
			    $$ = initializeTreeBranch();
			    setTreeBranch($$, $1, $3, NULL, NULL, "var", "decl",
					  NULL);
				setEnum($$, var_decl);
			  }
              ; 

var_list      	: var_list T_COMMA simple_name
                 {
              		TreeBranch *tmp = $1;
              		while(tmp->sibling)
              		tmp = tmp->sibling;
              	  
              		tmp->sibling = $3;
              	  
			   		$$ = $1;
			  		}
                | simple_name
                ; 

type		   	: T_ARRAY T_LPAREN constant T_RPAREN T_OF T_INT
                {
			    	$$ = initializeTreeBranch();
			    	setTreeBranch($$, $3, NULL, NULL, NULL, "type",
			    				"array","int");
					setEnum($$, arr_decl);
			  	}
                | T_INT
			  	{
			    	$$ = initializeTreeBranch();
			    	setTreeBranch($$, NULL, NULL, NULL, NULL, "type", 							"int",
					  NULL);
					setEnum($$, sing_decl);
			  	}
                ; 

statement_list	: statement_list T_SEMCLN statement
                {
			    	tmp = $1;
			    	while(tmp->sibling != NULL)
			      	{
						tmp = tmp->sibling;
			      	}
			    	tmp->sibling = $3;
			   		$$ = $1;
			  	}
                | statement
		        {$$ = $1;}
                ; 

statement	    : null_statement
                {$$ = $1;}
                | assign_statement
		        {$$ = $1;}
                | block_statement
		        {$$ = $1;}
                | if_statement
		        {$$ = $1;}
                | while_statement
		        {$$ = $1;}
                | case_statement
		        {$$ = $1;}
                | write_statement
		        {$$ = $1;}
                | read_statement
		        {$$ = $1;}
                ;

null_statement  : 
                {
                  $$ = initializeTreeBranch();
			      setTreeBranch($$, NULL, NULL, NULL, NULL, "stmt", "null",
					  NULL);
                }
                ;

assign_statement: identifier T_ASSIGN expression
                {
			      $$ = initializeTreeBranch();
			      setTreeBranch($$, $1, $3, NULL, NULL, "stmt", "assign",
					  NULL);
			      setEnum($$, assign);
			  	}
                ;

block_statement : block
                  {$$ = $1;}
                ;

if_statement  : T_IF condition T_THEN statement %prec LOWER_THAN_ELSE
              {
			    $$ = initializeTreeBranch();
			    setTreeBranch($$, $2, $4, NULL, NULL, "stmt", "if",
					  NULL);
			    setEnum($$, if_stmt);
			  }
              | T_IF condition T_THEN statement T_ELSE statement
			  {
			    $$ = initializeTreeBranch();
			    setTreeBranch($$, $2, $4, $6, NULL, "stmt", "if",
					  NULL);
			  }
              ;

while_statement : T_WHILE condition T_DO statement
                {
			      $$ = initializeTreeBranch();
			      setTreeBranch($$, $2, $4, NULL, NULL, "stmt", "while",
					  NULL);
				  setEnum($$, while_stmt);
			    }
                ;

condition		: expression
                  {$$ = $1;}
                ;

case_statement : T_CASE expression T_OF case_list T_ELSE statement T_END
               {
			     $$ = initializeTreeBranch();
			     setTreeBranch($$, $2, $4, $6, NULL, "stmt", "case",
					  NULL);
			   }
               | T_CASE expression T_OF case_list T_END
			   {
			     $$ = initializeTreeBranch();
			     setTreeBranch($$, $2, $4, NULL, NULL, "stmt", "case",
					  NULL);
			   }
               ;

case_list     : case_list T_SEMCLN case
              {
			    tmp = $1;
			    while(tmp->sibling != NULL)
			      {
				tmp = tmp->sibling;
			      }
			    tmp->sibling = $3;
			    $$ = $1;
			  }
              | case
			  {
			    $$ = $1;
			  }
              ;

case	      : const_list T_COLON statement
              {
			    $$ = initializeTreeBranch();
			    setTreeBranch($$, $1, $3, NULL, NULL, NULL, "case",
					  NULL);
			  }
              ;

const_list	  : const_list T_COMMA constant
              {
			    $$ = initializeTreeBranch();
			    setTreeBranch($$, $3, NULL, NULL, NULL, "list", "const",
					  NULL);
			    while($1->sibling != NULL)
			      {	
				$1 = $1->sibling;
			      }
			    $1->sibling = $$;
			  }
              | constant
			  {
			    $$ = initializeTreeBranch();
			    setTreeBranch($$, $1, NULL, NULL, NULL, "list", "const",
					  NULL);
			  }
              ;

write_statement : T_WRITE T_LPAREN write_list T_RPAREN
                {
                  $$ = initializeTreeBranch();
			      setTreeBranch($$, $3, NULL, NULL, NULL, "stmt", 
			    	     "write", NULL);
			    }
                ;

write_list    : write_list T_COMMA expression
              {
              	TreeBranch *tmp = $1;
              	while(tmp->sibling)
              		tmp = tmp->sibling;
              	  
              	tmp->sibling = $3;
              	  
			   	$$ = $1;
			  }
              | expression
              ;
                

read_statement		: T_READ T_LPAREN read_list T_RPAREN
                      {
                        $$ = initializeTreeBranch();
			    	    setTreeBranch($$, $3, NULL, NULL, NULL, "stmt", 
			    	          "read", NULL);
					  }
                      ;

read_list	    : read_list T_COMMA identifier
                {
                  TreeBranch *tmp = $1;
                  while(tmp->sibling)
              	  tmp = tmp->sibling;
              	  
              	  tmp->sibling = $3;
              	  
			   	  $$ = $1;
			  	}
                | identifier
                ;
                

expression		: simple_expression rel_operator simple_expression
                {
			      $$ = initializeTreeBranch();
			      setTreeBranch($$, $1, $3, NULL, NULL, "exp", "rel",
					  $2);
				  setEnum($$, Boolean);
			    }
                | simple_expression
			      {$$ = $1;}
                ;

simple_expression	: unary_operator term operator_term
                    {
			          $$ = initializeTreeBranch();
			          setTreeBranch($$, $2, $3, NULL, NULL, "exp", "sim",
					              NULL);
			        }
                    | term operator_term
			        { 
			          $$ = initializeTreeBranch();
			          setTreeBranch($$, $1, $2, NULL, NULL, "exp", "sim",
					              NULL);
			        }
			        | unary_operator term
			        {
			          $$ = initializeTreeBranch();
			          setTreeBranch($$, $2, NULL, NULL, NULL, "exp", "sim",
					             NULL);
			        }
                    | term
			        {$$ = $1;}
                    ;

operator_term		: add_sub term operator_term
                    {
			          $$ = initializeTreeBranch();
			          setTreeBranch($$, $2, NULL, NULL, NULL, "exp", "op",
					             $1);
			          while($3->sibling != NULL)
			          {	
				        $3 = $3->sibling;
			          }
			        $$->sibling = $3;
			        }
                    | add_sub term
			        {
			          $$ = initializeTreeBranch();
			          setTreeBranch($$, $2, NULL, NULL, NULL, "exp", "op",
					               $1);
			        }
			        | or term
			        {
			          $$ = initializeTreeBranch();
			          setTreeBranch($$, $2, NULL, NULL, NULL, "exp", "op",
					               $1);
					  setEnum($$, expOr);
			        }
                    ;

term			: term other_op factor
                {
			      $$ = initializeTreeBranch();
			      setTreeBranch($$, $1, $3, NULL, NULL, "exp", "op",
					         $2);
			    }
			    | term and factor
			    {
			      $$ = initializeTreeBranch();
			      setTreeBranch($$, $1, $3, NULL, NULL, "exp", "op",
					         $2);
			      setEnum($$, expAnd);
			    }
                | factor
			      {$$ = $1;}
                ;

factor 			: constant
                {
			       $$ = $1;
			    }
                | identifier
			    {
			       $$ = $1;
			    }
                | T_NOT factor
		        {
			       $$ = initializeTreeBranch();
			       setTreeBranch($$, $2, NULL, NULL, NULL, "exp", "NOT",
					 NULL);
			    }
			    | T_LPAREN expression T_RPAREN
		        {
			      $$ = initializeTreeBranch();
			      setTreeBranch($$, $2, NULL, NULL, NULL, "exp", "( )",
					 NULL);
			    }
                ;

rel_operator	       : T_EQ
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

unary_operator	       : T_ADD
                         {$$ = "+";}
                       | T_SUB
		         		 {$$ = "-";}
                       ;

add_sub	               : T_ADD
                         {$$ = "+";}
                       | T_SUB
		         	     {$$ = "-";}
                       ;
                       
or                     : T_OR
                         {$$ = "OR";}
                       ;
other_op	       : T_MUL
                       {$$ = "*";}
                     | T_DIV
		         	   {$$ = "DIV";}
                     | T_MOD
		               {$$ = "MOD";}
                     | T_SHL
		               {$$ = "SHL";}
                     | T_SHR
		               {$$ = "SHR";}
                     ;

and                  : T_AND
                       {$$ = "AND";}
                     ;
identifier	       : simple_name
                     {
                     $$ = $1;
                     setEnum($$, Id);
                     }
                   | simple_name T_LPAREN expression T_RPAREN
		             {
			        	$$ = initializeTreeBranch();
			         	setTreeBranch($$, $1, $3, NULL, NULL, "array",
			         	 NULL, NULL);
			         	setEnum($$, arrId); 
				 	 }
                   ;

constant	       : T_NUM
                   	 {
			   		 	$$ = initializeTreeBranch();
			    	 	setTreeBranchNUM($$, NULL, NULL, NULL, NULL, 							  "const", NULL, $1);
			 		 }
                   ;


simple_name	       : T_ID
                     {
			   		 	$$ = initializeTreeBranch();
			         	setTreeBranch($$, NULL, NULL, NULL, NULL, "name", 							  NULL, $1);
			         }
                   ;

%%
int parse ()
{
    if (yyparse() != 0)
	 		fprintf(stderr, "Compilation failed due to Syntax Errors.\n");
	return result;
}

/*function to return the head of the abstract syntax tree*/
TreeBranch *retHead(){
	TreeBranch *ptr = head;
	return ptr;
}

static int yylex(void)
{ return getToken(); }


void yyerror(char const *str)
{
    printf ("%s\n", str);
}
