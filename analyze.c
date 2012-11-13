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
 
 - checking for indexed array calls

 - checking that arrays are not indexed by boolean expressions
 
 - checking for variables on the left hand side of assignment
   statements
 
 - checking for boolean conditions for if and while statements
 
 - 
 *
 */
HashTable *currentTable;

/*
 * Calls the recursive tree traversal
 * and checks return values
 */
HashTable *analyze(TreeBranch *cur)
{
  int ret = analyzer(cur, 0);
  if(ret == 1)
    fprintf(stderr, "Compilation failed due to semantic errors\n");
  while(currentTable->parent != NULL)
    currentTable = currentTable->parent;
  return currentTable;
}

/*
 * recursive function to walk the tree and do the semantic checks
 * returns 0 on success
 * returns 1 if encountering semantic errors
 */
int analyzer(TreeBranch *cur, int retval)
{
  /*To skip recursive calls*/
  int skip1 = 0;
  int skip2 = 0;
  int skip3 = 0;
  int skipSib = 0;
  
  /*
   *=================================================================
   * Block Statements
   * Create a new scope
   * and terminate it when out of it
   *=================================================================
   */
  if(cur->nodeT == new_blk)
    {
      if(currentTable == NULL)
	   currentTable = hashInit(INIT_TABLE_SIZE);
      else
	  {
  	    /*
  	     * Set a new scope and continue traversal
  	     * Within the new block
  	     */
  	     HashTable *new = hashInit(INIT_TABLE_SIZE);
  	     setRelation(currentTable, new);
  	     currentTable = currentTable->next;
  	     if(cur->child1 != NULL)
	        retval = analyzer(cur->child1, retval);
	     if(cur->child2 != NULL)
	        retval = analyzer(cur->child2, retval);
	     if(cur->child3 != NULL)
	        retval = analyzer(cur->child3, retval);
	     /*
	      * disregard new scope for further statements within
	      * the parent scope
	      */
	     currentTable = currentTable->parent; 
	     if(cur->sibling != NULL)
	        retval = analyzer(cur->sibling, retval);
	     skip1 = 1;
	     skip2 = 1;
	     skip3 = 1;
	     skipSib = 1;
  	  }
    }
  /*
   *=================================================================
   * Constant Declarations
   *=================================================================
   */
  else if(cur->nodeT == const_decl)
    {
      if(cur->child1->attribute != NULL)
	  {
	    int tmp=add(cur->child1->attribute, cons_int, 1, currentTable);
	    if(tmp == 1)
	    {
	      fprintf(stderr, "ERROR: variable %s declared multiple times.\n",
		      cur->child1->attribute);
	      retval = 1;
	    }
	    skip1 = 1;
	    skip2 = 1;
	    skip3 = 1;
	  }
    }
  /*
   *=================================================================
   * Variable Declarations
   *=================================================================
   */
  else if(cur->nodeT == var_decl)
  {
    if(cur->child1->attribute != NULL)
    {
      if(cur->child1->attribute != NULL)
	  {
	    /*Non-Array Declarations*/
	    if(cur->child2->nodeT == sing_decl)
	    {
	      TreeBranch *ptr = cur->child1->sibling;
	      add(cur->child1->attribute, var_int, 1, currentTable);
	      while(ptr != NULL)
		  {
		    add(ptr->attribute, var_int, 1, currentTable);
		    ptr = ptr->sibling;
		  }	
	      skip1 = 1;
	      skip2 = 1;
	      skip3 = 1;
	    }
	  /*Array Declarations*/
	  if(cur->child2->nodeT == arr_decl)
	    {
	      TreeBranch *ptr = cur->child1->sibling;
	      add(cur->child1->attribute, arr_int, cur->child2->child1->num,
		  currentTable);
	      while(ptr != NULL)
		  {
		    add(ptr->attribute, arr_int, cur->child2->child1->num,
		      currentTable);
		    ptr = ptr->sibling;
		  }	
	      skip1 = 1;
	      skip2 = 1;
	      skip3 = 1;
	    }
	  }  
	}
  }
  /*
   *=================================================================
   * Lookups for constants and variables
   *=================================================================
   */
  if(cur->nodeT == Id)
  {
    HashElement *tmp = lookUpId(cur->attribute, currentTable);
    if(tmp == NULL)
    {
      fprintf(stderr, "Error: identifier %s undeclared\n", cur->attribute);
      retval = 1;
    }
    if(tmp->indices > 1)
    {
      fprintf(stderr, "Error: array %s not indexed when called\n",
            cur->attribute);
      retval = 1;
    }
    
    tmp = NULL;
    
  }
  /*
   *=================================================================
   * Lookups for arrays of integers
   *=================================================================
   */
  if(cur->nodeT == arrId)
  {
    HashElement *tmp = lookUpId(cur->child1->attribute, currentTable);
    if(tmp == NULL)
    {
      fprintf(stderr, "Error: array identifier %s undeclared\n",
            cur->child1->attribute);
      retval = 1;
    }
    if(cur->child2->nodeT == Boolean)
    {
      fprintf(stderr, "Error: array %s cannot be indexed by Boolean expression\n",
            cur->child1->attribute);
      retval = 1;
    }      
    tmp = NULL;
  }
  /*
   *=================================================================
   * Type Checking of nodes
   *=================================================================
   */
     /*
      * Checking for a variable on the left hand side of an
      * assignment
      */
   if(cur->nodeT == assign)
   {
     HashElement *tmp;
     if(cur->child1->attribute != NULL)
       tmp = lookUpId(cur->child1->attribute, currentTable);
     else
       tmp = lookUpId(cur->child1->child1->attribute, currentTable);  
    
     if(tmp->type == cons_int)
     {
       fprintf(stderr, "Error, left-val %s of assignment is a constant\n", 
           tmp->identifier);
       retval = 1;
     }
     tmp = NULL;
   }
   
     /*
      * Checking for a boolean type expression with if statements
      */
   if(cur->nodeT == if_stmt)
   {
     if(cur->child1->nodeT != Boolean)
     {
        fprintf(stderr, "Error, if statement not followed by boolean condition\n");    
        retval = 1;
     }
   }
     /*
      * Checking for a boolean type expression with while statements
      */
   if(cur->nodeT == while_stmt)
   {
     if(cur->child1->nodeT != Boolean)
     {
       fprintf(stderr, "Error, while statement not followed by boolean condition\n");
       retval = 1;
     }
   }

  /*
   * Move onto other nodes
   */

  if(cur->child1 != NULL && skip1 == 0)
    retval = analyzer(cur->child1, retval);
  if(cur->child2 != NULL && skip2 == 0)
    retval = analyzer(cur->child2, retval);
  if(cur->child3 != NULL && skip3 == 0)
    retval = analyzer(cur->child3, retval);
  if(cur->sibling != NULL && skipSib == 0)
    retval = analyzer(cur->sibling, retval);
		
  return retval;
}	
