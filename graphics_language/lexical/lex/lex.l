
/* 定义部分 */

%{
	#include <stdio.h>
	#include <stdlib.h>

%}

/* regular definitions */
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

ORIGIN		{printf("ORIGIN   \t\t%10s\t\t0\t\tNULL\n", yytext);}
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
