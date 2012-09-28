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

/*used in mtp.l and main.c to not read comments
 * no matter how nested*/
int nestedCounter;

/*enumerated type for all keywords, operators and data*/
typedef enum {
T_ENDFILE = 0, 
T_ERROR, 

T_AND, T_OR, T_NOT, 

T_IF, T_THEN, T_ELSE,

T_WHILE, T_DO,

T_BEGIN, T_END,

T_CASE, 

T_ARRAY, T_INT, T_CONS, T_VAR,

T_OF, 
 
T_READ, T_WRITE,

T_INBET,

T_ADD, T_SUB, T_MUL, T_DIV, T_MOD, T_SHL, T_SHR, 

T_EQ, T_GTHAN, T_LTHAN, T_GEQ, T_LEQ,

T_LPAREN, T_RPAREN, T_COMMA, T_PERIOD,

T_COLON, T_SEMCLN,

T_NUM,

T_ID, 

T_NL

}Token;

#endif
