#include <stdio.h>
#include <stdlib.h>
#include "lexem_list_types.h"

#define SIZE 16


static list_type lst; /* word list */
static buf_type buf; /* buffer that collects current lexem*/
static int sizebuf;  /* size of current lexem*/
static int sizelist; /* word list size*/
static int curbuf;   /* index of cur symbol in buf*/
static int curlist;  /* index of cur lexem in array*/


void clearlist() {
    int i;
    sizelist = 0;
    curlist = 0;
    if (lst == NULL)
        return;
    for (i = 0; lst[i] != NULL; i++)
        free(lst[i]);
    free(lst);
    lst = NULL;
}

void null_list() {
    sizelist = 0;
    curlist = 0;
    lst = NULL;
}

void termlist() {
    if (lst == NULL)
        return;
    if (curlist > sizelist - 1)
        lst = realloc(lst, (sizelist + 1) * sizeof(*lst));
    lst[curlist] = NULL;
    lst = realloc(lst, (sizelist = curlist + 1) * sizeof(*lst));
}

void nullbuf() {
    buf = NULL;
    sizebuf = 0;
    curbuf = 0;
}

void addsym(int cur_char) {
    if (curbuf > sizebuf - 1)
        buf = realloc(buf, sizebuf += SIZE);
    buf[curbuf++] = cur_char;
}

void addword() {
    if (curbuf > sizebuf - 1)
        buf = realloc(buf, sizebuf += 1);

    buf[curbuf++] = '\0';
    buf = realloc(buf, sizebuf = curbuf);

    if (curlist > sizelist - 1)
        lst = realloc(lst, (sizelist += SIZE) * sizeof(*lst)); 

    lst[curlist++] = buf;
}

void printlist(list_type lst) {
    int i;

    if (lst == NULL)
        return;

    for (i = 0; i < sizelist - 1; i++)
        printf("%s\n", lst[i]);
}

int symset(int cur_char) {
    return (cur_char != '\n') && (cur_char != ' ') && (cur_char != '\t')
     && (cur_char != '>') && (cur_char != EOF);
}


//returns 0 if eof was found
//otherwise returns 1 

list_type read_lexem_set(int *program_status) {
	char cur_char;
    typedef enum { Start, Word, Greater, Greater2, Stop } vertex;

    vertex V = Start;
    cur_char = getchar();
    null_list();

    while (1) {
        switch (V) {

        case Start:

            if (cur_char == ' ' || cur_char == '\t')
                cur_char = getchar();

            else if (cur_char == EOF || cur_char== '\n') {
            	(cur_char==EOF)?(*program_status = 0):(*program_status = 1);
                termlist();
                V = Stop;
            }
            else {
                nullbuf();
                addsym(cur_char);
                V = (cur_char == '>') ? Greater : Word;
                cur_char = getchar();
            }
            break;

        case Word:
            if (symset(cur_char)) {
                addsym(cur_char);
                cur_char = getchar();
            } 
            else {
                V = Start;
                addword();
            }
            break;

        case Greater:
            if (cur_char == '>') {
                addsym(cur_char);
                cur_char = getchar();
                V = Greater2;
            } 
            else {
                V = Start;
                addword();
            }
            break;

        case Greater2:
            V = Start;
            addword();
            break;

        case Stop:
        	return lst;
        }
    }
}

list_type get_lexem_list(int *program_status){
	list_type lexem_list = read_lexem_set(program_status);

	printlist(lexem_list);

	return lexem_list;
}