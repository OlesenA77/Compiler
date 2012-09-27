/*Alexander Olesen
 *1671836
 *CMPT 399
 *rules for flex to generate scanner files 
 *Error handling for lexical errors
 *uses a incrementing int to handle nested comments
 */
    
%{

#include "globals.h"

%}


DIGIT    [0-9]

NUMBER	 {DIGIT}+	

HEXLETTER [a-fA-F]

LETTER [a-zA-Z]

HEXDIGIT {DIGIT}|{HEXLETTER}

HEXNUMBER #{HEXDIGIT}+

NUM	 {HEXNUMBER}|{NUMBER}

ID       [a-zA-Z][a-zA-Z0-9_]*

%s COMMENTED


%%


<INITIAL>
{
"(*"            BEGIN(COMMENTED); nestedCounter++;
}
<COMMENTED>
{
"(*"	  	nestedCounter++;
"*)"      	nestedCounter--;
"\n"		return T_NL;
}


([aA][nN][dD])		return T_AND; 

([aA][rR][rR][aA][yY]) 	return T_ARRAY;

([bB][eE][gG][iI][nN])	return T_BEGIN;

([cC][aA][sS][eE])	return T_CASE;

([cC][oO][nN][sS][tT]) 	return T_CONS;

([dD][iI][vV])		return T_DIV;  

([dD][oO])		return T_DO;   

([eE][lL][sS][eE])	return T_ELSE;  

([eE][nN][dD])		return T_END;  

([iI][fF])		return T_IF;   

([iI][nN][tT][eE][gG][eE][rR])	return T_INT;  

([mM][oO][dD])		return T_MOD;  

([nN][oO][tT])		return T_NOT;  

([oO][fF])		return T_OF;    

([oO][rR])		return T_OR;  

([rR][eE][aA][dD])	return T_READ;  

([sS][hH][lL])		return T_SHL;  

([sS][hH][rR])		return T_SHR;  

([tT][hH][eE][nN])	return T_THEN; 

([vV][aA][rR])		return T_VAR;   

([wW][hH][iI][lL][eE])	return T_WHILE; 

([wW][rR][iI][tT][eE])	return T_WRITE;

"<>"			return T_INBET; 

"+"			return T_ADD;

"-"			return T_SUB;

"*"			return T_MUL;

"="			return T_EQ;

">"			return T_GTHAN;

"<"			return T_LTHAN;

">="			return T_GEQ;

"<="			return T_LEQ;

"("			return T_LPAREN;

")"			return T_RPAREN;

","			return T_COMMA;

"."			return T_PERIOD;

<<EOF>>			return T_ENDFILE;
  
#.*[g-zG-Z].*		{printf("out of range hexidecimal term on line ");
			 return T_ERROR;}

{NUM}			return T_NUM;



{ID}			return T_ID;
          
[ \t]+	                /* eat up whitespace */

"\n"			return T_NL;


"*)"			{printf("trailing end comment on line ");
			 return T_ERROR;}

.     		     	{printf("unrecognized character on line ");
			return T_ERROR;}
     
%%

