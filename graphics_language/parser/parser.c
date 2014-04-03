/*************************************************************************
    > File Name: parser.c
    > Author: Shawn Guo
    > Mail: iguoxiaopeng@gmail.com 
    > Created Time: 2013年12月08日 星期日 17时03分01秒
    > Last Changed: 
    > Notes:	简易绘图函数的语法分析器，需结合词法分析器。
    >			1.建立语法树	2.打印语法树	
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "lexical.h"
#include "parser.h"

//exit()的错误码定义
#define	 EXIT_MALLOC	1
#define	 EXIT_FAIL_OPEN	2
#define  EXIT_SYNTAX	3

//Syntax_Error 语法错误码定义
//目前直接退出语法分析器
#define	 ERROR_ERRTOKEN		1			//取到非法词法单元
#define	 ERROR_MATCH		2			//不是想要的匹配词法单元，即词法单元类型错误
#define	 ERROR_ATOM			3			//不符合Atom文法定义
#define	 ERROR_STATEMENT	4			//不符合Statement文法定义


/************************************ 全局变量定义 *******************************/
extern Token token;				//词法分析器中的全局变量
double parment_t = 0;			//绘图语言唯一的一个变量,T

/************************************ 语法分析器接口定义 *******************************/


/******** 下面的语法分析，总是默认提前读取一个词法单元,存放在全局变量token中 ****************/

//调用词法分析器接口，取出一个合法的词法单元,存放在token全局变量中.
//取到非法词法单元，则调用Syntax_Error函数.
void Fetch_Token()	
{
	token = Get_Token();				//调用词法分析器接口，得到一个词法单元
	if(token.type == ERRTOKEN)			//非法字符调用Syntax_Error函数显示错误信息
		Syntax_Error(ERROR_ERRTOKEN);
}

//先匹配已提前读取出来的词法单元的类型是否和AToken匹配,否则语法错误
//然后再提前读取一个词法单元
void Match_Token(Token_Type AToken)		
{
	if(token.type != AToken)
		Syntax_Error(ERROR_MATCH);
	Fetch_Token();
}

//根据错误码打印出错信息并退出语法分析器
void Syntax_Error(int erron)			
{
	switch(erron)
	{
		case ERROR_ERRTOKEN:
			fprintf(stderr, "Syntax Error [ERRTOKEN]: %s\n", token.lexeme);
			break;
		case ERROR_MATCH:
			fprintf(stderr, "Syntax Error [MATCH]: %s\n", token.lexeme);
			break;
		case ERROR_STATEMENT:
			fprintf(stderr, "Syntax Error [Statement]: %s\n", token.lexeme);
			break;
		case ERROR_ATOM:
			fprintf(stderr, "Syntax Error [Atom]: %s\n", token.lexeme);
			break;
	}
	exit(EXIT_SYNTAX);
}


/**************  递归下降分析-建立语法树 ******************/

void Parser(char* SrcFilePtr)		//对SrcFilePtr指针所指向的文件进行语法分析
{
	if(Init_Scanner(SrcFilePtr) == -1)	//初始化词法分析器
	{
		fprintf(stderr, "Failed to open source file!\n");
		exit(EXIT_FAIL_OPEN);
	}

	Fetch_Token();		//获取第一个记号, 永远保持提前查看一个记号进行语法分析.
	Program();			//进入递归下降分析,遇到语法错误或文件结束将停止分析.
	Close_Scanner();	//关闭词法分析器
}

// Program ---> {Statement SEMICO }
void Program()						//递归下降分析-建立Program语法树
{
	while(token.type != NONTOKEN)		//如果不是源文件结束，就持续进行
	{
		if(token.type == ERRTOKEN)		//取到非空但非法的词法单元,进行错误处理
			Syntax_Error(ERROR_ERRTOKEN);

		//每次循环都分析一行语句
		Statement();
		Match_Token(SEMICO);
	}
}

// Statement ---> Origin_Statement | Scale_Statement | Rot_Statement | For_Statement
void Statement()					//递归下降分析-建立Statement语法树
{
	switch(token.type)	//根据提前读取的词法单元token的类型选择语法分析分支.
	{
		case ORIGIN:
			Origin_Statement();
			break;
		case SCALE:
			Scale_Statement();
			break;
		case ROT:
			Rot_Statement();
			break;
		case FOR:
			For_Statement();
			break;
		default:
			Syntax_Error(ERROR_STATEMENT);	//取到的词法单元不是Statement的开头.
			break;
	}	
}

//ForStatement ----> FOR T FROM Expression TO Expression STEP Expression DRAW L_BRACKET \
						Expression COMMA Expression R_BRACKET
void For_Statement()				//递归下降分析-建立For_Statement语法分析树
{
	ExprNode *start_ptr, *end_ptr, *step_ptr, *x_ptr, *y_ptr;

	printf("---> For_Statement()\n");
	Match_Token(FOR);
	Match_Token(T);
	Match_Token(FROM);
	start_ptr = Expression();
	Match_Token(TO);
	end_ptr = Expression();
	Match_Token(STEP);
	step_ptr = Expression();
	Match_Token(DRAW);
	Match_Token(L_BRACKET);
	x_ptr = Expression();
	Match_Token(COMMA);
	y_ptr = Expression();
	Match_Token(R_BRACKET);
	//语法分析完成, 下面可以进行语义分析
	//这里不进行具体的绘图操作，只是打印一条确认信息
	Print_Syntax_Tree(start_ptr,0);
	Print_Syntax_Tree(end_ptr,0);
	Print_Syntax_Tree(step_ptr,0);
	Print_Syntax_Tree(x_ptr,0);
	Print_Syntax_Tree(y_ptr,0);
	printf("For_Statement()--->\n");

}

// Origin_Statement ---> ORIGIN IS L_BRACKET Expression COMMA Expression R_BRACKET
void Origin_Statement()				//递归下降分析-建立Origin_Statement语法分析树
{
	ExprNode *x_ptr, *y_ptr;

	printf("---> Origin_Statement()\n");
	Match_Token(ORIGIN);
	Match_Token(IS);
	Match_Token(L_BRACKET);
	x_ptr = Expression();
	Match_Token(COMMA);
	y_ptr = Expression();
	Match_Token(R_BRACKET);

	//---	
	Print_Syntax_Tree(x_ptr,0);
	Print_Syntax_Tree(y_ptr,0);
	printf("Origin_Statement() --->\n");
}

// Rot_Statement ---> ROT IS Expression
void Rot_Statement()				//递归下降分析-建立Rot_Statement语法分析树
{
	ExprNode *rot_ptr;

	printf("---> Rot_Statement()\n");
	Match_Token(ROT);
	Match_Token(IS);
	rot_ptr = Expression();

	//---
	Print_Syntax_Tree(rot_ptr,0);
	printf("Rot_Statement() --->\n");
}

// Scale_Statement ---> SCALE IS L_BRACKET Expression COMMA Expression R_BRACKET
void Scale_Statement()				//递归下降分析-建立Scale_Statement语法分析树
{
	ExprNode *x_ptr, *y_ptr;

	printf("---> Scale_Statement()\n");
	Match_Token(SCALE);
	Match_Token(IS);
	Match_Token(L_BRACKET);
	x_ptr = Expression();
	Match_Token(COMMA);
	y_ptr = Expression();
	Match_Token(R_BRACKET);

	//---
	Print_Syntax_Tree(x_ptr,0);
	Print_Syntax_Tree(y_ptr,0);
	printf("Scale_Statement() --->\n");
}

/*********************** 表达式语法树分析 *****************************/
//每一个函数调用执行都要保证提前预取出一个新的词法单元来

// Expression ---> Term{(PLUS|MINUS)Term}
ExprNode* Expression()				//递归下降分析-建立Expression表达式语法树
{
	ExprNode *left, *right;
	Token_Type token_type_tmp;		//保存运算符的类型

	left = Term();			//调用Term函数,创建左子树-Term

	while(token.type == PLUS || token.type == MINUS) //如果再次取得的token为plus/minus,则继续
	{
		token_type_tmp = token.type;
		Fetch_Token();
		right = Term();
		left = MakeExprNode(token_type_tmp, left, right);
	}


	return left;
}

// Term ---> Factor{(MUL|DIV)Factor}
ExprNode* Term()					//递归下降分析-建立Term表达式语法树
{
	ExprNode *left, *right;
	Token_Type token_type_tmp;

	left = Factor();
	while(token.type == MUL || token.type == DIV)
	{
		token_type_tmp = token.type;
		Fetch_Token();
		right = Factor();
		left = MakeExprNode(token_type_tmp, left, right);
	}
	return left;
}

// Factor ---> (PLUS|MINUS)Factor|Component
ExprNode* Factor()					//递归下降分析-建立Factor表达式语法树
{
	ExprNode *left,*right;
	Token_Type token_type_tmp;

	if(token.type == PLUS || token.type == MINUS)	//是否为一元运算符
	{
		token_type_tmp = token.type;
		left = (ExprNode*)malloc(sizeof(ExprNode));	//一元运算符的左子树为0.
		if(left == NULL)
		{
			fprintf(stderr, "Failed to malloc memory in Factor().\n");
			exit(EXIT_MALLOC);
		}
		left->OpCode = CONST_ID;
		left->Content.CaseConst = 0;
		Fetch_Token();				//预取一个词法单元，用于下面的Factor()函数调用
		right = Factor();
		left = MakeExprNode(token_type_tmp, left, right);
	}
	else	// Component
	{
		return Component();
	}

	return left;
}

// Component ---> Atom [POWER Component]
ExprNode* Component()				//递归下降分析-建立Component表达式分析树
{
	ExprNode *left, *right;
	Token_Type token_type_tmp;

	left = Atom();
	if(token.type == POWER)
	{
		token_type_tmp = token.type;
		Fetch_Token();
		right = Component();
		left = MakeExprNode(token_type_tmp, left, right);
	}
	
	return left;
}

//Atom ---> CONST_ID | T | FUNC L_BRACKET Expression R_BRACKET | L_BRACKET Expression R_BRACKET
ExprNode* Atom()					//递归下降分析-建立Atom表达式分析树
{
	ExprNode *left, *right;
	Token_Type token_type_tmp;
	
	left = (ExprNode*)malloc(sizeof(ExprNode));
	if(left == NULL)
	{
		fprintf(stderr, "Failed to malloc memory in Atom().\n");
		exit(EXIT_MALLOC);
	}

	switch(token.type)
	{
		case CONST_ID:
			left->OpCode = CONST_ID;
			left->Content.CaseConst = token.value;
			Fetch_Token();
			return left;
		case T:
			left->OpCode = T;
			left->Content.CaseParmPtr = &parment_t;
			Fetch_Token();
			return left;
		case FUNC:
			left->OpCode = FUNC;
			left->Content.CaseFunc.MathFuncPtr = token.FuncPtr;
			Fetch_Token();
			left->Content.CaseFunc.child = Atom();
			return left;
		case L_BRACKET:
			Fetch_Token();
			left = Expression();
			Match_Token(R_BRACKET);
			return left;
		default:
			Syntax_Error(ERROR_ATOM);//语法错误,不符合Atom文法
			break;
	}
}

//根据参数和类型制作一个表达式子树并返回其根地址
//制作失败，则打印错误信息并退出语法分析器
ExprNode * MakeExprNode(Token_Type OpCode, void* arg1, void* arg2)
{
	ExprNode* node = (ExprNode*)malloc(sizeof(ExprNode));
	if(node == NULL)			//申请内存失败,则退出程序.
	{
		fprintf(stderr, "Failed to malloc memory for ExprNode.\n");
		exit(EXIT_MALLOC);
	}
	
	node->OpCode = OpCode;
	switch(OpCode)		//根据类型配置不同的部分的结构体
	{
		case CONST_ID:	//常数-表达式树
			node->Content.CaseConst = (double)(*((double*)arg1));
			break;
		case T:			//参数-表达式树
			node->Content.CaseParmPtr = &parment_t;	//保存全局变量T的地址
			break;
		case FUNC:		//数学函数-表达式树
			node->Content.CaseFunc.MathFuncPtr = (FuncPtr)arg1;
			node->Content.CaseFunc.child = (ExprNode*)arg2;
			break;
		default :		//二元运算-表达式树(一元运算也当二元运算处理)
			node->Content.CaseOperator.left = (ExprNode*)arg1;
			node->Content.CaseOperator.right= (ExprNode*)arg2;
			break;
	}
	return node;
}

#define INDENT_NUM	2		//缩进值步进量，子层与父层之间缩进INDENT_NUM个空格
//给出已经建立好的语法分析树的根节点
//按照深度优先的先序遍历将语法树的逻辑结构打印出来.
void Print_Syntax_Tree(ExprNode *root, int indent)
{
	int i = 0;

	if(root)
	{
		//先缩进空格
		for(i = 0; i < indent; i++)
			printf(" ");
		//再打印根的内容
		switch(root->OpCode)
		{
			case PLUS:
				printf("+\n");
				Print_Syntax_Tree(root->Content.CaseOperator.left,indent + INDENT_NUM);
				Print_Syntax_Tree(root->Content.CaseOperator.right,indent + INDENT_NUM);
				break;
			case MINUS:
				printf("-\n");
				Print_Syntax_Tree(root->Content.CaseOperator.left,indent + INDENT_NUM);
				Print_Syntax_Tree(root->Content.CaseOperator.right,indent + INDENT_NUM);
				break;
			case MUL:
				printf("*\n");
				Print_Syntax_Tree(root->Content.CaseOperator.left,indent + INDENT_NUM);
				Print_Syntax_Tree(root->Content.CaseOperator.right,indent + INDENT_NUM);
				break;
			case DIV:
				printf("/\n");
				Print_Syntax_Tree(root->Content.CaseOperator.left,indent + INDENT_NUM);
				Print_Syntax_Tree(root->Content.CaseOperator.right,indent + INDENT_NUM);
				break;
			case POWER:
				printf("**\n");
				Print_Syntax_Tree(root->Content.CaseOperator.left,indent + INDENT_NUM);
				Print_Syntax_Tree(root->Content.CaseOperator.right,indent + INDENT_NUM);
				break;
			case FUNC:
				printf("FUNC:%x\n",(unsigned int)root->Content.CaseFunc.MathFuncPtr);
				Print_Syntax_Tree(root->Content.CaseFunc.child,indent + INDENT_NUM);
				break;
			case CONST_ID:
				printf("%.4lf\n",root->Content.CaseConst);
				break;
			case T:
				printf("T\n");
				break;
		}
	}
}
