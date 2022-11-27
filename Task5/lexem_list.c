#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexem_list_types.h"

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
    
void print_string_list(list_type lst) {
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

int get_lexem_list_len(list_type lst){
    int len = 0;

    while (lst[len]!=NULL){
        len++;
    }

    return len;

}

int is_error(int bracket_balance_error,int quote_balance_error,int unacceptable_char_error){
    return (bracket_balance_error||quote_balance_error||unacceptable_char_error);
}

void print_errors(int bracket_balance_error,int quote_balance_error,int unacceptable_char_error){
    if (bracket_balance_error)
        printf("BRACKET BALANCE ERROR\n");
    
    if (quote_balance_error)
        printf("QUOTE BALANCE ERROR\n");
    
    if (unacceptable_char_error)
        printf("UNACCEPTABLE_CHAR_ERROR\n");
    return;
}

//0 is assigned to program status if \n was found (in other words command has ended) 
//1 is assigned if eof was found (need to exit the program)
//2 is assigned if error was found
//all errors are processed than eof or \n are reached
list_type get_lexem_list(status *program_status) {
	int cur_char;    
    int bracket_balance = 0;

    int bracket_balance_error = 0;
    int quote_balance_error = 0;
    int unacceptable_char_error = 0;

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

                    if (is_error(bracket_balance_error, quote_balance_error, unacceptable_char_error)){
                        print_errors(bracket_balance_error, quote_balance_error, unacceptable_char_error);
                        *program_status = Error;
                    }
                    else{
                        *program_status = Finish;
                    }
                    termlist();
                    return lst;

                case '\n':
                    if (is_error(bracket_balance_error, quote_balance_error, unacceptable_char_error)){
                        print_errors(bracket_balance_error, quote_balance_error, unacceptable_char_error);
                        *program_status = Error;
                    }
                    else{
                        *program_status = Success;
                    }
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
                    if (cur_char == '(')
                        bracket_balance++;

                    if (cur_char == ')')
                        bracket_balance--;

                    if (bracket_balance<0){
                        bracket_balance_error = 1;
                    }

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
                                *program_status = Finish;
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
                        unacceptable_char_error = 1;
                        cur_char = getchar();
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
                V = Start;
                quote_balance_error = 1;
                addword();// adding word in order to avoid mem leak
                
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
