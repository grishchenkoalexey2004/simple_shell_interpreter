#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexem_list_types.h"

//TODO: add backslash processing

#define SIZE 16

static list_type lst; /* word list */
static buf_type buf; /* buffer that collects current lexem*/
static int sizebuf;  /* size of current lexem*/
static int sizelist; /* word list size*/
static int curbuf;   /* index of cur symbol in buf*/
static int curlist;  /* index of cur lexem in array*/

static char spec_sym_array[] = "<>&#|(); \n\t";

//symbols that can be used to create normal words (no special symbols included)
static char word_sym_array[] = "abcdefghijklmnopqrstuvwxyz0123456789$.,/_\"";

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

int is_word_symbol(int cur_char) {
    char *normal_sym_link = index(word_sym_array,cur_char);

    //if symbol!= eof and can be found in array of word symbols - return true

    if (cur_char!=EOF && normal_sym_link!=NULL)
        return 1;

    else
        return 0;
}


//returns 0 if eof was found
//otherwise returns 1 

list_type read_lexem_set(int *program_status) {
	char cur_char;
    // quote_opened = 1 if user inputs symbol
    int quote_opened = 0;

    

    typedef enum { Start, Word, Greater, Greater2, Ampersand, 
    Ampersand2, Vert_slash, Vert_slash2, Stop} vertex;

    vertex V = Start;
    cur_char = getchar();
    null_list();

    while (1) {
        switch (V) {

        case Start:
            switch(cur_char){
                case ' ':case '\t':
                    cur_char = getchar();
                    break;

                case EOF: case '\n':
                    (cur_char==EOF)?(*program_status = 0):(*program_status = 1);
                    termlist();
                    V = Stop;
                    break;

               
                case '&':
                    nullbuf(); 
                    addsym(cur_char);
                    V=Ampersand;
                    cur_char=getchar();
                    break;

                case '|':
                    nullbuf(); 
                    addsym(cur_char);
                    V = Vert_slash;
                    cur_char = getchar();
                    break;
                
                    
                case '>':
                    nullbuf(); 
                    addsym(cur_char);
                    V = Greater;
                    cur_char = getchar();
                    break;

                /*Single_special_characters - special character that can't be doubled, for example '<'*/
                case '<': case '(': case ')': case ';' : case '#':
                    nullbuf();
                    addsym(cur_char);
                    addword();
                    V=Start;
                    cur_char =getchar();
                    break;


                default:
                    if(is_word_symbol(cur_char)){
                        if (cur_char == '"')
                            quote_opened = (quote_opened)?0:1;
                        nullbuf();
                        addsym(cur_char);
                        V = Word;
                        cur_char = getchar();
                        break;
                    }
                    else{
                        printf("ERROR : UNACCEPTABLE SYMBOL");
                    }
                    
            }

        case Word:
            if (is_word_symbol(cur_char)) {
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

        case Ampersand:
            if (cur_char == '&') {
                addsym(cur_char);
                cur_char = getchar();
                V = Greater2;
            } 
            else {
                V = Start;
                addword();
            }
            break;
            
        case Ampersand2:
            V = Start;
            addword();
            break;

        case Vert_slash:
            if (cur_char == '|') {
                addsym(cur_char);
                cur_char = getchar();
                V = Greater2;
            } 
            else {
                V = Start;
                addword();
            }
            break;
            
        case Vert_slash2:
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