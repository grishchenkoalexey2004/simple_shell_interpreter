//TODO : exclude multiple <stdio.h> inclusion
#include <stdio.h>
#include "list.h"

//TODO : add | symbol 
//TODO : add malfunction defense
//TODO : comment out all the code

// size of symbol block we will read until eof
#define BLOCK_SIZE 11

static char alph[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789$./_\0";

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

typedef char char_block_type[BLOCK_SIZE];

// returns 1 if c - special symbol
int is_special(int c) {
    return (c == '|') || (c == '&') || (c == ';') || (c == '>') || (c == '<') ||
           (c == ',');
}

//checking if symbol of special word is pairable
int is_pairable(int c){ 
    return (c == '&') || (c == '>') || (c=='|'); 
}


int is_in_alphabet(int c){
    int return_val = 0;
    int index = 0;
    while (alph[index]!='\0'){
        if (alph[index]==c){
            return_val = 1;
            break;
        }
        index++;
    }
    return return_val;
}

// returns 1 if c - is a normal symbol
int is_word_symbol(int c) {
    return (c != '\n') && (c != ' ') && (c != '\t') && (c != EOF) &&
           (!is_special(c)) && (is_in_alphabet(c));
}

void read_char_block(char_block_type char_block) {
    fscanf(stdin, "%10c", char_block);
    return;
}

void clear_char_block(char_block_type char_block) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        char_block[i] = 0;
    }
    return;
}

//checks if char block is read to the end
//! if char block isn't read to the end, we can be sure, that eof has been encountered
int char_block_read(int ind){
    return (ind==BLOCK_SIZE);
}

// retrives next char from charblock 
// increases ind by 1 
char get_next_char(char_block_type char_block,int *ind){
    char next_char = char_block[*ind];
    *ind = *ind +1;
    return next_char;
}

// processes char block (writes char && words to list)
// params : char block  ; vertex - last vertex of the graph
// returns : new graph vertex 
vertex process_char_block(vertex V) {
    static int line_number = 1;
    static int error_on_the_line = 0;
    char input_char;
    char_block_type char_block;
    clear_char_block(char_block);
    read_char_block(char_block);

    input_char = char_block[0];
    int char_ind = 1;//index of next char

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
                    if (error_on_the_line){
                        clear_list();
                        printf("Unacceptable symbol on line number %d",line_number);
                    }
                    print_sort_clear_list();
                    line_number++;
                    input_char = get_next_char(char_block,&char_ind);
                    error_on_the_line = 0;
                }
                else {
                    null_buf();
                    add_sym(input_char);
                    if (is_pairable(input_char)) { // checking if sym == > | &
                        V = (input_char == '>') ? Greater : Ampersand;
                    } else if (is_special(input_char)) { //checking if sym is special and != > | &
                        V = Unpairable_special;
                    } else if (is_word_symbol(input_char)){
                        V = Word;
                    }

                    //if unacceptable symbol has been entered, buffer is cleared 
                    //error_on_the_line flag is set to 1, and corresponding operations 
                    //are performed when eof or \n chars are met
                    else{
                        clear_buf();
                        error_on_the_line = 1;
                        V = Start;
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

                else if (is_word_symbol(input_char)) {
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
                if (error_on_the_line){
                    clear_list();
                        printf("\n Unacceptable symbol on line number %d \n",line_number);

                }
                print_sort_clear_list();
                return Stop;
        }
    }
    
}  

int main() {
    vertex V = Start;
    null_list();
    while (V!=Stop) {
        V = process_char_block(V);
    }
    return 0;
}