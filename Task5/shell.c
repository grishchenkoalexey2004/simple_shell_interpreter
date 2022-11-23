#include <stdio.h>
#include "lexem_list_types.h"

list_type get_lexem_list(int *program_status);


int main(int argc,char *argv[]){
	int continue_reading = 1;
	list_type lexem_list = NULL;

	while (continue_reading){
		printf(">>");
		lexem_list = get_lexem_list(&continue_reading);


	}

	return 0;
}