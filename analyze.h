#ifndef _ANALYZE_H
#define _ANALYZE_H

#include "globals.h"
#include "hash.h"

HashTable *analyze(TreeBranch *head);

int analyzer(TreeBranch *cur, int retval);

#endif
