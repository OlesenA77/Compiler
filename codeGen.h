/*Alexander Olesen
 *1671836
 *CMPT 399
 */
 
/*
 *Header file for the code generator
 */
 
#ifndef _CODE_GEN_H
#define _CODE_GEN_H


#define HEADER "\
;;;Assembly output of mtp\n\
\textern read\n\
\textern write\n\
\tsection .data\n\
\tsection .bss\n\
\tsection .text\n\
\tglobal main\n\
main:\n\
\tpush ebp\n\
\tmov ebp, esp\n\
"

#define FOOTER "\
\tmov esp, ebp\n\
\tpop ebp\n\
\tret"
int codeGen(TreeBranch *head, HashTable *parent);

int cGen(TreeBranch *cur);

void printLine(char *label, char *command, char *arguments, char *comment);

void generateExpression(TreeBranch *cur);
#endif
