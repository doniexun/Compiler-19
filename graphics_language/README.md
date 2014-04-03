编译原理上机
===========

构造一个简单函数绘图语言的编译器
-------------
(词法分析器, 语法分析器)
-------------


1. 语言简述
<1> 5种语言
	*	循环绘图(FORDRAW)
	*	比例设置(SCALE)
	*	角度旋转(ROT)
	*	坐标平移(ORIGIN)
	*	注释	(--)或(//)
<2>	坐标系
	*	定义左上角为原点
	*	x从左向右增长
	*	y从上到下增长
<3> 函数举例
	origin is (100,300);	--设置原点的偏移量
	rot is 0;				--设置旋转角度(不旋转)
	scale is (1,1);			//设置比例
	for T from 0 to 200 step 1 draw (t, 0);		//设置横坐标轨迹
	for T from 0 to 150 step 1 draw (0,-t);		//设置纵坐标轨迹
	for T from 0 to 120 step 1 draw (t,-t);		//函数f(t)=t轨迹

	默认值:
	origin is (0,0);
	rot is 0;
	scale is (1,1);
2. 语句的语法和语义
