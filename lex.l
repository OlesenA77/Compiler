/*Alexander Olesen
 *1671836
 *CMPT 399
 *rules for flex to generate scanner files
 *Error handling for lexical errors
 *uses a incrementing int to handle nested comments
 */

%{
#include "globals.h"
#include "parse.h"
#define TOK_MAX 12
#define _POSIX_C_SOURCE 200809L

char tokenString[TOK_MAX+1];
/*strings to be used for verbose token returns*/
  static char *TOKENS[]={
  "ENDFILE",
	"ERROR",
  "AND   ", "OR    ", "NOT   ",
  "IF    ", "THEN  ", "ELSE  ",
  "WHILE ", "DO    ",
  "BEGIN ", "END   ",
  "CASE  ",
  "ARRAY ", "INT   ", "CONS  ", "VAR   ",
  "OF    ",
  "READ  ", "WRITE ",
  "INBET ",
  "ADD   ", "SUB   ", "MUL   ", "DIV   ", "MOD   ", "SHL   ", "SHR   ",
  "EQUAL ", "GTHAN ", "LTHAN ", "GEQ   ", "LEQ   ",
  "LPAREN", "RPAREN", "COMMA ", "PERIOD",
  "COLON ", "SEMCLN", "ASSIGN",
  "NUM   ",
  "ID    "
    };
int printflag=1;
%}


DIGIT    [0-9]
NUMBER	 {DIGIT}+	
HEXLETTER [a-fA-F]
LETTER [a-zA-Z]
HEXDIGIT {DIGIT}|{HEXLETTER}
HEXNUMBER #{HEXDIGIT}+
ID       [a-zA-Z][a-zA-Z0-9_]*

%%

([aA][nN][dD])					{return T_AND;}
([aA][rR][rR][aA][yY]) 	{return T_ARRAY;}
([bB][eE][gG][iI][nN])	{return T_BEGIN;}
([cC][aA][sS][eE])			{return T_CASE;}
([cC][oO][nN][sS][tT]) 	{return T_CONS;}
([dD][iI][vV])					{return T_DIV;}
([dD][oO])							{return T_DO;}
([eE][lL][sS][eE])			{return T_ELSE;}
([eE][nN][dD])					{return T_END;}
([iI][fF])							{return T_IF;}
([iI][nN][tT][eE][gG][eE][rR])	{return T_INT;}
([mM][oO][dD])					{return T_MOD;}
([nN][oO][tT])					{return T_NOT;}
([oO][fF])							{return T_OF;}
([oO][rR])							{return T_OR;}
([rR][eE][aA][dD])			{return T_READ;}
([sS][hH][lL])					{return T_SHL;}
([sS][hH][rR])					{return T_SHR;}
([tT][hH][eE][nN])			{return T_THEN;}
([vV][aA][rR])					{return T_VAR;}
([wW][hH][iI][lL][eE])	{return T_WHILE;}
([wW][rR][iI][tT][eE])	{return T_WRITE;}

"<>"			{return T_INBET;}
"+"				{return T_ADD;}
"-"				{return T_SUB;}
"*"				{return T_MUL;}
"="				{return T_EQ;}
">"				{return T_GTHAN;}
"<"				{return T_LTHAN;}
">="			{return T_GEQ;}
"<="			{return T_LEQ;}
"("				{return T_LPAREN;}
")"				{return T_RPAREN;}
","				{return T_COMMA;}
":"				{return T_COLON;}
";"				{return T_SEMCLN;}
":="      {return T_ASSIGN;}
"."				{return T_PERIOD;}

#.*[g-zG-Z].*		{printf("out of range hexidecimal term on line ");
			 						return T_ERROR;}

{NUMBER}			{yylval.num = strtol(yytext, NULL, 10);
						 return T_NUM;}

{HEXNUMBER}	{yylval.num = strtol(yytext, NULL, 16);
						 return T_NUM;}

{ID}			{yylval.string = strdup(yytext);
							return T_ID;}

[ \t]+	  /* eat up whitespace */

"\n"			{lineCount++;}

"(*" 			{nestedCounter++;}

"*)"			{ if(nestedCounter > 0)
						{
							nestedCounter--;
						}
						else
						{
							printf("trailing end comment on line ");
							return T_ERROR;
						}	
			 		}

.     		{return T_ERROR;}

%%
int getToken(void)
{
	int tmp = yylex();	
	/*lexer verbose mode*/
	while(nestedCounter != 0)
	{		
		tmp = yylex();
	}
	if(verboflag > 0) 	
	{	
		if(nestedCounter == 0)
		{		
			strncpy(tokenString, yytext, TOK_MAX);
			if(1 == printflag)
			{
				printf("%d  ", tmp-258);		
				printf("%s  ", TOKENS[tmp-258]);
				printf("%s\n", tokenString);
			}
			if("PERIOD" == TOKENS[tmp-258])	
				printflag=0;
		}
	}
	
	if(tmp == T_ERROR)
		{
			printf("unrecogized token at line: \n");
		}
	return tmp;
}


