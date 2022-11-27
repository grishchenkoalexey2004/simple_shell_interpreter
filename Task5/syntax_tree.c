// Grammar description

/*
<shell_command>  = <command_list>
<command_list ::= <pipe> { [one of &,&&,;||] <pipe>} [& ;]
<pipe> = <command> {|<command>}
<command> = <simple command> | (<command list>) [< file_name] [[one of >, >>] file_name]
<simple command> ::=<file_name> {<argument>} [< filename] [[one of > >>] file_name]
*/


//TODO: make syntax tree builer that reads && and || and creates appropriate number of structure 

#include <stdio.h>
#include <string.h>
#include "lexem_list_types.h"
#include "syntax_tree_types.h"


static char and_separator[] = "&&";
static char or_separator[] = "||";
static char semicolon_separator[] = ";";
static char background_process_separator[] = "&";


//returns index of separator in the list or -1 if no separator was found
//changes value of start_index variable so that lst[start_index] points to lexem after separator

//returns 1 if lexem is separator, 0 if not
int is_separator(char *lexem){
	return ((strcmp(and_separator,lexem)==0) || (strcmp(or_separator,lexem)==0)
		||(strcmp(semicolon_separator,lexem)==0) || (strcmp(background_process_separator,lexem)==0));

}

//debug function 
void print_list_slice(list_type lst,int start_index,int end_index){
    
    for (int i = start_index; i <=end_index; i++)
        printf("%s  ", lst[i]);

    printf("\n");

    return;
}

//returns index of the separator

int find_separator(list_type lst,int start_index,int end_index){
	int index = start_index;

	for(;index<=end_index;index++){
		if (is_separator(lst[index]))
			return index;	
	}

	return -1;//return last element of the list
	//in this case
}


//all lexems inside list are indexed
//each function forming syntaxtree works in predefined slice of lexem list
//lst is not a null list
cmd_link build_syntax_tree(list_type lst,int start_index,int end_index){
	char *separator = NULL;
	int sep_index;
	cmd_link tree_root = NULL;

	while (1){
		sep_index =find_separator(lst,start_index,end_index);
		if (sep_index!=(-1)){
			print_list_slice(lst,start_index, sep_index-1);
			separator = lst[sep_index];
			start_index  = sep_index+1;
		}
		else{
			if (start_index<=end_index){
				print_list_slice(lst, start_index, end_index); // printing the last part
				break;
			}
			else 
				break; // this branch will be executed if command ends by &
		}
		
	} 

	return NULL;
}