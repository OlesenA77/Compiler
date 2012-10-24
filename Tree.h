/*Alexander Olesen
 *1671836
 *CMPT 399
 */
#define MAX 64
/*modified from tiny language parser from Louden*/


typedef struct _TreeBranch
	{
		struct _TreeBranch * child1;
		struct _TreeBranch * child2;
		struct _TreeBranch * child3;
    struct _TreeBranch * sibling;
    char *type;
		char *subtype;
		char *attribute;
		int num;
		int flag; /*0 is no number, 1 is number is assigned*/
   } TreeBranch;

TreeBranch *initializeTreeBranch();

void setTreeBranch(TreeBranch *Branch, TreeBranch *childA, TreeBranch *childB,
										 TreeBranch *childC, TreeBranch *sib, char _type[],
												char _subtype[], char _attribute[]);

void setTreeBranchNUM(TreeBranch *Branch, TreeBranch *childA,
TreeBranch *childB, TreeBranch *childC, TreeBranch *sib, char _type[],
												char _subtype[], int num);

void setString(char *des, char *src);

void printTree(TreeBranch * cur, int indent);
