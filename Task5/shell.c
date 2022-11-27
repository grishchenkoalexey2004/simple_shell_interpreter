#include <stdio.h>
#include "lexem_list_types.h"
#include "syntax_tree_types.h"

list_type get_lexem_list(status *program_status);

void print_string_list(list_type lst);

int get_lexem_list_len(list_type lst);

cmd_link build_syntax_tree(list_type lst,int start_index,int end_index);

int main(int argc,char *argv[]){
	status program_status;
 	int lexem_list_len = 0;	
 	list_type lexem_list = NULL;
	program_status = Success;
	cmd_link tree_root = NULL;

	while(1){
		printf("=>");
		lexem_list = get_lexem_list(&program_status);

		if (program_status == Success){
			//print_string_list(lexem_list);
			lexem_list_len = get_lexem_list_len(lexem_list);
			if (lexem_list_len)
				tree_root = build_syntax_tree(lexem_list,0,lexem_list_len-1);
		}

		else if (program_status == Finish){
			print_string_list(lexem_list);
			break;
		}
		else 
			break;

	}
	
	
		 
	

	if (program_status == Error){
		return 1;
	}

	return 0;
}