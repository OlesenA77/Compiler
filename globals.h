/*Alexander Olesen
 *1671836
 *CMPT 399
 *Lexer Assignment
 *this consists of an explicit enumerated type for
 *return values from the lexer
 */

/*referenced by main.c and mtp.l*/

#ifndef GLOBALS_H
#define GLOBALS_H

#define INIT_TABLE_SIZE 20

int nestedCounter;

int verboflag;

int lineCount;

typedef enum 
{
/*Not applicable*/
O=0,
/*declarations*/
const_decl, var_decl, arr_decl

} node_T;

typedef struct _TreeBranch
	{
		struct _TreeBranch * child1;
		struct _TreeBranch * child2;
		struct _TreeBranch * child3;
    	struct _TreeBranch * sibling;
    	char *type;
		char *subtype;
		char *attribute;
		int num;
		int flag; /*0 is no number, 1 is number is assigned*/
		node_T nodeT; /*node type*/
   } TreeBranch;
   
/*PROTOTYPES*/
int getToken(void);
int parse();

#endif /*globals.h*/
