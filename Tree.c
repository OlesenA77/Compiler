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
}


/*Any value can be set to NULL when the function is called*/
void setTreeBranch(TreeBranch *Branch, TreeBranch *childA, TreeBranch *childB,
										 TreeBranch *childC, TreeBranch *sib, char _type[],
												char _subtype[], char _attribute[])
{
	Branch->child1  = childA;
	Branch->child2  = childB;
	Branch->child3  = childC;
	Branch->sibling = sib;
	if(_type != NULL)
		setString(Branch->type, _type);
	if(_subtype != NULL)
		setString(Branch->subtype, _subtype);
	if(_type != NULL)	
	setString(Branch->attribute, _attribute);
}


void setString(char *des, char *src)
{
	des = malloc(sizeof(char)*MAX);
	if(des == NULL)
	{
		printf("out of memory error at line %d", lineCount);
		exit(1);
	}
	strncpy(des, src, MAX);
	return;
}

void PrintTree(TreeBranch * t)
{
	return;
}












