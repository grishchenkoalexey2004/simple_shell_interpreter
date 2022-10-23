#include "list.h"
#include <stdio.h>

#define SIZE 16
void sort();

char **lst; /* список слов (в виде массива)*/
char *buf; /* буфер для накопления текущего слова*/

int sizebuf;  /* размер буфера текущего слова*/
int sizelist; /* размер списка слов*/

int curbuf;   /* индекс текущего символа в буфере*/
int curlist;  /* индекс текущего слова в списке*/

void clear_list() {
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

void term_list() {
    if (lst == NULL)
        return;
    if (curlist > sizelist - 1)
        lst = realloc(lst, (sizelist + 1) * sizeof(*lst));
    lst[curlist] = NULL;
    /*выравниваем используемую под список память точно по размеру списка*/
    lst = realloc(lst, (sizelist = curlist + 1) * sizeof(*lst));

    return;
}

void null_buf() {
    buf = NULL;
    sizebuf = 0;
    curbuf = 0;
}

void add_sym(char c) {
    if (curbuf > sizebuf - 1)
        buf = realloc(buf, sizebuf += SIZE); /* увеличиваем буфер при
            необходимости */
    buf[curbuf++] = c;
}
void add_word() {
    if (curbuf > sizebuf - 1)
        buf = realloc(buf, sizebuf += 1); /* для записи ’\0’ увеличиваем буфер
            при
            необходимости
            */
    buf[curbuf++] = '\0';

    buf = realloc(buf, sizebuf = curbuf);
    if (curlist > (sizelist - 1))
        lst = realloc(lst, (sizelist += SIZE) * sizeof(*lst)); 
    lst[curlist++] = buf;
}


void print_list() {
    int i;
    if (lst == NULL)
        return;
    for (i = 0; i < sizelist - 1; i++)
        printf("%s\n", lst[i]);
}