/*Alexander Olesen
 *1671836
 *CMPT 399
 *Lexer Assignment
 *main .c file, calls functions from files generated
 *by flex and mtp.l
 */

#include "lex.h"
#include "parse.h"
#include "globals.h"
#include "hash.h"
#include "analyze.h"
#include "tree.h"
#include <stdlib.h>
#include <stdio.h>
#define USAGE "Usage: %s [options] file \n\
Options: \n\
  -h         Display this information \n\
  -v         Display extra debugging information\n", argv[0]
/*used in mtp.l and main.c to not read comments
 * no matter how nested*/



int main( int argc, char **argv )
{
	int indent = 0;			/*indent for printing tree*/
	lineCount=1;            /*line counter*/
	char cmdopt;                /*catch value for getopt*/
	verboflag=0;            /*flag for verbose mode, incremented by each
		                				level of verbose mode eg. -v, -vv, -vvv*/
	/*
	int tablesize = 20;
	HashTable *SymTable;
	SymTable = hash_init(tablesize);
	if(SymTable == NULL)
	{
		fprintf(stderr, "PROBLEM");
	}
	add("Blarg", var_int, SymTable);
	add("Honk", var_int, SymTable);
	add("Arrgh", var_int, SymTable);
	add("HonkHonk", var_int, SymTable);
	add("Bowla", var_int, SymTable);
	add("B1", var_int, SymTable);
	add("Flog", var_int, SymTable);
	add("blarg", var_int, SymTable);
	printTable(SymTable);
	printf("\n\n");*/


	int nestedCounter=0; /*declared in globals.h so mtp.l is in scope
		      for the variable*/

	/*Process Command line Options*/
	while ((cmdopt = getopt(argc, argv, "vh:")) != -1)
	{
    	switch (cmdopt)
		{
      		case 'v':
				verboflag++;
				break;
	
    		/*print help message and quit*/
     	 	case 'h':
				printf("%s", USAGE);
				exit(0);
      	}
  	}
	if(argc > 1)
	{
		yyin = fopen(argv[argc-1], "r");
		if(yyin == NULL)
		{
			printf("file not found\n", stderr);
			exit(1);
		}
	}	
  else
    {
      printf(USAGE);
      return 0;
    }

/*============================================================*
 *  RUN LOOP                                                  *
 *============================================================*/

	parse();
	TreeBranch *tmp;
	tmp = retHead();
	if(verboflag > 1)
	{
		printTree(tmp, indent);
	}
	tmp = retHead();
	analyze(tmp);
  return 0;

}

