// Grammar description

/*
<shell_command>  = <command_list>
<command_list ::= <pipe> { [one of &,&&,;||] <pipe>} [& ;]
<pipe> = <command> {|<command>}
<command> = <simple command> | (<command list>) [< file_name] [[one of >, >>] file_name]
<simple command> ::=<file_name> {<argument>} [< filename] [[one of > >>] file_name]
*/


//TODO: develop decent error catch system (with jumps and etc)


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexem_list_types.h"
#include "syntax_tree_types.h"


static char and_separator[] = "&&";
static char or_separator[] = "||";
static char semicolon_separator[] = ";";
static char background_process_separator[] = "&";

int is_separator(char *lexem);
int find_closing_bracket(list_type lst,int start_index,int end_index);
void print_list_slice(list_type lst,int start_index,int end_index);
int find_separator(list_type lst,int start_index,int end_index);
cmd_link build_syntax_tree(list_type lst,int start_index,int end_index);
void build_pipe_chain(list_type lst,cmd_link first_pipe_cmd,int start_index,int end_index);



//returns 1 if lexem is separator, 0 if not
int is_separator(char *lexem){
	return ((strcmp(and_separator,lexem)==0) || (strcmp(or_separator,lexem)==0)
		||(strcmp(semicolon_separator,lexem)==0) || (strcmp(background_process_separator,lexem)==0));

}

void null_struct_fiels(cmd_link cmd){

	cmd->command_name = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->psubcmd = NULL;
	cmd->pipe = NULL;
	cmd->next = NULL;
	cmd->separator = NULL;

	return;
}

//debug function 
void print_list_slice(list_type lst,int start_index,int end_index){
    
    for (int i = start_index; i <=end_index; i++)
        printf("%s  ", lst[i]);

    printf("\n");

    return;
}

//searches for ")" in [start_index,end_index] range
//the scenario in which closing bracket isn't found is impossible

int find_closing_bracket(list_type lst,int start_index,int end_index){
	int index = start_index;
	while (1){
		if (strcmp(lst[index],")")==0)
			return index;
		
		index++;

	}
}

//returns index of the first found "|" or (-1) if "|" wasn't found
//ignores pipe symbols in ()
int find_pipe_sym(list_type lst,int start_index,int end_index){
	int index = start_index;
	for(;index<=end_index;){

		if (strcmp("(",lst[index])==0){
			index = find_closing_bracket(lst,start_index,end_index)+1;
			continue;
		}

		if (strcmp(lst[index],"|")==0)
			return index;

		index++;
	}
	return -1;
}

//returns index of the separator
/* 
ignores separators that are inside curly brackets 
these separators will be parsed during subshell parsing stage
*/

// (-1) if separator wasn't found in the remaining part of the lexem list

int find_separator(list_type lst,int start_index,int end_index){
	int index = start_index;
	for(;index<=end_index;){

		if (strcmp("(",lst[index])==0){
			index = find_closing_bracket(lst,start_index,end_index)+1;
			continue;
		}

		if (is_separator(lst[index]))
			return index;

		index++;
	}
	return -1;
}


//all lexems inside list are indexed
//each function forming syntaxtree works in predefined slice of lexem list
//lst is not a null list

cmd_link build_syntax_tree(list_type lst,int start_index,int end_index){
	char *separator = NULL;
	cmd_link tree_root =NULL;
	cmd_link prev_cmd = NULL;
	cmd_link new_cmd = NULL;
	int sep_index;	
	int syntax_error = 0;


	while (1){
		sep_index =find_separator(lst,start_index,end_index);

		//if shell command starts from separator or there is no process between separators
		if (sep_index == start_index){
			printf("SYNTAX ERROR");
			syntax_error = 1;
			break;

		}

		if (sep_index!=(-1)){//if separator was found

			separator = lst[sep_index];
			new_cmd = malloc(sizeof(struct cmd_inf));
			new_cmd->separator = separator;


			if (tree_root==NULL)
				tree_root = new_cmd;
			
			if (prev_cmd!=NULL)
				prev_cmd->next = new_cmd;
			
			prev_cmd = new_cmd;

			build_pipe_chain(lst,new_cmd,start_index,sep_index-1);

			start_index  = sep_index+1;
		}

		//TODO: rework this chunk of code
		else{
			//if lexem_list_hasn't been processed to the end
			if (start_index<=end_index){
				new_cmd = malloc(sizeof(struct cmd_inf));
				new_cmd->separator = NULL;
				new_cmd->next = NULL;

				if (prev_cmd!=NULL)
					prev_cmd->next = new_cmd;

				if (tree_root == NULL) //if command doesn't contain any separators
					tree_root = new_cmd;


				build_pipe_chain(lst,new_cmd,start_index,end_index);

				break;
			}

			else{
				//prev_cmd has already been processed, there is no need to launch recursive descent
				prev_cmd->next = NULL;
				break; // this branch will be executed if command is ended by separator (this means there is no text after it)
			}
		}
		
	} 

	if (!syntax_error)
		return tree_root;
	else 
		return NULL;
}

//returns link to the first command in the pipe

void build_pipe_chain(list_type lst,cmd_link first_pipe_cmd,int start_index,int end_index){
	int pipe_sym_index;
	int syntax_error = 0;
	cmd_link new_pipe_cmd = NULL;
	cmd_link prev_pipe_cmd = NULL;

	pipe_sym_index = find_pipe_sym(lst,start_index,end_index);

	if (pipe_sym_index==(-1)){// if there is no pipe sequence
		print_list_slice(lst, start_index, end_index);
		return;
	}

	if (pipe_sym_index == start_index){
		printf("SYNTAX ERROR");
		return;
	}

	//print first command
	print_list_slice(lst, start_index, pipe_sym_index-1);
	prev_pipe_cmd = first_pipe_cmd;
	//pipe list will start from the first command in the pipe

	start_index = pipe_sym_index+1;
	if (start_index>end_index){
		printf("SYNTAX ERROR");
		return;
	}

	//this cycle will be activated with list slice has format cmd1|cmd2{|cmd N}  (>=2 commands in pipe)
	while ((pipe_sym_index =find_pipe_sym(lst,start_index,end_index))!=(-1)){

		if (pipe_sym_index == start_index){
			printf("SYNTAX ERROR");
			syntax_error = 1;
			break;
		}

		printf("pipe\n");

		print_list_slice(lst,start_index,pipe_sym_index-1);
		new_pipe_cmd = malloc(sizeof(struct cmd_inf));
		prev_pipe_cmd->pipe = new_pipe_cmd;
		prev_pipe_cmd = new_pipe_cmd;

		//changing indexes
		start_index = pipe_sym_index+1;
		if (start_index>end_index){
			printf("SYNTAX ERROR");
			return;
		}

	}

	printf("pipe\n");
	//reading last pipe command
	new_pipe_cmd = malloc(sizeof(struct cmd_inf));
	prev_pipe_cmd->pipe= new_pipe_cmd;
	new_pipe_cmd->pipe = NULL;
	print_list_slice(lst,start_index,end_index);

	return;
}