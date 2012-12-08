/*Alexander Olesen
 *1671836
 *CMPT 399
 */
 
/*
 *implementation for the code generator
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "globals.h"
#include "hash.h"
#include "tree.h"
#include "codeGen.h"

HashTable *currentTable;
int tableflag = 0;
static int curOffset = 4;
FILE *ff;
int labelnum = 0;
/*
 *==============================================
 * Wrapper function for the traversing cGen 
 * function
 *==============================================
 */
int codeGen(TreeBranch *head, HashTable *parent)
{
  currentTable = parent;
  ff = fopen("Output/output.s", "w+");
  fprintf(ff, HEADER);  
  cGen(head);
  fprintf(ff, FOOTER);
  fclose(ff);
  return 0;
}
/*
 *=============================================
 * recursive traversing function that generates
 * assembly code
 *=============================================
 */
int cGen(TreeBranch *cur)
{
  int skip1 = 0;
  int skip2 = 0;
  int skip3 = 0;
  int skipSib = 0;
  
  /*=====================================================================================
   *   Block Statements
   *
   *=====================================================================================
   */  
  if(cur->nodeT == new_blk)
  {
    /*first block*/
    if(tableflag == 0)
    {
      tableflag = 1;
      currentTable->MaxOffset = curOffset;
       if(cur->child1 != NULL)
        cGen(cur->child1);
       if(cur->child2 != NULL)
        cGen(cur->child2);
       if(cur->child3 != NULL)
        cGen(cur->child3);
       skip1 = 1;
       skip2 = 1;
       skip3 = 1;
       skipSib = 1;  
    }
    /*other blocks*/
    else
    {
      currentTable->MaxOffset = curOffset;
      currentTable = currentTable->next;
      fprintf(ff, ";;; entering new stack frame\n");
      if(cur->child1 != NULL)
        cGen(cur->child1);
      if(cur->child2 != NULL)
        cGen(cur->child2);
      if(cur->child3 != NULL)
        cGen(cur->child3);
      
      currentTable = currentTable->parent;
      if(currentTable->next != NULL)
      { 
        if (currentTable->next->next != NULL)
        {
          currentTable->next = currentTable->next->next;
        }
      }  
      fprintf(ff, "\tadd esp, %d ;reset stack pointer\n", curOffset-currentTable->MaxOffset);
      curOffset = currentTable->MaxOffset;  
      if(cur->sibling != NULL)
        cGen(cur->sibling);
        
      skip1 = 1;
      skip2 = 1;
      skip3 = 1;
      skipSib = 1;  
    }
  }
  /*================================================================================
   *        Constant Declarations
   *================================================================================
   */
   else if(cur->nodeT == const_decl)
    {
	  HashElement *tmp = lookUpId(cur->child1->attribute, currentTable);
	  tmp->val = cur->child2->num;
	  /*fprintf(ff, "\tsub esp, %d\n", 4);
	  fprintf(ff, "\tmov dword [ebp - %d], %d\t;cons %s\n", tmp->offset, tmp->val, tmp->identifier);*/
    }
  /*================================================================================
   *          Variable Declarations
   *================================================================================
   */
   else if(cur->nodeT == var_decl)
    {
      if(cur->child2->nodeT == sing_decl)
      {
       /*================================================================================
	    *       Non-Array Declarations
	    *================================================================================
	    */
	    HashElement *tmp = lookUpId(cur->child1->attribute, currentTable);
	    tmp->offset = curOffset;
	    curOffset += 4;
	    /*initialize all variables to zero, seems easier that way*/
	    fprintf(ff, "\tsub esp, 4\t;variable %s\n", tmp->identifier);
	    //fprintf(ff, "\tmov dword [ebp - %d], %d\t;var %s\n", tmp->offset, 0, tmp->identifier);
	    TreeBranch *ptr = cur->child1->sibling;
	    while(ptr != NULL)
	    {
	      tmp = lookUpId(ptr->attribute, currentTable);
	      tmp->offset = curOffset;
	      curOffset +=4;
	      /*initialize all variables to zero, seems easier that way*/
	      fprintf(ff, "\tsub esp, 4\t;variable %s\n", tmp->identifier);
	      //fprintf(ff, "\tmov dword [ebp - %d], %d\t;var %s\n", tmp->offset, 0, tmp->identifier);
	      ptr = ptr->sibling;
	    }
	  }
	  /*================================================================================
	   *       Array Declarations
	   *================================================================================
	   */
	  if(cur->child2->nodeT == arr_decl)
	  {
	    HashElement *tmp = lookUpId(cur->child1->attribute, currentTable);
	    tmp->offset = curOffset;
	    if(tmp->indices == 0)
	    {
	      HashElement *tmp2 = lookUpId(cur->child2->child1->attribute, currentTable);
	      tmp->indices = tmp2->val;
	    }
	    curOffset += (4*tmp->indices);
	    fprintf(ff, "\tsub esp, %d\t;array %s\n", (4*tmp->indices), tmp->identifier);
	    TreeBranch *ptr = cur->child1->sibling;
	    while(ptr != NULL)
	    {
	      if(tmp->indices == 0)
	      {
	        HashElement *tmp2 = lookUpId(cur->child2->child1->attribute, currentTable);
	        tmp->indices = tmp2->val;
	      }
	      tmp = lookUpId(cur->child1->attribute, currentTable);
	      tmp->offset = curOffset;
	      curOffset += (4*tmp->indices);
	      fprintf(ff, "\tsub esp, %d\t;array %s\n", (4*tmp->indices), tmp->identifier);
	      ptr = ptr->sibling;
	    }
	  }
    }
  /*=====================================================================================
   *     Assignment statements
   *
   *=====================================================================================
   */
   if(cur->nodeT == assign)
   { 
     int prevOffset = curOffset;
     fprintf(ff, "\t;; Assignment Statement\n");
     HashElement *tmp = lookUpId(cur->child1->attribute, currentTable);
     generateExpression(cur->child2);
     fprintf(ff, "\tmov [ebp - %d], eax\t;assigning\n", tmp->offset);
     fprintf(ff, "\tadd esp, %d\t;end of assignment\n", curOffset-prevOffset);
     curOffset = prevOffset;
   }
  /*=====================================================================================
   *    Write statements
   *
   *=====================================================================================
   */
    if(cur->nodeT == write_stmt)
    {
      int wrcount = 0;
      TreeBranch *ptr = cur->child1;
      fprintf(ff, "\t;; Write Statement\n");
      while(ptr != NULL)
      {
        generateExpression(ptr);
        fprintf(ff, "\tsub esp, 4\n");
        fprintf(ff, "\tmov [ebp - %d], eax\n", curOffset);
        curOffset += 4;
        wrcount++;
        ptr = ptr->sibling;
      } 
      fprintf(ff, "\tsub esp, 4\n");
      fprintf(ff, "\tmov dword [ebp - %d], %d\n", curOffset, wrcount);
      fprintf(ff, "\tcall write\n");
      fprintf(ff, "\tadd esp, %d\n", (wrcount*4 + 4));
      curOffset -= (wrcount*4);
      skip1 = 1;
      skip2 = 1;
      skip3 = 1;
    }
   
   /*=====================================================================================
   *     Read Statements
   *
   *=====================================================================================
   */
   if(cur->nodeT == read_stmt)
    {
      int rdcount = 0;
      TreeBranch *ptr = cur->child1;
      fprintf(ff, "\t;; Write Statement\n");
      while(ptr != NULL)
      {
        generateExpression(ptr);
        fprintf(ff, "\tsub esp, 4\n");
        fprintf(ff, "\tmov [ebp - %d], eax\n", curOffset);
        curOffset += 4;
        rdcount++;
        ptr = ptr->sibling;
      } 
      fprintf(ff, "\tsub esp, 4\n");
      fprintf(ff, "\tmov dword [ebp - %d], %d\n", curOffset, rdcount);
      fprintf(ff, "\tcall read\n");
      fprintf(ff, "\tadd esp, %d\n", (rdcount*4 + 4));
      curOffset -= (rdcount*4);
      skip1 = 1;
      skip2 = 1;
      skip3 = 1;
    }
   
   /*=====================================================================================
   *      While statements
   *
   *=====================================================================================
   */
    if(cur->nodeT == while_stmt)
    {
      
        fprintf(ff, "\t;; While Statement\n");
        int prevOffset = curOffset;
        generateExpression(cur->child1);
        int iterlabel = labelnum;
        labelnum++;
        int endlabel = labelnum;
        labelnum++;
      
        fprintf(ff, "L%d:\n", iterlabel);
        generateExpression(cur->child1);   
        /*generate the necessary jump after the comparison*/
        if(strcmp(cur->child1->attribute, "=")==0)
        {
            fprintf(ff, "\tje L%d\n", endlabel);
        }
        if(strcmp(cur->child1->attribute, ">")==0)
        {
            fprintf(ff, "\tjg L%d\n", endlabel);
        }
        if(strcmp(cur->child1->attribute, "<")==0)
        {
            fprintf(ff, "\tjl L%d\n", endlabel);
        }
        if(strcmp(cur->child1->attribute, ">=")==0)
        {
            fprintf(ff, "\tjge L%d\n", endlabel);
        }
        if(strcmp(cur->child1->attribute, "<=")==0)
        {
            fprintf(ff, "\tjle L%d\n", endlabel);
        }
        cGen(cur->child2);
        fprintf(ff, "\tjmp L%d\t;iterate\n", iterlabel);
        fprintf(ff, "L%d:\n", endlabel);
        fprintf(ff, "\tadd esp, %d\n;;;end of while statement\n", curOffset-prevOffset);
        curOffset = prevOffset;    
    }
   
   /*=====================================================================================
   *     if statements
   *
   *=====================================================================================
   */
    if((cur->nodeT == if_stmt) || (cur->nodeT == ifelse_stmt))
    {
        int prevOffset = curOffset;
        fprintf(ff, "\t;; If Statement\n");
        int iflabel = labelnum;
        labelnum++;
        generateExpression(cur->child1);   
        /*generate the necessary jump after the comparison*/
        if(strcmp(cur->child1->attribute, "=")==0)
        {
            fprintf(ff, "\tjne L%d\n", iflabel);
        }
        if(strcmp(cur->child1->attribute, ">")==0)
        {
            fprintf(ff, "\tjng L%d\n", iflabel);
        }
        if(strcmp(cur->child1->attribute, "<")==0)
        {
            fprintf(ff, "\tjnl L%d\n", iflabel);
        }
        if(strcmp(cur->child1->attribute, ">=")==0)
        {
            fprintf(ff, "\tjnge L%d\n", iflabel);
        }
        if(strcmp(cur->child1->attribute, "<=")==0)
        {
            fprintf(ff, "\tjnle L%d\n", iflabel);
        }
        cGen(cur->child2);
        skip2 = 1;
        /*
         *if there is no else, simply skip to the end
         */
        if(cur->nodeT == if_stmt)
        {
            fprintf(ff, "L%d:\n", iflabel);
        }
        /* disclaimer: this is really hard to not sound confusing
         * 
         * if the (if statement) has an (else statenment) then the (if statement)
         * will need to have a jump to miss the (else statement), otherwise the
         * jump from the (if statement) should jump to the beginning of the (else
         * statement)
         */
        if(cur->nodeT == ifelse_stmt)
        {
            int elselabel = labelnum;
            labelnum++;
            fprintf(ff, "\tjmp L%d\t;skip else\n", elselabel);
            
            fprintf(ff, "L%d:;skip to else clause\n", iflabel);
            cGen(cur->child3);
            skip3 = 1;
            fprintf(ff, "L%d:\n", elselabel);
        }
            
        fprintf(ff, "\tadd esp, %d\n;;;end of if statement\n", curOffset-prevOffset);
        curOffset = prevOffset;    
    }
      
  if(cur->child1 != NULL && skip1 == 0)
    cGen(cur->child1);
  if(cur->child2 != NULL && skip2 == 0)
    cGen(cur->child2);
  if(cur->child3 != NULL && skip3 == 0)
    cGen(cur->child3);
  if(cur->sibling != NULL && skipSib == 0)
    cGen(cur->sibling);
  
  return 0;
}


/*=====================================================================================
*            Expression Handling
*      
*=====================================================================================
 */
void generateExpression(TreeBranch *cur)
{
    /*
     *==============================================================
     *            Boolean comparisons
     * Does up to the cmp function, callers responsibility to
     * know which jump to use
     *==============================================================
     */
    if(cur->nodeT == Boolean)
    {
        int tmpOffset;
        generateExpression(cur->child1);
        fprintf(ff, "\tsub esp, 4\n");
        fprintf(ff, "\tmov [ebp - %d], eax\n", curOffset);
        tmpOffset = curOffset;
        curOffset+=4;
        generateExpression(cur->child2);
        fprintf(ff, "\tcmp [ebp - %d], eax\n", tmpOffset);
    }
    /*
     *==============================================================
     *             Arithmetic expressions
     *
     *==============================================================
     */
    if(cur->nodeT == simExpr)
    {
        if(cur->attribute == NULL)
        {
            generateExpression(cur->child1);
            fprintf(ff, "\tsub esp, 4\n");
            fprintf(ff, "\tmov [ebp - %d], eax\n", curOffset);
            curOffset += 4;
            generateExpression(cur->child2);
        }

        else if(strcmp(cur->attribute, "-")==0)                             
        {
            generateExpression(cur->child1);
            fprintf(ff, "\tneg eax\t;negate the argument\n");
            fprintf(ff, "\tsub esp, 4\n");
            fprintf(ff, "\tmov [ebp - %d], eax\n", curOffset);
            curOffset += 4;
            if(cur->child2 != NULL)                                     
            {
                generateExpression(cur->child2);
            }
        }                                                      
    }
    else if(cur->nodeT == opTerm)
    {
        if(strcmp(cur->attribute, "-") == 0)
        {
            fprintf(ff, "\tsub esp, 4\n");
            fprintf(ff, "\tmov [ebp - %d], eax\n", curOffset);
            curOffset += 4;
            generateExpression(cur->child1);
            fprintf(ff, "\tsub eax, [ebp - %d]\t;subtraction\n", curOffset-4);
            if(cur->sibling != NULL)
            {
                fprintf(ff, "\tsub esp, 4\n");
                fprintf(ff, "\tmov [ebp - %d], eax\n", curOffset);
                curOffset += 4;
                generateExpression(cur->sibling);
                fprintf(ff, "\tsub eax, [ebp - %d]\t;subtraction\n", curOffset-4);
            }
        
        }
        if(strcmp(cur->attribute, "+") == 0)
        {
            fprintf(ff, "\tsub esp, 4\n");
            fprintf(ff, "\tmov [ebp - %d], eax\n", curOffset);
            curOffset += 4;
            generateExpression(cur->child1);
            fprintf(ff, "\tadd eax, [ebp - %d]\t;addition\n", curOffset-4);
            if(cur->sibling != NULL)
            {
                fprintf(ff, "\tsub esp, 4\n");
                fprintf(ff, "\tmov [ebp - %d], eax\n", curOffset);
                curOffset += 4;
                generateExpression(cur->sibling);
                fprintf(ff, "\tadd eax, [ebp - %d]\t;addition\n", curOffset-4);
            }
        }
   }
   else if(cur->nodeT == term)
   {
       if(strcmp(cur->attribute, "*") == 0)
       {
           generateExpression(cur->child1);
           fprintf(ff, "\tsub esp, 4\n");
           fprintf(ff, "\tmov [ebp - %d], eax\n", curOffset);
           curOffset += 4;
           generateExpression(cur->child2);
           fprintf(ff, "\tmov edx, dword 0\n");
           fprintf(ff, "\timul dword [ebp - %d]\t;multiplication\n", curOffset - 4);
       }
       
       if(strcmp(cur->attribute, "DIV") == 0)
       {
           generateExpression(cur->child1);
           fprintf(ff, "\tsub esp, 4\n");
           fprintf(ff, "\tmov [ebp - %d], eax\n", curOffset);
           curOffset += 4;
           generateExpression(cur->child2);
           fprintf(ff, "\tmov edx, dword 0\n");
           fprintf(ff, "\tidiv dword [ebp - %d]\t;division\n", curOffset - 4);
       }
       
       if(strcmp(cur->attribute, "MOD") == 0)
       {
           generateExpression(cur->child1);
           fprintf(ff, "\tsub esp, 4\n");
           fprintf(ff, "\tmov [ebp - %d], eax\n", curOffset);
           curOffset += 4;
           generateExpression(cur->child2);
           fprintf(ff, "\tmov edx, dword 0\n");
           fprintf(ff, "\tidiv dword [ebp - %d]\t;modulus\n", curOffset - 4);
           fprintf(ff, "\tmov eax, edx\n");
       }
       
       if(strcmp(cur->attribute, "SHL") == 0)
       {
           generateExpression(cur->child1);
           fprintf(ff, "\tsub esp, 4\n");
           fprintf(ff, "\tmov [ebp - %d], eax\n", curOffset);
           curOffset += 4;
           generateExpression(cur->child2);
           fprintf(ff, "\tshl eax, [ebp - %d]\t;left-shift\n", curOffset - 4);
       }
       
       if(strcmp(cur->attribute, "SHR") == 0)
       {
           generateExpression(cur->child1);
           fprintf(ff, "\tsub esp, 4\n");
           fprintf(ff, "\tmov [ebp - %d], eax\n", curOffset);
           curOffset += 4;
           generateExpression(cur->child2);
           fprintf(ff, "\tshr eax, [ebp - %d]\t;right-shift\n", curOffset - 4);
       }
   }
   else if(cur->nodeT == factor)
   {
       //something related to a not factor goes here
   }
      
   else if(cur->nodeT == Id)
   {
       HashElement *tmp = lookUpId(cur->attribute, currentTable);
       fprintf(ff, "\tmov eax, dword [ebp - %d]\n", tmp->offset);
       tmp = NULL;
   }
   /*
   else if(cur->nodeT == arrId)
   {
       HashElement *tmp = lookUpId(cur->attribute, currentTable);
       fprintf(ff, "\tsub esp, 4\n");
       fprintf(ff, "\tmov dword [ebp - %d], %d\t;array %s\n", curOffset, tmp->val,tmp->identifier);
       curOffset += 4;
       tmp = NULL;
   }
   */
   else if(cur->nodeT == cons)
   {
       fprintf(ff, "\tmov eax, %d\t;constant\n", cur->num);
   }
}
















