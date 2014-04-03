OBJECT=$(T1)
T1=lexical

target:$(OBJECT)

$(T1):$(T1).c main.c
	gcc $(T1).c main.c -lm -o $(T1)

clean:
	rm $(OBJECT)

test: $(T1)
	./$(T1) test.lan
	make clean
tag:
	ctags -R .
