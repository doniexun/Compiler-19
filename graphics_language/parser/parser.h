/*************************************************************************
    > File Name: parser.h
    > Author: Shawn Guo
    > Mail: iguoxiaopeng@gmail.com 
    > Created Time: 2013年12月08日 星期日
    > Last Changed: 
    > Notes:	简易绘图函数的语法分析器，需结合词法分析器。
    >			1.建立语法树	2.打印语法树	
*************************************************************************/

#ifndef __PARSE_H__
#define	__PARSE_H__

#include "lexical.h"
#include "parser.h"

/**************************** 相关数据结构定义 **************************/
typedef struct ExprNode ExprNode;
typedef struct CaseOperator CaseOperator;
typedef struct CaseFunc CaseFunc;
typedef union Content Content;
typedef double (*FuncPtr)(double);//定义一个返回值为double，参数为double的函数指针,用作存储sin等地址
struct ExprNode{				//表达式的语法树
	enum Token_Type OpCode;		//关于表达式词法单元的种类:PLUS,MINUS,MUL,DIV,POWER,FUNC,CONST_ID,T
	union{
		struct {
			ExprNode *left, *right;		//二元操作树的左右子树
		} CaseOperator;					//二元操作
		struct {
			ExprNode *child;
			FuncPtr MathFuncPtr;		//对应的数学调用函数:sin,cos...
		} CaseFunc;						//函数调用
		double CaseConst;				//常数,绑定右值,对应OpCode=CONST_ID
		double *CaseParmPtr;			//参数T,绑定左值,OpCode=T, 参数T为全局变量，这里保存其地址
	} Content;
};



/************************** 语法分析器接口 ****************************/

//调用词法分析器接口，取出一个合法的词法单元,存放在token全局变量中.
//取到非法词法单元，则调用Syntax_Error函数.
void Fetch_Token();						

//先匹配已提前读取出来的词法单元的类型是否和AToken匹配,否则语法错误
//然后再提前读取一个词法单元
void Match_Token(enum Token_Type AToken);

//根据错误码打印出错信息并退出语法分析器
void Syntax_Error(int erron);

//根据参数和类型制作一个表达式子树并返回其根地址
//如果制作失败,则打印错误信息并退出语法分析器
ExprNode * MakeExprNode(Token_Type OpCode, void* arg1, void* arg2);	
//表达式相关函数
//递归下降语法分析-建立表达式语法树
ExprNode* Expression();
ExprNode* Term();
ExprNode* Factor();
ExprNode* Component();
ExprNode* Atom();

//对SrcFilePtr指针所指向的文件进行语法分析
//语法分析器的入口函数:
//1.初始化词法分析器
//2.进行递归下降语法分析
//3.直到文件结束，关闭词法分析
void Parser(char* SrcFilePtr);
void Program();
void Statement();
void Origin_Statement();
void Rot_Statement();
void Scale_Statement();
void For_Statement();

//给出已经建立好的语法分析树的根节点
//按照深度优先的先序遍历将语法树的逻辑结构打印出来.
//其中indent为缩进值，用于调整打印位置
void Print_Syntax_Tree(ExprNode *root, int indent);
#endif
