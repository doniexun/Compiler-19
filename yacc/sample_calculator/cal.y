%{
#include<ctype.h>	
#include<stdio.h>
#define YYSTYPE	int	/* double type for Yacc stack */
%}

%token NUMBER
%left '+' '-'
%left '*' '/'
%right UMINUS

%%

lines	:	lines expr '\n'			{printf("%d\n", $2);}
		|	lines '\n'
		|	/*empty*/
		|	error '\n'		{ yyerror("reenter previous line:"); yyerrok;}
		;

expr : expr '+' expr	{$$ = $1 + $3;}
	 | expr '-' expr	{$$ = $1 - $3;}
	 | expr '*' expr	{$$ = $1 * $3;}
	 | expr '/' expr	{$$ = $1 / $3;}
	 | '(' expr ')'		{$$ = $2;}
	 | '-' expr %prec UMINUS	{$$ = -$2;}
	 | NUMBER
	 ;
	 

%%

int yyerror(char const *message)
{
     fputs(message, stderr);
     fputc('\n',stderr);
     return 0;
}

void main()
{
	yyparse();
}
