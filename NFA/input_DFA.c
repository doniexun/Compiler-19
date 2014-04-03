/*************************************************************************
    > File Name: input_DFA.c
    > Author: Shawn Guo
    > Mail: iguoxiaopeng@gmail.com 
    > Created Time: 2013年11月26日 星期二 08时27分32秒
    > Last Changed: 2013/12/3
    > Notes:	设计数据结构,用来存储DFA的状态表.
	>			根据输入不断构建状态表
	>			Usage: argv[0] status_num element_num
	>			实现基本的字符串匹配!
*************************************************************************/

#include<stdio.h>
#include<string.h>
//#include<malloc.h>
#include <stdlib.h>

#define	STRING_BUFFER_NUM 100	//用于存放匹配字符串输入的缓冲区

#define STATUS_MAX_NUM	10
#define	ELEMENT_MAX_NUM 10
int status_num;				//状态总数, status_0为开始状态start
int element_num;			//词法基本元素总数

struct node		//存储状态的数据结构
{
	int status_id;		//状态标号

	//array[1][2] = 1 表示status_id对应的状态中, 输入第1号元素,可以跳转找状态2中去.
	//array[1][2] = 0 表示无法经过元素1到达状态2.
	//int array[ELEMENT_NUM][STATUS_NUM];
	int **array;			//DFA的离开状态固定，但为了和NFA的数据结构兼容，这里用NFA的数据结构
};

typedef struct node Node;

//char alphabet[ELEMENT_NUM];		//程序按元素0～元素element_num - 1计算
									//交互时需要将元素转换为用户给定的字母
char *alphabet;

//标准格式打印状态表
void Print_Map();

//my_node对应状态表中的模式匹配, 从status_start开始, 寻找匹配字符ch的终点status_end并返回。
//失败返回-1
int Match_Ch(Node *my_node, int status_start, char ch);
//寻找匹配string字符串的终点status_end并返回
//失败返回-1
int Match_String(Node *my_node, int status_start, char *string);

int main(int argc, char *argv[])
{
//	Node my_node[STATUS_NUM];
	Node *my_node;
	char string[STRING_BUFFER_NUM];

	int i = 0, j = 0, k = 0;
	char ch;
	
	//根据用户的参数,初始化状态表
	if(argc != 3)
	{
		fprintf(stderr, "Usage: %s status_num element_num\n",argv[0]);
		fprintf(stderr, "status_num should greater than 1 because of (start_status 0)\n");
		return -1;
	}
	else
	{
		status_num = atoi(argv[1]);
		element_num = atoi(argv[2]);
		if(status_num < 1 || status_num > STATUS_MAX_NUM || \
		   element_num < 1 || element_num > ELEMENT_MAX_NUM)
		{
			fprintf(stderr,"Error: 1 < status_num < %d , 1 < element_num < %d\n",
					STATUS_MAX_NUM, ELEMENT_MAX_NUM);
			return -1;
		}

		//真正的初始话工作
		if((alphabet = (char*)malloc(element_num * sizeof(char))) == NULL)
		{
			fprintf(stderr, "Error: Failed to malloc.\n");
			return -1;
		}

		if((my_node = (Node *)malloc(status_num * sizeof (Node))) == NULL)
		{
			fprintf(stderr, "Error: Failed to malloc.\n");
			return -1;
		}
		{
			int i,j,k;

			for(i = 0; i < status_num; i++)
			{
				my_node[i].status_id = i;

				if((my_node[i].array = (int **)malloc(element_num * sizeof(int *))) == NULL)
				{
					fprintf(stderr, "Error: Failed to malloc.\n");
					return -1;
				}
				for(j = 0; j < element_num; j++)
				{
					if((my_node[i].array[j] = (int *)malloc(status_num * sizeof(int))) == NULL)
					{
					fprintf(stderr, "Error: Failed to malloc.\n");
					return -1;
					}
					for(k = 0; k < status_num; k++)
					{
						my_node[i].array[j][k] = -1; //-1表示无.
					}
				}
			}
		}
	}
/*
	//初始化状态表
	for(i = 0; i < STATUS_NUM; i++)
	{
		my_node[i].status_id = i;
		for(j = 0; j < ELEMENT_NUM; j++)
			for(k = 0; k < STATUS_NUM; k++)
				my_node[i].array[j][k] = -1;
	}
*/

	printf("The num of Status: %d\n", status_num);
	printf("The num of Element: %d\n", element_num);
	printf("Please input elements:\n\n");
	printf("-----------------------------------------------------\n");
	for(i = 0; i < element_num; i++)
	{	
		printf("Element No.%d:", i);
		alphabet[i] = getchar();
		getchar();					//eat '\n'
	}

	printf("-----------------------------------------------------\n");
	for(i = 0; i < element_num; i++)
	{	
		printf("Element No.%d is %c\n", i, alphabet[i]);
	}

	printf("-----------------------------------------------------\n");
	printf("Input the DFA Map use this : <status_from, element, status_to>.\n");
	printf("eg. <0,a,1> \n");
	printf("Input EOF to end input.\n");
	while(1)
	{
		i = scanf("<%d,%c,%d>", &j, &ch, &k);
		if(i == 3)	//成功读取
		{
			getchar();		// eat '\n' or ' '
			//进行输入范围判断
			if((j < 0 || j >= status_num) || (k < 0 || k >= status_num))
			{
				printf("Input Error: status_id (0~%d).\n", status_num - 1);
				continue;
			}
			
			for(i = 0; i < element_num; i++)
				if(ch == alphabet[i])
					break;
			if(i == element_num)	//没有对应元素
			{
				printf("Input Error: Can't match this element:	[ %c ].\n", ch);
				continue;
			}

			//输入没有问题，配置状态表,此时i为对应字符ch的数字标号
			printf("You input <status_%d,element_%d,status_%d>\n", j, i, k);
			my_node[j].array[i][k] = 1;
		}
		else if(i == EOF)
			break;
		else	//读取失败
		{
			printf("Input Error!\n");
			printf("Input the DFA Map use this : <status_from, element, status_to>.\n");
			printf("eg. <0,a,1> \n");
			//fflush(stdin);	//gcc无法不支持此语句，此语句为c的扩充，部分编译器支持(VC)
			{
				int c;
				while ((c = getchar()) != '\n' && c != EOF);
			}

			continue;
		}
	}

	//输入完毕，得到完整的状态表
	Print_Map(my_node);

	//模式匹配
	while(1)
	{
		printf("Input a string to match.\n");
		scanf("%s",string);
		printf("String length:%d\n", strlen(string));
		if(Match_String(my_node, 0, string) == -1)
			break;
	}

    return 0;
}

//标准格式打印状态表
void Print_Map(Node *my_node)
{
	int i,j,k;

	printf("*******************************Status Map*************************\n");
	printf("_________________________________________________________________\n");
	printf("Status|");
	for(i = 0; i < element_num; i++){
		printf("%c",alphabet[i]);
		for(j = 1; j < status_num; j++)
			printf(" ");
		printf("|");
	}
	printf("\n");
	
	for(i = 0; i < status_num; i++)
	{
		printf("  %2d  |",i);
		for(j = 0; j < element_num; j++)
		{
			for(k = 0; k < status_num; k++)
				if(my_node[i].array[j][k] != -1)
					printf("%d",k);
				else
					printf(" ");
			printf("|");
		}
		printf("\n");
	}

	printf("______|__________________________________________________________\n");
}

//my_node对应状态表中的模式匹配, 从status_start开始, 寻找匹配字符ch的终点status_end并返回。
//失败返回-1
int Match_Ch(Node *my_node, int status_start, char ch)
{
	int alphabet_id, i;
	int *status_p;

	if(ch == '\0')
		return status_start;

	//查找是否有该元素
	for(alphabet_id = 0; alphabet_id < element_num; alphabet_id++)
	{
		if(ch == alphabet[alphabet_id])
			break;
	}

	if(alphabet_id == element_num)
		return -1;

	//确实有该元素,则查找该状态的状态表.
	status_p = my_node[status_start].array[alphabet_id];
	for(i = 0; i < status_num; i++)
	{
		if(status_p[i] != -1)
			return i;
	}

	return -1;
}


//寻找匹配string字符串的终点status_end并返回
//失败返回-1
int Match_String(Node *my_node, int status_start, char *string)
{
	int i = 0;
	int status_end;

	if(string == NULL)
		return status_start;
	
	while((status_end = Match_Ch(my_node, status_start, string[i])) != -1)
	{
		printf("No.%d : %d -- %c -> %d\n", i, status_start, string[i], status_end);
		status_start = status_end;
		i++;
		if(string[i] == '\0') //遇到结束符号,则退出
			break;
	}

	if(status_end == -1) //匹配字符串失败,返回错误信息
	{
		fprintf(stderr, "status_%d can't go by %c\n", status_start, string[i]);
		fprintf(stderr, "Failed to match string %s\n", string);
		return -1;
	}

	return status_end;
}
