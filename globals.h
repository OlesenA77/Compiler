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

int verboflag;

int lineCount;

/*PROTOTYPES*/
int getToken(void);
int parse();

#endif /*globals.h*/
