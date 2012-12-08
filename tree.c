/*Alexander Olesen
 *1671836
 *CMPT 399
 */

#include "tree.h"
#include "lex.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>

/*initialize a new Tree Branch for use in parse*/
TreeBranch *initializeTreeBranch()
{
	TreeBranch *Branch = (TreeBranch *) malloc(sizeof(TreeBranch));
	if (Branch == NULL)
	{
		fprintf(stderr, "Out of Memory error at line %d", lineCount);
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
	Branch->nodeT = O;
	return Branch;
}


/*Any value can be set to NULL when the function is called*/
/*Sets a new Tree Branch with a char * attribute*/
void setTreeBranch(TreeBranch *Branch, TreeBranch *childA, TreeBranch *childB,
										 TreeBranch *childC, TreeBranch *sib, char *_type,
												char *_subtype, char *_attribute)
{
	int tmp;
	Branch->child1  = childA;
	Branch->child2  = childB;
	Branch->child3  = childC;
	Branch->sibling = sib;
	if(_type != NULL)
	{
		tmp = strlen(_type);
		Branch->type = malloc(sizeof(char)*tmp);
		if(Branch->type == NULL)
		{
			printf("out of memory error at line %d", lineCount);
			exit(1);
		}
		strncpy(Branch->type, _type, tmp);
	}
	if(_subtype != NULL)
	{
		tmp = strlen(_subtype);
		Branch->subtype = malloc(sizeof(char)*tmp);
		if(Branch->subtype == NULL)
		{
			printf("out of memory error at line %d", lineCount);
			exit(1);
		}
		strncpy(Branch->subtype, _subtype, tmp);
	}	
	if(_attribute != NULL)	
	{
		tmp = strlen(_attribute);
		Branch->attribute = malloc(sizeof(char)*tmp);
		if(Branch->attribute == NULL)
		{
			printf("out of memory error at line %d", lineCount);
			exit(1);
		}
		strncpy(Branch->attribute, _attribute, tmp);
	}
}
/*Any value can be set to NULL when the function is called*/
/*Sets a new Tree Branch with a char * attribute*/
void setTreeBranchNUM(TreeBranch *Branch, TreeBranch *childA,
TreeBranch *childB, TreeBranch *childC, TreeBranch *sib, char _type[],
												char _subtype[], int _num)
{
	int tmp;
	Branch->child1  = childA;
	Branch->child2  = childB;
	Branch->child3  = childC;
	Branch->sibling = sib;
	if(_type != NULL)
	{
		tmp = strlen(_type);
		Branch->type = malloc(sizeof(char)*MAX);
		if(Branch->type == NULL)
		{
			printf("out of memory error at line %d", lineCount);
			exit(1);
		}
		strncpy(Branch->type, _type, tmp);
	}
	if(_subtype != NULL)
	{
		tmp = strlen(_subtype);
		Branch->subtype = malloc(sizeof(char)*tmp);
		if(Branch->subtype == NULL)
		{
			printf("out of memory error at line %d", lineCount);
			exit(1);
		}
		strncpy(Branch->subtype, _subtype, tmp);
	}	
	Branch->num = _num;
	Branch->flag = 1;
}

/*set enumerated type*/
void setEnum(TreeBranch *Branch, node_T T)
{
	Branch->nodeT = T;
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
    {
	    printf("child1:  ");
		printTree(cur->child1, (indent+2));
	}
	if(cur->child2 != NULL)
	{
	    printf("child2:  ");
		printTree(cur->child2, (indent+2));
	}
	if(cur->child3 != NULL)
	{
	    printf("child3:  ");
		printTree(cur->child3, (indent+2));
    }
    if(cur->sibling != NULL)
	{
	    printf("sibling: ");
		printTree(cur->sibling, indent);
	}
	return;
}












