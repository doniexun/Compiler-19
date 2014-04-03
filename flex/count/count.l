%{
   #include "stdio.h"
   #include "stdlib.h"
   int num_num=0,num_id=0;
%}
INTEGER [-+]?[1-9][0-9]*
ID [a-zA-Z][a-zA-Z_0-9]*
SPACE [ /n/t]
%%
{INTEGER} { num_num++;
  printf("(num=%d)\n",atoi(yytext));/*打印数字值*/
    /*数字数加一*/
}

{ID} { num_id++;
    printf("(id=%s)\n",yytext);
    }

{SPACE} |     /* '|'表示space模式采用的动作同下一个模式. '.'表示匹配'\n'以外的任意字符 */
. {
   /*什么也不做,滤掉白字符和其它字符*/
   }
%%

int main()
{
   yylex(); //执行一遍词法分析程序
   printf("num=%d,id=%d\n",num_num,num_id);
   return 0;
}

int yywrap()	//此函数必须由用户提供,该函数返回非0值，则yylex()返回0而结束，否则yylex()继续扫描.
{
    return 1; //返回非0值，停止扫描并返回
}
