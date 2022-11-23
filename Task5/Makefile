all: lexem_list.o shell.o
	gcc -g -Wall -o shell.out lexem_list.o shell.o
	rm *.o

lexem_list.o: lexem_list.c
	gcc -g -Wall -c -o lexem_list.o lexem_list.c

syntax_tree.o: syntax_tree.c
	gcc -g -Wall -c -o syntax_tree.o syntax_tree.c

shell.o: shell.c
	gcc -g -Wall -c -o shell.o shell.c

run: ./shell.out
	./shell.out 


clean:
	rm *.o

