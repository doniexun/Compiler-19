%{
	
%}

%token

%%


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
