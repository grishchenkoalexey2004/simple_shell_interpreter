//TODO : exclude multiple <stdio.h> inclusion
#include <stdio.h>
#include <string.h>
#include "list.h"

// size of symbol block we will read until eof
#define BLOCK_SIZE 11
typedef enum {
    Start,
    Word,
    Greater,
    Greater2,
    Ampersand,
    Ampersand2,
    Unpairable_special,
    Stop
} vertex;

// returns 1 if c - special symbol
int is_special(int c) {
    return (c == '|') || (c == '&') || (c == ';') || (c == '>') || (c == '<') ||
           (c == ',');
}

int is_pairable(int c){ 
    return (c == '&') || (c == '>'); 
}

// returns 1 if c - is a normal symbol
int symset(int c) {
    return (c != '\n') && (c != ' ') && (c != '\t') && (c != EOF) &&
           (!is_special(c));
}

void read_char_block(char char_block[]) {
    fscanf(stdin, "%10c", char_block);
    return;
}

void clear_char_block(char arr[]) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        arr[i] = 0;
    }
    return;
}

//checks if char block is read
int char_block_read(int ind){
    return (ind==BLOCK_SIZE);
}

// retrives next char from the array 
// increases ind by 1 

char get_next_char(char char_block[],int *ind){
    char next_char = char_block[*ind];
    *ind = *ind +1;
    return next_char;
}


// processes char block (writes char && words to list)
// params : char block  ; vertex - last vertex of the graph
// returns : new graph vertex 

//error on input petya >> alex
vertex process_char_block(char char_block[], vertex V) {
    char input_char = char_block[0];
    int char_ind = 1;
    while (1){
        switch (V) {
    
            case Start:
                // space or tab termination
                if (input_char == ' ' || input_char == '\t')
                    input_char = get_next_char(char_block,&char_ind);

                else if (input_char == '\0'){
                    if (char_block_read(char_ind))
                        return Start;
                    else
                        V = Stop;
                }

                // newline termination - print list, clear list, get ready to input next
                else if (input_char == '\n') {
                    term_list();
                    print_list();
                    clear_list();
                    input_char = get_next_char(char_block,&char_ind);
                }

                // this code is executed in case user starts typing new word (either
                // common or special)
                else {
                    null_buf();
                    add_sym(input_char);
                    if (is_pairable(input_char)) {
                        V = (input_char == '>') ? Greater : Ampersand;
                    } else if (is_special(input_char)) {
                        V = Unpairable_special;
                    } else {
                        V = Word;
                    }
                    input_char = get_next_char(char_block,&char_ind);
                }
                break;

            case Word:
                /*before reading the second symbol of the word we need to make sure that char block
                                                                                hasn't ended*/

                if (input_char=='\0'){
                    if (char_block_read(char_ind))
                        return Word;
                    else{
                        add_word();
                        V=Stop;
                    } 
                }  

                else if (symset(input_char)) {
                    add_sym(input_char);
                    input_char = get_next_char(char_block,&char_ind);

                    if (input_char=='\0'){
                        if (char_block_read(char_ind))
                            return Word;
                        else{
                            add_word();
                            V=Stop;
                        } 
                    }  
                } 

                else {
                    V = Start;
                    add_word();
                }
                break;

            // if greater symbol is met we are waiting for another greater symbol
            case Greater:
                if (input_char =='\0'){ // checking if the is finished
                    if (char_block_read(char_ind))
                        return Greater;
                    else{
                        add_word();//saving '>' before program exit
                        V=Stop;
                    }
                }

                else if (input_char == '>') {
                    add_sym(input_char);
                    // receiving symbol of next lexem
                    input_char = get_next_char(char_block,&char_ind);
                    V = Greater2;
                }
                // if no another greater symbol has been inputted
                else {
                    add_word();
                    V = Start;
                }
                break;

            // saving >> word + going to Start vertex
            case Greater2:
                V = Start;
                add_word();
                break;

            case Ampersand:
                if (input_char =='\0'){ // checking if the is finished
                    if (char_block_read(char_ind))
                        return Ampersand;
                    else{
                        add_word();//saving '&' before program exit
                        V=Stop;
                    }
                }

                else if (input_char == '&') {
                    add_sym(input_char);
                    // receiving next word symbol
                    input_char = get_next_char(char_block,&char_ind);
                    V = Ampersand2;
                }

                else {
                    V = Start;
                    add_word();
                }
                break;

            case Ampersand2:
                V = Start;
                add_word();
                break;

            case Unpairable_special:
                add_word();
                V = Start;
                break;

            // exiting program
            //TODO: put add_word here
            case Stop:
                term_list();
                print_list();
                sort_list();
                print_list();
                clear_list();
                return Stop;
        }
    }
    
}

int main() {
    char char_block[BLOCK_SIZE];

    vertex V = Start;
    clear_char_block(char_block);
    read_char_block(char_block);
    null_list();

    while (V!=Stop) {
        V = process_char_block(char_block, V);
        clear_char_block(char_block);
        read_char_block(char_block);
    }
    return 0;
}