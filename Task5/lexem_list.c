#include <stdio.h>
#include <stdlib.h>
#include "lexem_list_types.h"

#define SIZE 16


static int c;      /*текущий символ */
static list_type lst; /* список слов (в виде массива)*/
static buf_type buf; /* буфер для накопления текущего слова*/
static int sizebuf;  /* размер буфера текущего слова*/
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

void addsym() {
    if (curbuf > sizebuf - 1)
        buf = realloc(buf, sizebuf += SIZE);
    buf[curbuf++] = c;
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

void printlist() {
    int i;

    if (lst == NULL)
        return;

    for (i = 0; i < sizelist - 1; i++)
        printf("%s\n", lst[i]);
}

int symset(int c) {
    return c != '\n' && c != ' ' && c != '\t' && c != '>' && c != EOF;
}

void read_lexem_set() {
    typedef enum { Start, Word, Greater, Greater2, Newline, Stop } vertex;
    vertex V = Start;
    c = getchar();

    null_list();

    while (1 == 1) {
        switch (V) {

        case Start:

            if (c == ' ' || c == '\t')
                c = getchar();

            else if (c == EOF) {
                termlist();
                printlist();
                clearlist();
                V = Stop;
            } 
            else if (c == '\n') {
                termlist();
                printlist();
                V = Newline;
                c = getchar();
            } 
            else {
                nullbuf();
                addsym();
                V = (c == '>') ? Greater : Word;
                c = getchar();
            }
            break;

        case Word:
            if (symset(c)) {
                addsym();
                c = getchar();
            } 
            else {
                V = Start;
                addword();
            }
            break;

        case Greater:
            if (c == '>') {
                addsym();
                c = getchar();
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

        case Newline:
            clearlist();
            V = Start;
            break;

        case Stop:
            exit(0);
            break;
        }
    }
}

list_type *get_lexem_list(){
	read_lexem_set();
	return &lst;
}