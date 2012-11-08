#include "analyze.h"
#include "tree.h"
#include "globals.h"
#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/*
 * checks for semantic errors using the abstract syntax tree
 * will walk the entire tree checking the following:
 * 
 - checking for multiple declarations of the same identifier
 ==> This is checked when the identifier is added to the 
 symbol table
 Will also be checked again if there are multiple symbol
 tables
 
 *
 */
HashTable *SymTable;

int analyze(TreeBranch *cur)
{
  SymTable = hash_init(INIT_TABLE_SIZE);
  analyzer(cur);
  printTable(SymTable);
  return 0;
}

/*
 * recursive function to walk the tree and do the semantic checks
 */
int analyzer(TreeBranch *cur)
{
  int skip1 = 0;
  int skip2 = 0;
  int skip3 = 0;
	
  if(cur->nodeT == const_decl)
    {
      if(cur->child1->attribute != NULL)
      {
      	char *tmp = malloc(sizeof(char)*strlen(cur->child1->attribute));
      	strcpy(tmp, cur->child1->attribute);
      	fprintf(stderr, "%s\n", tmp);
      	add(tmp, cons_int, SymTable);
      	fprintf(stderr, "add succeeded");
      	free(tmp);
      }
    }
	
  if(cur->child1 != NULL && skip1 == 0)
    analyzer(cur->child1);
  if(cur->child2 != NULL && skip2 == 0)
    analyzer(cur->child2);
  if(cur->child3 != NULL && skip3 == 0)
    analyzer(cur->child3);
  if(cur->sibling != NULL)
    analyzer(cur->sibling);
		
  return 0;
}	
