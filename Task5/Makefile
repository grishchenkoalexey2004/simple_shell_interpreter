all: lexem_tree.o syntax_tree.o
	gcc -g -Wall -o shell.out lexem_list.o syntax_tree.o
	rm *.o

lexem_list.o: lexem_tree.c
	gcc -g -Wall -c -o lexem_list.o lexem_list.c

syntax_tree.o: syntax_tree.c
	gcc -g -Wall -c -o syntax_tree.o syntax_tree.c

clean:
	rm *.o