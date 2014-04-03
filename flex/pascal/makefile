OBJECT=$(T1)
T1=pascal

target:$(T1)

$(T1):$(T1).l
	flex $(T1).l
	gcc lex.yy.c -o $(T1)



clean:
	rm lex.yy.c $(T1)
