%{
	/*
	    * count the non-comment lines in a file.
		 * Author: Gavin Xu
		  */
	int ncsl_lines, comment_lines, macro_lines;

	int start=1;
%}



opencom \/\*
closcom \*\/

%start COMMENT
%start P

%%
BEGIN P;
{opencom}    { BEGIN COMMENT; }
<COMMENT>. { }
<COMMENT>\n { comment_lines++; }
<COMMENT>{closcom}    { BEGIN P; }
<P>switch.*;    { ncsl_lines++; }
<P>switch.*\n    { ncsl_lines++; }
<P>for.*;    { ncsl_lines++; }
<P>for.*\n    { ncsl_lines++; }
<P>if.*;    { ncsl_lines++; }
<P>if.*\n    { ncsl_lines++; }
<P>[ \t]*else[ \t]*\n {}
<P>while.*;    { ncsl_lines++; }
<P>while.*\n    { ncsl_lines++; }
<P>case.*[0-9a-zA-Z_]+.*: |
<P>default[ \t]*:    { ncsl_lines++; }
<P>[ \t]*\{[ \t]*\n    { }
<P>[ \t]*\}[ \t]*\n    { }
<P>.*[0-9a-zA-Z_]+.*;\n { ncsl_lines++; }
<P>\/\/.*\n { comment_lines++;}
<P>[ \t]*[0-9a-zA-Z_]+\(.*\).*\n { }
<P>^#define.*\n { macro_lines++;}
<P>^#include.*\n { macro_lines++; }
<P>^#ifdef.*\n { macro_lines++; }
<P>^#ifndef.*\n { macro_lines++; }
<P>^#else.*\n { }
<P>^#endif.*\n { }
<P>. { }
\n
%%

main(argc,argv)
	int argc;
	char *argv[];
{

	ncsl_lines = 0;
	comment_lines = 0;
	macro_lines = 0;


	++argv, --argc; /* skip over program name */
	if ( argc > 0 )
	{
		yyin = fopen( argv[0], "r" );
		if ( !yyin ) return -1;

	}
	else
		yyin = stdin;


	yylex();

	printf("\ncounting ncsl lines of file: %s: \n",argv[0]);

	printf("comment lines: %d\n", comment_lines);
	printf("ncsl lines: %d\n", ncsl_lines);
	printf("macro lines: %d\n", macro_lines);
	printf("\n");
}


int yywrap()    //此函数必须由用户提供,该函数返回非0值，则yylex()返回0而结束，否则yylex()继续扫描.
{
     return 1; //返回非0值，停止扫描并返回
}

