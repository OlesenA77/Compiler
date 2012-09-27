/*Alexander Olesen
 *1671836
 *CMPT 399
 *rules for flex to generate scanner files 
 */
    
%{

#include "globals.h"

%}


DIGIT    [0-9]

NUMBER	 {DIGIT}+	

HEXLETTER [a-fA-F]

HEXDIGIT {DIGIT}|{HEXLETTER}

HEXNUMBER #{HEXDIGIT}+

NUM	 {HEXNUMBER}|{NUMBER}

ID       [a-zA-Z][a-zA-Z0-9_]*

%%

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

<<EOF>>			return T_ENDFILE;
  

{NUM}			return T_NUM;

{ID}        		return T_ID;
          
[ \t]+          /* eat up whitespace */

"\n"			return T_NL;

.     		     	return T_ERROR; /*unrecognized token*/
     
%%

