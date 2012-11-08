/*Alexander Olesen
 *1671836
 *CMPT 399
 */
#define MAX 64
/*modified from tiny language parser from Louden*/
#include "globals.h"

TreeBranch *initializeTreeBranch();

void setTreeBranch(TreeBranch *Branch, TreeBranch *childA, TreeBranch *childB,
								 TreeBranch *childC, TreeBranch *sib, char _type[],
												char _subtype[], char _attribute[]);

void setTreeBranchNUM(TreeBranch *Branch, TreeBranch *childA,TreeBranch *childB,
									 TreeBranch *childC, TreeBranch *sib, char _type[],
												char _subtype[], int num);
	
void setEnum(TreeBranch *Branch, node_T T);

void setString(char *des, char *src);

void printTree(TreeBranch * cur, int indent);
