OBJECT=$(T1)
T1=lex

target:$(T1)

$(T1):$(T1).l
	flex $(T1).l
	gcc lex.yy.c -o $(T1)



clean:
	rm lex.yy.c $(OBJECT)
