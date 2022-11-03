#include <stdio.h>
#include "list.h"

// size of symbol block 
#define BLOCK_SIZE 11

//word symbols
static char alph[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789$./_\0";

//graph nodes 
typedef enum {
    Start,
    Word,
    Greater,
    Greater2,
    Ampersand,
    Ampersand2,
    Vert_slash,
    Vert_slash2,
    Unpairable_special,
    Stop
} vertex;

//type of char block declared
typedef char char_block_type[BLOCK_SIZE];


// returns 1 if c - special symbol
int is_special(int c) {
    return (c == '|') || (c == '&') || (c == ';') || (c == '>') || (c == '<') 
    || (c == '(') || (c==')');
}

//checking if symbol of special word is pairable
int is_pairable(int c){ 
    return (c == '&') || (c == '>') || (c=='|'); 
}

//tries to find char c in alph array (returns 1 in case of successful search)
int is_word_symbol(int c){
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

//fills char block with 10 new symbols from stdin
void read_char_block(char_block_type char_block) {
    fscanf(stdin, "%10c", char_block);
    return;
}

//fills char block with '\0'
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

//reads and processes char block
//return value - vertex we ended on after processing the last symbol from the char block
vertex process_char_block(vertex V) {

    static int line_number = 1; //number of current line
    static int error_on_the_line = 0;// 0- no error 
    static char unacceptable_sym;//in this variable symbol that caused input error is saved

    char input_char;
    int char_ind = 1;//index of next char
    char_block_type char_block;

    clear_char_block(char_block);
    read_char_block(char_block);

    input_char = char_block[0];

    while (1){
        switch (V) {
            case Start:
                // space or tab termination
                if (input_char == ' ' || input_char == '\t')
                    input_char = get_next_char(char_block,&char_ind);

                //char block end termination
                else if (input_char == '\0'){
                    if (char_block_read(char_ind))
                        return Start;
                    else
                        V = Stop;
                }

                // newline termination - print list, clear list, get ready to input next
                else if (input_char == '\n') {
                    if (error_on_the_line){
                        //null has to be written at the end of the list, otherwise clearlist
                        //won't be able to see the borders of the list
                        term_list();
                        clear_list();
                        //printing error
                        printf("\nUnacceptable symbol on line number %d - '%c'\n",line_number,unacceptable_sym);
                    }
                    //do all necessary operations and prepare to process next string
                    print_sort_clear_list();
                    line_number++;
                    input_char = get_next_char(char_block,&char_ind);
                    error_on_the_line = 0;
                }
                //if new word is about to be input
                else {
                    null_buf();
                    add_sym(input_char);
                    if (is_pairable(input_char)) { // checking if sym == > | &
                        switch(input_char){
                            case '>':
                                V = Greater;
                                break;
                            case '&':
                                V = Ampersand;
                                break;
                            case '|':
                                V = Vert_slash;
                                break;
                        }
                    } else if (is_special(input_char)) { //checking if sym is special and unpairable
                        V = Unpairable_special;
                    } else if (is_word_symbol(input_char)){
                        V = Word;
                    }

                    //if unacceptable symbol has been entered, buffer is cleared 
                    //error_on_the_line flag is set to 1, and corresponding operations 
                    //are performed when eof or \n chars are met
                    else{
                        unacceptable_sym = input_char;
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

            case Vert_slash:
                if (input_char =='\0'){ // checking if the is finished
                    if (char_block_read(char_ind))
                        return Vert_slash;
                    else{
                        add_word();//saving '&' before program exit
                        V=Stop;
                    }
                }

                else if (input_char == '|') {
                    add_sym(input_char);
                    // receiving next word symbol
                    input_char = get_next_char(char_block,&char_ind);
                    V = Vert_slash2;
                }

                else {
                    V = Start;
                    add_word();
                }
                break;

            case Vert_slash2:
                V = Start;
                add_word();
                break;

            //creates word from one symbol
            case Unpairable_special:
                add_word();
                V = Start;
                break;

            // exiting program
            case Stop:
                //if error was found on the last line
                if (error_on_the_line){
                    clear_list();
                    printf("\n Unacceptable symbol on line number %d - '%c' \n",line_number,unacceptable_sym);

                }
                /*print_sort_clear_list does nothing in case the error has been caught and the list has
                therefore been cleared*/
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