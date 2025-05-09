#ifndef B9A4182F_6259_4B18_B2EF_396B351AF795
#define B9A4182F_6259_4B18_B2EF_396B351AF795
#include "1.h"
/*r型格式生成，返回值为该目标代码的行数*/
int objMips_UnConj(string op, string target, string comment);
int objMips_RM(string op, string rs, string rt, int immediate, string comment);
int objMips_Conj(string op, string rs, string rt, int immediate, string comment);
int objMips_Conj(string op, string rs, string rt, string tag, string comment);
int objMips_RegIm(string op, string rd, string rs, string immediate, string comment);
int objMips_Regs(string op, string rd, string rs, string comment);
int objMips_Regs(string op, string rd, string rs, string rt, string comment);
int objMips_Regs(string op, string rd, string comment);
void objMips_SysCall(int choice, string reg = "");
/*找到变量所在AR的基址*/
/*var变量要对对应level-1的过程活动记录找！！！！！！！！*/
void getARBase(int varLevel);

/*找到变量所在的绝对地址*/
void findAbsAddr(Treenode* t);

/*为运算表达式生成目标代码*/
void expObj(Treenode* t);

/*为具体语句生成目标代码，也就是begin~end里的*/
void stObj(Treenode* t);

/*为过程体中语句生成目标代码*/
void genStmt(Treenode* t);
void genExp(Treenode* t);
/*为主程序入口目标代码生成*/
void cGen(Treenode* t);
void codeGen(Treenode* t);
void codeGen(TreenodePtr& t);
int  emitSkip(int num);
void emitBackup(int loc, int addr);
void emitRestore();
void emitComment(string c);
void genProc(Treenode* t);

#endif /* B9A4182F_6259_4B18_B2EF_396B351AF795 */
