
/* 定义部分 */

%{
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"

static Token Token_Table[] =			//存放<静态>词法单元的数据结构
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

%}

/*regular definitions*/

delim					[ \t\n\r]
ws						{delim}+
letter					[A-Za-z]
digit					[0-9]
id						{letter}({letter}|{digit})*
number					{digit}+(\.{digit}+)?(E[+-]?{digit}+)?
semico					[;]
l_bracket				[(]
r_bracket				[)]
comma					[,]
plus					[+]
minus					"-"
mul						"*"
div						"/"
power					"**"
const_id				{digit}+(\.{digit}*)?

%%

{ws}		{/* no action and no return */}
"//".*		{/* eat all comments. */}
"--".*		{/* eat all comments. */}

ORIGIN		{}
SCALE		{printf("SCALE    \t\t%10s\t\t0\t\tNULL\n", yytext);}
ROT			{printf("ROT      \t\t%10s\t\t0\t\tNULL\n", yytext);}
IS			{printf("IS       \t\t%10s\t\t0\t\tNULL\n", yytext);}
TO			{printf("TO       \t\t%10s\t\t0\t\tNULL\n", yytext);}
STEP		{printf("STEP     \t\t%10s\t\t0\t\tNULL\n", yytext);}
DRAW		{printf("DRAW     \t\t%10s\t\t0\t\tNULL\n", yytext);}
FOR			{printf("FOR      \t\t%10s\t\t0\t\tNULL\n", yytext);}
FROM		{printf("FROM     \t\t%10s\t\t0\t\tNULL\n", yytext);}

T			{printf("T        \t\t%10s\t\t0\t\tNULL\n", yytext);}

{semico}	{printf("SEMICO   \t\t%10s\t\t0\t\tNULL\n", yytext);}
{l_bracket}	{printf("L_BRACKET\t\t%10s\t\t0\t\tNULL\n", yytext);}
{r_bracket}	{printf("R_BRACKET\t\t%10s\t\t0\t\tNULL\n", yytext);}
{comma}		{printf("COMMA    \t\t%10s\t\t0\t\tNULL\n", yytext);}

{plus}		{printf("PLUS     \t\t%10s\t\t0\t\tNULL\n", yytext);}
{minus}		{printf("MINUS    \t\t%10s\t\t0\t\tNULL\n", yytext);}
{mul}		{printf("MUL      \t\t%10s\t\t0\t\tNULL\n", yytext);}
{div}		{printf("DIV      \t\t%10s\t\t0\t\tNULL\n", yytext);}
{power}		{printf("POWER    \t\t%10s\t\t0\t\tNULL\n", yytext);}

PI			{printf("CONST_ID \t\t%10s\t\t3.1415926\tNULL\n", yytext);}
E			{printf("CONST_ID \t\t%10s\t\t2.71828\t\tNULL\n", yytext);}

sin			{printf("FUNC_SIN \t\t%10s\t\t0\t\tsin_address\n", yytext);}
cos			{printf("FUNC_COS \t\t%10s\t\t0\t\tcos_address\n", yytext);}
tan			{printf("FUNC_TAN \t\t%10s\t\t0\t\ttan_address\n", yytext);}
log			{printf("FUNC_LOG \t\t%10s\t\t0\t\tlog_address\n", yytext);}
exp			{printf("FUNC_EXP \t\t%10s\t\t0\t\texp_address\n", yytext);}
sqrt		{printf("FUNC_SQRT\t\t%10s\t\t0\t\tsqrt_address\n", yytext);}

{const_id}	{printf("CONST_ID \t\t%10s\t\t%-5s\t\tNULL\n", yytext, yytext);}

{id}	{printf("ERROR    \t\t%10s\t\t0\t\tNULL\n", yytext);}

%%

int main(int argc, char *argv[])
{
	//参数检查
	if(argc < 2)
	{
		fprintf(stderr,"Usage: %s source_file.\n", argv[0]);
		return -1;
	}

	//打开输入文件
	//yyin为lex词法生成器的输入文件，默认stdin.
	if((yyin = fopen(argv[1], "r")) == NULL)
	{
		fprintf(stderr,"Error: Can't open %s!\n", argv[1]);
		return -1;
	}
	printf("_________________________________________________________________________________\n");
	yylex();
	printf("_________________________________________________________________________________\n");
	return 0;
}

int yywrap()
{
	return 1;
}
