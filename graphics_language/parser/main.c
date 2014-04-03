/*************************************************************************
    > File Name: main.c
    > Author: Shawn Guo
    > Mail: iguoxiaopeng@gmail.com 
    > Created Time: 2013年12月09日
    > Last Changed: 
    > Notes:	语法分析器的测试主程序
    >			Usage: parse source_file	
*************************************************************************/

#include <stdio.h>
#include "parser.h"
int main(int argc, char *argv[])
{
	Token token;

	//检查参数
	if(argc < 2)
	{
		fprintf(stderr, "Usage: %s source_file\n", argv[0]);
		return -1;
	}

	//初始化语法分析器,其他工作交给语法分析器做
	Parser(argv[1]);
	return 0;
}

