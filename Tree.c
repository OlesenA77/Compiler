/*Alexander Olesen
 *1671836
 *CMPT 399
 */

#include "Tree.h"
#include "lex.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>

/*strings to be used for verbose token returns*/
  static char *TOKENS[] = {
  "ENDFILE",
	"ERROR",
  "AND   ", "OR    ", "NOT   ",
  "IF    ", "THEN  ", "ELSE  ",
  "WHILE ", "DO    ",
  "BEGIN ", "END   ",
  "CASE  ",
  "ARRAY ", "INT   ", "CONS  ", "VAR   ",
  "OF    ",
  "READ  ", "WRITE ",
  "INBET ",
  "ADD   ", "SUB   ", "MUL   ", "DIV   ", "MOD   ", "SHL   ", "SHR   ",
  "EQUAL ", "GTHAN ", "LTHAN ", "GEQ   ", "LEQ   ",
  "LPAREN", "RPAREN", "COMMA ", "PERIOD",
  "COLON ", "SEMCLN", "ASSIGN",
  "NUM   ",
  "ID    "
    }; 

/*initialize a new Tree Branch for use in parse*/
TreeBranch *initializeTreeBranch()
{
	TreeBranch *Branch = (TreeBranch *) malloc(sizeof(TreeBranch));
	if (Branch == NULL)
	{
		printf("Out of Memory error at line %d", lineCount);
		exit(1);
	}
	Branch->child1 = NULL;	
	Branch->child2 = NULL;
	Branch->child3 = NULL;
	Branch->sibling = NULL;
	Branch->type = NULL;
	Branch->subtype = NULL;
	Branch->attribute = NULL;
	Branch->num = 0;
	Branch->flag = 0;
}


/*Any value can be set to NULL when the function is called*/
/*Sets a new Tree Branch with a char * attribute*/
void setTreeBranch(TreeBranch *Branch, TreeBranch *childA, TreeBranch *childB,
										 TreeBranch *childC, TreeBranch *sib, char *_type,
												char *_subtype, char *_attribute)
{
	Branch->child1  = childA;
	Branch->child2  = childB;
	Branch->child3  = childC;
	Branch->sibling = sib;
	if(_type != NULL)
	{
		Branch->type = malloc(sizeof(char)*MAX);
		if(Branch->type == NULL)
		{
			printf("out of memory error at line %d", lineCount);
			exit(1);
		}
		strncpy(Branch->type, _type, MAX);
	}
	if(_subtype != NULL)
	{
		Branch->subtype = malloc(sizeof(char)*MAX);
		if(Branch->subtype == NULL)
		{
			printf("out of memory error at line %d", lineCount);
			exit(1);
		}
		strncpy(Branch->subtype, _subtype, MAX);
	}	
	if(_attribute != NULL)	
	{
		Branch->attribute = malloc(sizeof(char)*MAX);
		if(Branch->attribute == NULL)
		{
			printf("out of memory error at line %d", lineCount);
			exit(1);
		}
		strncpy(Branch->attribute, _attribute, MAX);
	}
}
/*Any value can be set to NULL when the function is called*/
/*Sets a new Tree Branch with a char * attribute*/
void setTreeBranchNUM(TreeBranch *Branch, TreeBranch *childA,
TreeBranch *childB, TreeBranch *childC, TreeBranch *sib, char _type[],
												char _subtype[], int _num)
{
	Branch->child1  = childA;
	Branch->child2  = childB;
	Branch->child3  = childC;
	Branch->sibling = sib;
	if(_type != NULL)
	{
		Branch->type = malloc(sizeof(char)*MAX);
		if(Branch->type == NULL)
		{
			printf("out of memory error at line %d", lineCount);
			exit(1);
		}
		strncpy(Branch->type, _type, MAX);
	}
	if(_subtype != NULL)
	{
		Branch->subtype = malloc(sizeof(char)*MAX);
		if(Branch->subtype == NULL)
		{
			printf("out of memory error at line %d", lineCount);
			exit(1);
		}
		strncpy(Branch->subtype, _subtype, MAX);
	}	
	Branch->num = _num;
	Branch->flag = 1;
}
/*==============================================
Prints the Tree
==============================================*/
void printTree(TreeBranch * cur, int indent)
{
	int i = 0;
	while(i < indent)								
	{
		printf(" ");
		i++;
	}
	if(cur->type != NULL)
		printf("%s ", cur->type);
	if(cur->subtype != NULL)
		printf("%s ", cur->subtype);
	if(cur->attribute != NULL)
		printf("[%s]\n", cur->attribute);
	else if(cur->flag == 1)
		printf("%i\n", cur->num);
	else printf("\n");

	if(cur->child1 != NULL)
		printTree(cur->child1, (indent+2));
	if(cur->child2 != NULL)
		printTree(cur->child2, (indent+2));
	if(cur->child3 != NULL)
		printTree(cur->child3, (indent+2));
  if(cur->sibling != NULL)
		printTree(cur->sibling, indent);
	
	return;
}












