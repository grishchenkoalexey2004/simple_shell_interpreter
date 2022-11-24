#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexem_list_types.h"

//TODO: create enumerated data type for program_status 
//TODO: organize error printing 

#define SIZE 16

//graph type
typedef enum { Start, Word, Greater, Greater2, Ampersand, 
    Ampersand2, Vert_slash, Vert_slash2, Quoted_sequence} vertex;


static list_type lst; /* word list */
static buf_type buf; /* buffer that collects current lexem*/
static int sizebuf;  /* size of current lexem*/
static int sizelist; /* word list size*/
static int curbuf;   /* index of cur symbol in buf*/
static int curlist;  /* index of cur lexem in array*/

static char spec_sym_array[] = "<>&#|(); \n\t";

//symbols that can be used to create normal words (no special symbols included)
static char word_char_array[] = "abcdefghijklmnopqrstuvwxyz0123456789$.,/_\\";

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
    char *normal_sym_link = index(word_char_array,cur_char);

    //if symbol!= eof and can be found in array of word symbols - return true

    if (cur_char!=EOF && normal_sym_link!=NULL)
        return 1;

    else
        return 0;
}

//0 is assigned to program status if \n was found (in other words command has ended) 
//1 is assigned if eof was found (need to exit the program)
//2 is assigned if error was found

list_type read_lexem_set(int *program_status) {
	int cur_char;    

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

                case EOF: 
                    *program_status = 1;
                    termlist();
                    return lst;

                case '\n':
                    *program_status = 0;
                    termlist();
                    return lst;

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

                case '"':
                    nullbuf();
                    V = Quoted_sequence;
                    cur_char =getchar();
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

                //processing of word symbols 
                //in this section error is raised in case symbol doesn't belong to word symbol set
                default:
                    if(is_word_symbol(cur_char)){                        
                        nullbuf();

                        

                        if (cur_char == '\\'){
                            cur_char = getchar();
                            if (cur_char == EOF){
                                *program_status = 1;
                                termlist();
                                return lst; 
                                                           }
                            else{
                                addsym(cur_char);
                                V = Word;
                            }
                            
                        }

                        else{
                            addsym(cur_char);
                            V = Word;
                        }
                        
                        cur_char = getchar();
                    }

                    else{
                        printf("ERROR! : UNACCEPTABLE SYMBOL!");
                        *program_status = 2;
                        termlist();
                        return lst;
                    }
            }
            break; 

        case Quoted_sequence:
            while (cur_char!=EOF && cur_char!='"' && cur_char!='\n'){

                if (cur_char == '\\'){
                    cur_char = getchar();

                    if (cur_char!=EOF){
                        addsym(cur_char);
                        cur_char = getchar();
                    }

                    else{
                        addword();
                        V = Start;
                    }
                }

                else{
                    addsym(cur_char);
                    cur_char = getchar();
                }
                
            }

            //closing quote found
            if (cur_char=='"'){
                addword();
                V=Start;
                cur_char = getchar();
            }
            else if ((cur_char == EOF) || (cur_char == '\n')){
                printf("ERROR! Double quote hasn't been closed");
                *program_status = 2;
                addword(); // adding word to wordlist in order to avoid memory leak
                termlist();
                return lst;
            }
            break;

        case Word:
            if (is_word_symbol(cur_char)) {

                if (cur_char == '\\'){
                    cur_char = getchar();

                    if (cur_char!=EOF){
                        addsym(cur_char);
                        cur_char = getchar();
                    }

                    else{
                        addword();
                        V = Start;
                    }
                }

                else{
                    addsym(cur_char);
                    cur_char = getchar();
                }
                
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
        }
    }
}

list_type get_lexem_list(int *program_status){

	list_type lexem_list = read_lexem_set(program_status);
	printlist(lexem_list);

	return lexem_list;
}