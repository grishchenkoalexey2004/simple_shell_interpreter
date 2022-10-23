#include "list.h"
#include <stdio.h>

// returns 0 if symbol is special, otherwise returns 1
// TODO: requires corrections

int symset(int c) {
    return (c != '\n') && (c != ' ') && (c != '\t') && (c != '>') && (c != EOF);
}

int main() {
    char input_char;
    typedef enum { Start, Word, Greater, Greater2, Newline, Stop } vertex;
    vertex V = Start;
    input_char = getchar();
    null_list();
    while (1 == 1) {
        switch (V) {

        case Start:
            if (input_char == ' ' || input_char == '\t')
                input_char = getchar();
            else if (input_char == EOF) {
                term_list();
                print_list();
                clear_list();
                V = Stop;
            } else if (input_char == '\n') {
                term_list();
                print_list();
                V = Newline;
                input_char = getchar();
            } else {
                null_buf();
                add_sym(input_char);
                V = (input_char == '>') ? Greater : Word;
                input_char = getchar();
            }
            break;

        case Word:
            if (symset(input_char)) {
                add_sym(input_char);
                input_char = getchar();
            } else {
                V = Start;
                add_word();
            }
            break;

        case Greater:
            if (input_char == '>') {
                add_sym(input_char);
                input_char = getchar();
                V = Greater2;
            } else {
                V = Start;
                add_word();
            }
            break;

        case Greater2:
            V = Start;
            add_word();
            break;

        case Newline:
            clear_list();
            V = Start;
            break;

        case Stop:
            exit(0);
            break;
        }
    }
}