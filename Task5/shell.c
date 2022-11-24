#include <stdio.h>
#include "lexem_list_types.h"

list_type get_lexem_list(status *program_status);


int main(int argc,char *argv[]){
	status program_status;
	list_type lexem_list = NULL;

	while (program_status == Success){
		printf("=>");
		lexem_list = get_lexem_list(&program_status);

	}

	if (program_status == Error){
		return 1;
	}

	return 0;
}