/************************************************************************
    > File Name: Lexical.c
    > Author: Shawn Guo
    > Mail: iguoxiaopeng@gmail.com 
    > Created Time: 2013年11月23日 星期六 17时33分21秒
    > Last Changed: 
    > Notes: 为绘图语言构造一个词法分析器
	>		 用词法分析器识别其中的记号，并将各记号的信息显示出来 
	>		 词法分析器分为<扫描阶段>和<词法分析阶段>
*************************************************************************/

#include<stdio.h>
#include<math.h>
#include<ctype.h>
#include<string.h>
#include"lexical.h"

#define	BUFFER_NUM	50

/******************************* 全局变量定义 **************************************/

FILE *FP = NULL; //全局文件指针变量, 指向打开的文件.

int DFA_STATE = 0;	//初始化状态为起始状态0;
//具体状态编号参考对应的DFA图.

char CHAR_BUFFER[20] = {0}; //用于输入字符的缓冲区
int CHAR_FLAG = 0;			//缓冲区索引
int POINT_FLAG = 0;			//小数点标记, 遇到小数点标记为1, 用于检测多小数点的错误

Token token = {};			//全局变量token.
Token Token_Table[] =			//存放<静态>词法单元的数据结构
{
	//常量
	{CONST_ID,	"PI",	3.1415926,	NULL},	//0
	{CONST_ID,	"E",	2.71828,	NULL},	//1
	//参数
	{T,			"T",	0.0,		NULL},	//2
	//函数
	{FUNC,		"SIN",	0.0,		sin},	//3
	{FUNC,		"COS",	0.0,		cos},	//4	
	{FUNC,		"TAN",	0.0,		tan},	//5
	{FUNC,		"LN",	0.0,		log},	//6
	{FUNC,		"EXP",	0.0,		exp},	//7
	{FUNC,		"SQRT",	0.0,		sqrt},	//8
	//关键字
	{ORIGIN,	"ORIGIN",	0.0,	NULL},	//9
	{SCALE,		"SCALE",	0.0,	NULL},	//10
	{ROT,		"ROT",		0.0,	NULL},	//11
	{IS,		"IS",		0.0,	NULL},	//12
	{FOR,		"FOR",		0.0,	NULL},	//13
	{FROM,		"FROM",		0.0,	NULL},	//14
	{TO,		"TO",		0.0,	NULL},	//15
	{STEP,		"STEP",		0.0,	NULL},	//16
	{DRAW,		"DRAW",		0.0,	NULL},	//17
	//分隔符号	
	{SEMICO,	";",		0.0,	NULL},	//18
	{L_BRACKET,	"(",		0.0,	NULL},	//19
	{R_BRACKET,	")",		0.0,	NULL},	//20
	{COMMA,		",",		0.0,	NULL},	//21
	//运算符号
	{PLUS,		"+",		0.0,	NULL},	//22
	{MINUS,		"-",		0.0,	NULL},	//23
	{MUL,		"*",		0.0,	NULL},	//24
	{DIV,		"/",		0.0,	NULL},	//25
	{POWER,		"**",		0.0,	NULL},	//26
};


/******************************* 词法分析器接口定义 **************************************/

//打开输入文件file_name, 初始化词法分析器，准备进行词法分析
//成功返回0，失败返回错误码-1.
int Init_Scanner(const char* file_name)
{
	if((FP = fopen(file_name, "r")) == NULL)
	{
		//打开失败,返回错误码给调用者
		return -1;
	}
	else 
		return 0;
}

char Next_Char() //从文件中读取一个字符
{
	if(FP != NULL)
		return fgetc(FP);
	else
		return EOF;
}

int Retract() //将文件指针FP回退一个位置,成功返回0，失败返回错误码-1.
{
	if(FP != NULL && (fseek(FP, -1, SEEK_CUR) == 0))
		return 0;
	else
		return -1;
}

//调用此函数，则进行一次词法分析，输出一个匹配的词法单元
Token Get_Token(void)
{
	char ch;
	CHAR_FLAG = 0;
	
	while(TRUE)	//不断读取下一个字符, 根据DFA选择相应状态，直到达到某一个接受状态
	{
		ch = Next_Char();

		switch(DFA_STATE)
		{
			case 0:	//开始状态, 则根据ch的值进行状态分支的选择
				{
					CHAR_FLAG = 0;
					POINT_FLAG = 0;

					if(ch == EOF) //开始状态就遇到EOF,说明文件结束
					{
						token.type = NONTOKEN;
						return token;
					}
					if(isalpha(ch)) //字母分支
					{
						CHAR_BUFFER[CHAR_FLAG++] = ch; //将字符暂时存放在缓冲区
						DFA_STATE = 1;
						break;
					}
					else if(isdigit(ch)) //数字分支
					{
						CHAR_BUFFER[CHAR_FLAG++] = ch; //将字符暂时存放在缓冲区
						DFA_STATE = 2;
						break;
					}
					else
					{
						switch(ch)
						{
							case '*':DFA_STATE = 4;break;
							case '/':DFA_STATE = 6;break;
							case '-':DFA_STATE = 7;break;
							case '+':return Token_Table[22];break;
							case ',':return Token_Table[21];break;
							case ';':return Token_Table[18];break;
							case '(':return Token_Table[19];break;
							case ')':return Token_Table[20];break;
							//开始状态时遇到空白字符[" "|\t|\n|\r],单纯的跳过
							case ' ':case '\t':case '\n':case '\r':break;
							default://其他字符,一律视为非法输出
								token.type = ERRTOKEN;
								return token;
						}
					}
				}
				break;
			case 1:	//可接受状态： ID, 存放在缓冲区中，不断读取，直到非字母非数字的字符出现
				{
					if(isalpha(ch)) //字母分支
					{
						CHAR_BUFFER[CHAR_FLAG++] = ch; //将字符暂时存放在缓冲区
						DFA_STATE = 1;
						break;
					}
					else if(isdigit(ch)) //数字分支
					{
						CHAR_BUFFER[CHAR_FLAG++] = ch; //将字符暂时存放在缓冲区
						DFA_STATE = 2;
						continue;
					}
					else //此次输入为非ID字符，则ID匹配终止.
					{
						//1. 将文件指针回退一个位置，重置状态为开始状态
						Retract();
						DFA_STATE = 0;
						//2. 将缓冲区的ID进行匹配(因为本语言的ID是固定的，没有其他变量ID)
						CHAR_BUFFER[CHAR_FLAG] = '\0';
						if(CHAR_FLAG == 1)
						{
							if(strcasecmp(CHAR_BUFFER, "T") == 0)
							{
								//匹配词法单元<参数T>
								return Token_Table[2];
							}
							else if(strcasecmp(CHAR_BUFFER, "E") == 0)
							{
								//匹配词法单元<常数E>
								return Token_Table[1];
							}
							else 
							{
								//其他的一个字符ID，均为错误
								token.type = ERRTOKEN;
								return token;
							}
						}
						else if(CHAR_FLAG ==  2)
						{
							if(strcasecmp(CHAR_BUFFER, "TO") == 0)
								return Token_Table[15];
							else if(strcasecmp(CHAR_BUFFER, "IS") == 0)
								return Token_Table[12];
							else if(strcasecmp(CHAR_BUFFER, "PI") == 0)
								return Token_Table[0];
							else 
							{
								//其他的二字符ID，均为错误
								token.type = ERRTOKEN;
								token.lexeme = CHAR_BUFFER;
								return token;
							}
						}
						else if(CHAR_FLAG == 3)
						{
							if(strcasecmp(CHAR_BUFFER, "ROT") == 0)
								return Token_Table[11];
							else if(strcasecmp(CHAR_BUFFER, "FOR") == 0)
								return Token_Table[13];
							else if(strcasecmp(CHAR_BUFFER, "SIN") == 0)
								return Token_Table[3];
							else if(strcasecmp(CHAR_BUFFER, "COS") == 0)
								return Token_Table[4];
							else if(strcasecmp(CHAR_BUFFER, "TAN") == 0)
								return Token_Table[5];
							else if(strcasecmp(CHAR_BUFFER, "LOG") == 0)
								return Token_Table[6];
							else if(strcasecmp(CHAR_BUFFER, "EXP") == 0)
								return Token_Table[7];
							else
							{
								//其他的三字符ID，均为错误
								token.type = ERRTOKEN;
								token.lexeme = CHAR_BUFFER;
								return token;
							}
						}
						else if(CHAR_FLAG == 4)
						{
							if(strcasecmp(CHAR_BUFFER, "STEP") == 0)
								return Token_Table[16];
							else if(strcasecmp(CHAR_BUFFER, "DRAW") == 0)
								return Token_Table[17];
							else if(strcasecmp(CHAR_BUFFER, "FROM") == 0)
								return Token_Table[14];	
							else if(strcasecmp(CHAR_BUFFER, "SQRT") == 0)
								return Token_Table[8];	
							else
							{
								//其他的4字符ID，均为错误
								token.type = ERRTOKEN;
								token.lexeme = CHAR_BUFFER;
								return token;
							}
						}
						else if(strcasecmp(CHAR_BUFFER, "SCALE") == 0)
							return Token_Table[10];
						else if(strcasecmp(CHAR_BUFFER, "ORIGIN") == 0)
							return Token_Table[9];
						else	//与所有关键字都不匹配的ID, 则为输入错误
						{
							token.type = ERRTOKEN;
							token.lexeme = CHAR_BUFFER;
							return token;
						}
					}
				}
				break;
			case 2:	//常数匹配, 数字都以字符的形式存放在缓冲区中
				{
					if(isdigit(ch)) //仍为数字,继续
					{
						CHAR_BUFFER[CHAR_FLAG++] = ch; //将字符暂时存放在缓冲区
						DFA_STATE = 2;
						break;
					}
					else if(ch == '.')
					{
						if(POINT_FLAG == 0)
						{
							POINT_FLAG = CHAR_FLAG; //标记小数点的位置
							CHAR_BUFFER[CHAR_FLAG++] = ch; //将字符暂时存放在缓冲区
							DFA_STATE = 2;
							break;
						}
						else
						{
							//多个小数点,错误
							token.type = ERRTOKEN;
							token.lexeme = CHAR_BUFFER;
							fprintf(stderr, "Error: 多小数点错误!\n");
							return token;
						}
					}
					else	//读取到其他非数字字符,则回退并返回数字词法单元
					{
						//1. 将文件指针回退一个位置，重置状态为开始状态
						Retract();
						DFA_STATE = 0;
						//2. 将缓冲区的数字进行转换
						CHAR_BUFFER[CHAR_FLAG] = '\0';
						if(POINT_FLAG != 0) //有小数点的数据转换
						{
							int zhengshu = 0, xiaoshu = 0;
							int i = CHAR_FLAG - 1 - POINT_FLAG; //小数部分的位数

							token.type = CONST_ID;
							CHAR_BUFFER[POINT_FLAG] = '\0';

							if(i == 0) //有小数点但是没有小数部分
							{
								token.value = (double)(atoi(CHAR_BUFFER));
								return token;
							}
							//真正有小数部分需要转换的
							zhengshu = atoi(CHAR_BUFFER);
							xiaoshu = atoi(&CHAR_BUFFER[POINT_FLAG + 1]);
							token.value = (double)(zhengshu) + ((double)xiaoshu / (pow(10.0, i)));
							return token;
						}
						else //无小数点的数据转换
						{
							token.type = CONST_ID;
							token.value = (double)(atoi(CHAR_BUFFER));
							return token;
						}
					}
				}
				break;
			case 4: 
				{
					if(ch == '*')
					{
						// 读取两个*，可接受状态
						DFA_STATE = 0;
						return Token_Table[26];
					}
					else //确定为*而非**
					{
						//1. 将文件指针回退一个位置，重置状态为开始状态
						Retract();
						DFA_STATE = 0;
						//2. 返回
						return Token_Table[24];
					}
				}
				break;
			case 6:
				{
					if(ch == '/')	//读取两个/,可接受状态，注释
					{
						//直接读取这一行剩余所有字符串，并全部抛弃
						while((ch = Next_Char()) != '\n');
						//重置状态为开始状态
						DFA_STATE = 0;
						break;
					}
					else //确定为除号
					{
						//1. 将文件指针回退一个位置，重置状态为开始状态
						Retract();
						DFA_STATE = 0;
						//2. 返回
						return Token_Table[25];
					}
				}
				break;
			case 7:
				{
					if(ch == '-')	//读取两个-， 可接受状态，注释
					{
						//直接读取这一行剩余所有字符串，并全部抛弃
						while((ch = Next_Char()) != '\n');
						//重置状态为开始状态
						DFA_STATE = 0;
						break;
					}
					else //确定为负号
					{
						//1. 将文件指针回退一个位置，重置状态为开始状态
						Retract();
						DFA_STATE = 0;
						//2. 返回
						return Token_Table[23];
					}	
				}
				break;
		}
	}
}

//关闭输入文件，撤销词法分析器.
void Close_Scanner(void)
{
	fclose(FP);
}
