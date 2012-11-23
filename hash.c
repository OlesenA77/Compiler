/*Alexander Olesen
 *1671836
 *CMPT 399
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "hash.h"


/*
 * Initialize a new Hash Table
 * Returns a pointer to the new table
 */
HashTable *hashInit(int size)
{
	int i=0;
	HashTable *Table = malloc(sizeof(HashTable));
	if (Table == NULL)
	{
		fprintf(stderr, "Out of Memory on symbol table initialization");
		exit(1);
	}
	Table->elements = malloc(sizeof(HashElement) * size);
	if (Table->elements == NULL)
	{
		fprintf(stderr, "Out of Memory on symbol table initialization");
		exit(1);
	}
	Table->size = size;
	Table->MaxOffset = 0;
	Table->next = NULL;
	Table->parent = NULL;
	while(i<size)
	{
		Table->elements[i].next = NULL;
		Table->elements[i].identifier = NULL;
		Table->elements[i].val = 0;
		Table->elements[i].indices = 0;
		Table->elements[i].memalloc = 0;
		Table->elements[i].address = 0;
		Table->elements[i].offset = 0;
		Table->elements[i].type = NA;
		i++;
	}
	
	return Table;
}
/*
 * Set the parent/child relation between two
 * Hash Tables
 * a is the parent of b
 * the next field is added as a linked list of hash tables 
 * in the exact order that they are created
 * this allows code generation to move from each symbol table
 * in order, see codeGen.c for implementation
 */
void setRelation(HashTable *a, HashTable *b)
{
	b->parent = a;
	HashTable *ptr = a;
	while(ptr->next != NULL)
	{
	    ptr = ptr->next;
	}
	ptr->next = b;
}
 
/*
 * Increases the size of a hash table, simply by doubling it
 */
HashTable *growTable(HashTable *Table)
{
	int i;
	HashElement *ptr;
	HashTable *newTable = hashInit(Table->size+INIT_TABLE_SIZE);
	for(i = 0; i<Table->size; i++)
	{
		if(Table->elements[i].identifier != NULL)
		{
			add(Table->elements[i].identifier, Table->elements[i].type,
				 Table->elements[i].indices, newTable);
			ptr = Table->elements[i].next;
			while (ptr != NULL)
			{
				add(ptr->identifier, ptr->type, ptr->indices, newTable);
				ptr = ptr->next;
			}
		}
	}
	free(Table);
	return newTable;
}
/*
 * Generates a hash number from the string in
 * by adding up the numerical values of the 
 * chars, adding their index, then dividing by
 * the size of the Table it is being inserted in
 */
int hash(char *in, int size)
{
	
	int sum = 0;
	int lng = strlen(in);
	int i;
	for(i=0; i<lng; i++)
	{
		sum += ((int)(in[i]+i));/* solves ba = ab */
	}
	
	sum = sum % size;
	return(sum);
}

/*
 * adds an element to a hash table
 * returns 0 if the element is already added
 * returns hash number if the identifier has already been added
 */
int add(char *in, TYPE type, int numIndices, HashTable *Table)
{
	int tmp = hash(in, Table->size);
	/*no collisions*/
	if(Table->elements[tmp].identifier == NULL)
	{
		Table->elements[tmp].identifier = malloc(sizeof(char)*strlen(in));
		strcpy(Table->elements[tmp].identifier, in);
		Table->elements[tmp].type = type;
		Table->elements[tmp].indices = numIndices;
		return tmp;
	}
	/*collision resolution*/
	else
	{
		if(strcmp(in, Table->elements[tmp].identifier) == 0)
		{
			return 0;
		}
		HashElement *ptr = &Table->elements[tmp];
		while(ptr->next != NULL)
		{
			ptr = ptr->next;
			if(strcmp(in, ptr->identifier) == 0)
			{
			  return 0;
			}	
		}
		ptr->next = malloc(sizeof(HashElement));
		if(ptr->next == NULL){
		    fprintf(stderr, "Out of Memory Error");
		    exit(1);
		}
		ptr->next->identifier = malloc(sizeof(char)*strlen(in));
		if(ptr->next->identifier == NULL){
		    fprintf(stderr, "Out of Memory Error");
		    exit(1);
		}
		strcpy(ptr->next->identifier, in);
		ptr->next->indices = numIndices;
		ptr->next->type = type;
		return tmp;
	}	
}

/*
 * Prints out a given hash table
 */
void printTable(HashTable *Table)
{
  int TableNum = 1;
  while(Table != NULL)
  {
	printf("\n\nSymbol Table: %d\n", TableNum);
	int i=0;
	HashElement *ptr;
	while(i<Table->size)
	{
		printf("_________________________________________\n");
		printf("%d ", i+1);
		if(Table->elements[i].identifier != NULL)
		{
			printf("| %s", Table->elements[i].identifier);
			printf("=%s", printType(Table->elements[i].type));
			printf("(%d)", Table->elements[i].indices);
			ptr = Table->elements[i].next;
			while (ptr != NULL)
			{
				printf("-> %s", ptr->identifier);
				printf("=%s", printType(ptr->type));
				printf("(%d)", Table->elements[i].indices);
				ptr = ptr->next;
			}
		}
		printf("\n");
		i++;
	}
  Table = Table->next;
  TableNum++;
  }	
}

/*
 * Helper function for the printTable function that
 * prints out the string related to the enumerated
 * type
 */ 
char *printType(TYPE type)
{
	if(type == cons_int)
		return "cons_int";
	if(type == var_int)
		return "var_int";
	if(type == arr_int)
		return "arr_int";
	if(type == boolean)
		return "boolean";
	return "Error";
}
/*
 * Searches the symbol table for an identifier
 * Returns 0 if the entry exists in the Table
 * Returns 1 if the entry does not exist
 */
HashElement *lookUpId(char *in, HashTable *Table)
{
  HashElement *ret = NULL;/*entry does not exist until found*/
  if(Table->parent != NULL)
  {
      ret = lookUpId(in, Table->parent);
  }
  /*find integers and constants*/
  int tmp = hash(in, Table->size);
  if(Table->elements[tmp].identifier != NULL)
  {
    HashElement *ptr = &Table->elements[tmp];
    if(strcmp(ptr->identifier, in) == 0)
    {
      ret = ptr;/*success!!*/
      return ret;
    }
    while(ptr != NULL)
    {
      if(strcmp(ptr->identifier, in) == 0)
      {
        ret = ptr;/*success!*/
        return ret;
      }
      ptr = ptr->next;
    }
  }
  return ret;
}
















