
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "hash.h"

/*
 * Initialize a new Hash Table
 * Returns a pointer to the new table
 */
HashTable *hash_init(int size)
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
	Table->prev = NULL;
	while(i<size)
	{
		Table->elements[i].next = NULL;
		Table->elements[i].identifier = NULL;
		Table->elements[i].num = 0;
		Table->elements[i].memalloc = 0;
		Table->elements[i].address = 0;
		Table->elements[i].offset = 0;
		Table->elements[i].type = NA;
		i++;
	}
	
	return Table;
}

/*
 * Increases the size of a hash table, simply by doubling it
 */
HashTable *growTable(HashTable *Table)
{
	int i;
	HashElement *ptr;
	HashTable *newTable = hash_init(Table->size*2);
	for(i = 0; i<Table->size; i++)
	{
		if(Table->elements[i].identifier != NULL)
		{
			add(Table->elements[i].identifier, Table->elements[i].type,
				 newTable);
			ptr = Table->elements[i].next;
			while (ptr != NULL)
			{
				add(ptr->identifier, ptr->type, newTable);
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
int add(char *in, TYPE type, HashTable *Table)
{
	int tmp = hash(in, Table->size);
	fprintf(stderr, "%d\n", tmp);
	fprintf(stderr, "%s\n", in);
	/*no collisions*/
	if(Table->elements[tmp].identifier == NULL)
	{
		Table->elements[tmp].identifier = malloc(sizeof(char)*strlen(in));
		strcpy(Table->elements[tmp].identifier, in);
		Table->elements[tmp].type = type;
		return 0;
	}
	/*collision resolution*/
	else
	{
		if(strcmp(in, Table->elements[tmp].identifier) == 0)
		{
			return 1;
		}
		HashElement *ptr;
		ptr = Table->elements[tmp].next;
		while(ptr != NULL)
		{
			ptr = ptr->next;	
		}
		ptr = malloc(sizeof(HashElement));
		ptr->identifier = malloc(sizeof(char)*strlen(in));
		strcpy(ptr->identifier, in);
		ptr->type = type;
		return 0;
	}	
}

/*
 * Prints out a given hash table
 */
void printTable(HashTable *Table)
{
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
			ptr = Table->elements[i].next;
			while (ptr != NULL)
			{
				printf("-> %s", ptr->identifier);
				printf("=%s", printType(Table->elements[i].type));
				ptr = ptr->next;
			}
		}
		printf("\n");
		i++;
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
	if(type == boolean)
		return "boolean";
	return "Error";
}




















