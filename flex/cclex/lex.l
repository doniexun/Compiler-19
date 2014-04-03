
/* 定义部分 */

%{
	#include <stdio.h>
	#include <stdlib.h>

%}

/* regular definitions */
delim	[ \t\n]
ws		{delim}+
letter	[A-Za-z]
digit	[0-9]
id		{letter}({letter}|{digit})*
number	{digit}+(\.{digit}+)?(E[+-]?{digit}+)?


%%

{ws}		{/* no action and no return */}

auto|break|case|char|const|continue|default|do|double |
else|enum|extern|float|for|goto|if|int|long|register|return |
short|signed|sizeof|static|struct|switch|typedef|union|unsigned |
void|volatile|while	{
	printf("KeyWord:	%s\n", yytext);
}


{id}		{
	printf("Identifier:	%s\n",yytext);
}

{number}	{
	printf("Number: %s\n",yytext);
}

"("|")"|"["|"]"|"->"|"."|"!"|"~"|"++"|"--"|"-"|"*" |
"&"|"/"|"%"|"+"|"<<"|">>"|"<"|"<="|">"|">="|"=="|"!="|"^" |
"|"|"&&"|"||"|"?"|"="|"+="|"-="|"*="|"/="|"%="|"^="|"|=" |
"<<="|">>="|","		{
	printf("Operators:	%s\n",yytext);
}

"{"|"}"	{
	printf("{}\n");
}


.	{
	printf("other...\n");
}




%%

int main()
{
	yylex();
	return 0;
}

int yywrap()
{
	return 1;
}
