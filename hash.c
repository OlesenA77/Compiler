
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
 */
void setRelation(HashTable *a, HashTable *b)
{
	a->next = b;
	b->parent = a;
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
 * returns 0 if the element is added to the table
 * returns 1 if the identifier has already been added
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
		return 0;
	}
	/*collision resolution*/
	else
	{
		if(strcmp(in, Table->elements[tmp].identifier) == 0)
		{
			return 1;
		}
		HashElement *ptr = &Table->elements[tmp];
		while(ptr->next != NULL)
		{
			ptr = ptr->next;
			if(strcmp(in, ptr->identifier) == 0)
			{
			  return 1;
			}	
		}
		ptr->next = malloc(sizeof(HashElement));
		ptr->next->identifier = malloc(sizeof(char)*strlen(in));
		strcpy(ptr->next->identifier, in);
		ptr->next->indices = numIndices;
		ptr->next->type = type;
		return 0;
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
			if(Table->elements[i].indices > 1)
					printf("(%d)", Table->elements[i].indices); 
			printf("=%s", printType(Table->elements[i].type));
			ptr = Table->elements[i].next;
			while (ptr != NULL)
			{
				printf("-> %s", ptr->identifier);
				if(ptr->indices > 1)
					printf("(%d)", ptr->indices); 
				printf("=%s", printType(ptr->type));
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
  /*find integers and constants*/
  int tmp = hash(in, Table->size);
  if(Table->elements[tmp].identifier != NULL)
  {
    HashElement *ptr = &Table->elements[tmp];
    if(strcmp(ptr->identifier, in) == 0)
      ret = ptr;/*success!!*/
    while(ptr != NULL)
    {
      if(strcmp(ptr->identifier, in) == 0)
        ret = ptr;/*success!*/
      ptr = ptr->next;
    }
    if(Table->parent != NULL)
    {
      ret = lookUpId(in, Table->parent);
    }
  }
  return ret;
}















