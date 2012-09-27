/*Alexander Olesen
 *1671836
 *CMPT 399
 *Lexer Assignment
 *main .c file, calls functions from files generated
 *by flex and mtp.l
 */

#include "mtp.h"
#include "globals.h"
#include <stdlib.h>
#include <stdio.h>
#define USAGE "More to be added here"

Token getToken()
{
  Token blarg;
  blarg = yylex();
  return blarg;
}



int main( int argc, char **argv )
{
  char *TOKEN[]={
  "ENDFILE", "ERROR",
    "AND  ", "OR   ", "NOT  ",
    "IF   ", "THEN ", "ELSE ",
    "WHILE", "DO   ",
    "BEGIN", "END  ",
    "CASE ", 
    "ARRAY", "INT  ", "CONS ", "VAR  ",
    "OF   ",
    "READ ", "WRITE",
    "INBET",
    "ADD  ", "SUB  ", "MUL  ", "DIV  ", "MOD  ", "SHL  ", "SHR  ",
    "EQUAL", "GTHAN", "LTHAN", "GEQ  ", "LEQ  ",
    "NUM  ",
    "ID   "
    };
  int linecount=1; /*line counter*/
  char cmdopt; /*catch value for getopt*/
  int verboflag=0; /*flag for verbose mode, incremented by each
		   level of verbose mode eg. -v, -vv, -vvv*/

  Token tmpTok; /*value to catch getToken return value*/

  char *tmpVal = malloc(sizeof(char)*256);

  
  /*Process Command line Options*/
  while ((cmdopt = getopt(argc, argv, "vh:")) != -1) {
    switch (cmdopt) 
      {
      case 'v':
	verboflag++;
	break;
	
      /*print help message and quit*/
      case 'h':
	/*print("%s", help);*/
	exit(0);
      }
    
  }
  if(argc == 3)
    yyin = fopen(argv[2], "r");

  else if(argc == 2)
    yyin = fopen(argv[1], "r");  
  else
    {
      printf(USAGE);
      return 0;
    }

  tmpTok=1;

  while(tmpTok != T_ENDFILE)
    {
      tmpTok = getToken(); /*return the next token*/ 
      tmpVal = yytext;  /*catch the string literal from file*/
      
      if(tmpTok == T_NL)
	{
	  linecount++;
	}
      if(verboflag > 0)
	{
	  if(tmpTok == T_NL)
	    {
	      continue;
	    }
	  printf("%i:  ", linecount);
	  printf("%s ", TOKEN[tmpTok]);
	  printf("%s\n", tmpVal);
	}
      else if(tmpTok == T_ERROR) /*basic error handling*/
	{
	  printf("%i:  ", linecount);
	  printf("%s ", TOKEN[tmpTok]);
	  printf("%s\n", tmpVal);
	}
    }
  
  return 0;
  
}

