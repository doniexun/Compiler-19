OBJECT=$(T1)
T1=parser

target:$(OBJECT)

$(T1):lexical.c lexical.h parser.c parser.h main.c
	gcc lexical.c parser.c main.c -g -lm -o  $(T1)


clean:
	rm $(OBJECT)

test: $(T1)
	./$(T1) test.lan
tag:
	ctags -R .
