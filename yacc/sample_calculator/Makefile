OBJECT=cal

TARGET:$(OBJECT).l $(OBJECT).y
	flex $(OBJECT).l
	yacc -d $(OBJECT).y
	gcc lex.yy.c y.tab.c -o $(OBJECT)

clean:
	-rm lex.yy.c y.tab.* $(OBJECT)
