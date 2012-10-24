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
#include <stdlib.h>
#include <stdio.h>
#define USAGE "Usage: mtp [options] file \n\
Options: \n\
  -h         Display this information \n\
  -v         Display extra debugging information"



int main( int argc, char **argv )
{
  int linecount=1;            /*line counter*/
  char cmdopt;                /*catch value for getopt*/
  verboflag=0;            /*flag for verbose mode, incremented by each
		                				level of verbose mode eg. -v, -vv, -vvv*/

  int tmpTok; /*value to catch getToken return value*/

  char *tmpVal = malloc(sizeof(char)*256); /*variable to catch yytext on each
					     run*/

  nestedCounter=0; /*declared in globals.h so mtp.l is in scope
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
  if(argc> 0)
    yyin = fopen(argv[argc-1], "r");

  else
    {
      printf(USAGE);
      return 0;
    }

  tmpTok=1;
/*============================================================*
 *  RUN LOOP                                                  *
 *============================================================*/

      /*tier 1 verbose mode*/
/*      if(verboflag > 0)*/
/*				{*/
/*	  			if(tmpTok == T_NL)*/
/*	    			{*/
/*	      			continue;*/
/*	    			}*/
/*					printf("%i:  ", linecount);*/
/*	  			/*printf("%s ", TOKEN[tmpTok]);*/
/*					printf("%s\n", tmpVal);*/
/*				}*/
/*      else if(tmpTok == T_ERROR) /*basic error handling*/
/*				{*/
/*	  			printf("%i:  ", linecount);*/
/*	  			/*printf("%s ", TOKEN[tmpTok]);*/
/*	  			printf("%s\n", tmpVal);*/
/*				}*/
/*    }*/

	parse();
	printf("%d", verboflag);
  return 0;

}

