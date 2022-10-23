#include "list.h"
#include <stdio.h>

//TODO - move newline code into start node 
// add unpairable special sym node 


//returns 1 if c - special symbol
int is_special(int c){
    return  (c=='|') || (c=='&') || (c==';') || (c == '>') || (c == '<') || (c = ',');
}


//returns 1 if c - is a normal symbol
int symset(int c) {
    return (c != '\n') && (c != ' ') && (c != '\t') && (c != EOF) && (!is_special(c));
}





int main() {
    char input_char;
    typedef enum { Start,Word,Greater,Greater2,Ampersand,Ampersand2,Unpairable_special,Stop } vertex;
    vertex V = Start;

    input_char = getchar();
    null_list();
    while (1) {
        switch (V) {
            //this vertex is reached at the beginning of program's runtime 
            //or than previous word input was interrupted for various purposes
            case Start:
                // space or tab termination 
                if (input_char == ' ' || input_char == '\t')
                    input_char = getchar();

                //if previous word was terminated by eof - print list and exit the program
                else if (input_char == EOF) 
                    V = Stop;

                //newline termination - print list, clear list, get ready to input next line 
                else if (input_char == '\n'){
                    term_list();
                    print_list();
                    clear_list();
                    input_char = getchar();
                }

                //this code is executed in case user starts typing new word (either common or special)
                else {
                    null_buf();
                    add_sym(input_char);
                    if (is_special(input_char)){
                        V = (input_char == '>') ? Greater : Unpairable_special;
                        V = (input_char == '&') ? Ampersand : Unpairable_special;
                    }
                    else{
                        V = Word;
                    }
                    //input_char = getchar();
                }
                break;

            case Word:
                if (symset(input_char)) {
                    add_sym(input_char);
                    input_char = getchar();
                } 
                else {
                    V = Start;
                    add_word();
                }
                break;

            //if greater symbol is met we are waiting for another greater symbol
            case Greater:
                if (input_char == '>') {
                    add_sym(input_char);
                    input_char = getchar();
                    V = Greater2;
                } 

                //if no another greater symbol has been inputted
                else {
                    add_word();
                    V = Start;
                }
                break;

            //saving >> word + going to Start vertex
            case Greater2:
                V = Start;
                add_word();
                break;

            case Ampersand:
                input_char = getchar();
                if (input_char=='&'){
                    add_sym(input_char);
                    V = Ampersand2;
                }

                else{
                    V = Start;
                    add_word();
                }
                break;

            case Ampersand2:
                V=Start;
                add_word();
                break;

            case Unpairable_special:

                break;

            //exiting program
            case Stop:
                term_list();
                print_list();
                clear_list();
                exit(0);
                break;
            }
    }
}