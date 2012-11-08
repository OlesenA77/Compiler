#ifndef _HASH_H
#define _HASH_H
/*  header file for implementation of a hash table with seperate
	chaining as well as linked list of hash tables for seperate
	scopes
*/

/*enumerated type for types*/
typedef enum {NA=0,cons_int, var_int, boolean} TYPE;

/*elements of the hash table*/
typedef struct _hash_element{
	struct _hash_element *next;
	char * identifier;
	int    num;
	int memalloc;
    int address;
	int offset;
	TYPE type;
} HashElement;

/*hash table*/
typedef struct _hash_table{
	struct _hash_table *next;
	struct _hash_table *prev;
	HashElement *elements;
	int size;
}HashTable;

HashTable *hash_init(int size);

int hash(char *in, int size);

int add(char *in, TYPE type, HashTable *table);

HashTable *growtable(HashTable *Table);

void printTable(HashTable *Table);

char *printType(TYPE type);

int search();

int probe();

#endif
