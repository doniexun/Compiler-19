OBJECT=input_DFA input_NFA
CC=gcc

TARGET:$(OBJECT)

$(OBJECT):%:%.c
	$(CC) $< -o $@

clean:
	-rm $(OBJECT)
tag:
	ctags -R .
