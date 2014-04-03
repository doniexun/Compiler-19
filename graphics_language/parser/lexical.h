/************************************************************************
    > File Name: Lexical.h
    > Author: Shawn Guo
    > Mail: iguoxiaopeng@gmail.com 
    > Created Time: 2013年11月
    > Last Changed: 
    > Notes: 为绘图语言构造一个词法分析器
	>		 用词法分析器识别其中的记号，并将各记号的信息显示出来 
	>		 词法分析器分为<扫描阶段>和<词法分析阶段>
*************************************************************************/

#ifndef __LEXICAL_H__
#define	__LEXICAL_H__

#include<math.h>

#define	TRUE	1
#define	FALSE	0

/******************************* 数据结构定义 **************************************/
enum Token_Type	//词法单元的模式
{
	//保留字
	ORIGIN,SCALE,ROT,IS,TO,STEP,DRAW,FOR,FROM,
	//参数
	T,
	//分隔符号
	SEMICO,L_BRACKET,R_BRACKET,COMMA,
	//运算符
	PLUS,MINUS,MUL,DIV,POWER,
	//函数(调用)
	FUNC,
	//常数
	CONST_ID,
	//空记号(源程序结束)
	NONTOKEN,
	//出错记号(非法输入)
	ERRTOKEN
};
typedef enum Token_Type Token_Type;

struct Token	//词法单元的数据结构 <单元名，属性值>
{
	Token_Type	type;					//词法单元的模式/类别
	char*		lexeme;					//属性, 原始输入的字符串

	double		value;					//属性, 若词法单元为常数则是常数值
	double		(* FuncPtr)(double);	//属性,	若词法单元为函数则是函数指针
};
typedef struct Token Token;


/******************************* 词法分析器接口定义 **************************************/

//打开输入文件file_name, 初始化词法分析器，准备进行词法分析
//成功返回0,失败返回-1.
int Init_Scanner(const char* file_name);

//调用此函数，则进行一次词法分析，输出一个匹配的词法单元
Token Get_Token(void);

//关闭输入文件，撤销词法分析器.
void Close_Scanner(void);

/******************************* 全局变量声明 **************************************/
extern FILE *FP;	//全局文件指针变量, 指向打开的文件.
extern int DFA_STATE ; //初始化状态为起始状态0;
//初始化状态为起始状态0;


extern char CHAR_BUFFER[]; //用于输入字符的缓冲区
extern int CHAR_FLAG ;          //缓冲区索引
extern int POINT_FLAG;         //小数点标记, 遇到小数点标记为1, 用于检测多小数点的错误
extern Token Token_Table[];        //存放<静态>词法单元的数据结构
extern Token token;					//全局变量token

#endif
