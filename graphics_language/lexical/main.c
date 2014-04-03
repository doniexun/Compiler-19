/*************************************************************************
    > File Name: main.c
    > Author: Shawn Guo
    > Mail: iguoxiaopeng@gmail.com 
    > Created Time: 2013年12月08日 星期日 19时28分02秒
    > Last Changed: 
    > Notes: 
*************************************************************************/

#include <stdio.h>
#include "lexical.h"
   
int main(int argc, char *argv[])
{
	Token token;

	//检查参数
	if(argc < 2)
	{
		fprintf(stderr, "Usage: %s source_file\n", argv[0]);
		return -1;
	}

	//利用参数文件初始化词法分析器
	if(Init_Scanner(argv[1]) == -1)
	{
		fprintf(stderr, "Error: Can't open %s\n", argv[1]);
		return -1;
	}

	printf("词法单元模式\t\t字符串\t\t常数值\t\t函数指针\n");
	printf("____________________________________________________________________\n");

	while(TRUE) //一直循环进行词法分析,直到文件结束符.
	{
		token = Get_Token();		//通过词法分析器获得一个词法单元

		//这里不进行处理, 仅打印至stdout
		if(token.type != NONTOKEN)		//如果不是结束符号,则打印此数据结构;否则退出循环.
		{
			printf("%d\t\t\t%s\t\t%f\t\t%x\n",token.type, token.lexeme, token.value,
					(unsigned int)(token.FuncPtr));
		}
		else 
		{
			printf("-----------------End Of Source File --------------------\n");
			break;
		}
	}

	Close_Scanner();	//关闭词法分析器
	printf("____________________________________________________________________\n");

	return 0;
}

