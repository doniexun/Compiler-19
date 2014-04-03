OBJECT=$(T1)
T1=count

target:$(T1)

$(T1):$(T1).l
	flex $(T1).l
	gcc lex.yy.c -o count



clean:
	rm lex.yy.c count
