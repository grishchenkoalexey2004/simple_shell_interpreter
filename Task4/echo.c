#include <stdio.h>
#include <string.h>
#include <unistd.h>

int string_index = 0;
//spec_symbol - symbol after backslash 
	

void print_special_symbol(char spec_symbol,int esc_seq_enabled,int *stop_input){
	char backslash = '\\';
	switch (spec_symbol){
		case '\\':
			printf("%c",backslash);
			break;

		case 'a':
			if (esc_seq_enabled)
				printf("\a");
			else
				printf("%c%c",backslash,'a');
			break;

		case 'b':
			if (esc_seq_enabled)
				printf("\b");
			else
				printf("%c%c",backslash,'b');
			break;

		case 'c':
			if (esc_seq_enabled)
				*stop_input = 1;
			else
				printf("%c%c",backslash,'c');
			break;

		case 'e':
			if (esc_seq_enabled)
				printf("\e");
			else
				printf("%c%c",backslash,'e');
			break;

		case 'n':
			if (esc_seq_enabled)
				printf("\n");
			else
				printf("%c%c",backslash,'n');
			break;		

		case 't':
			if (esc_seq_enabled)
				printf("\t");
			else
				printf("%c%c",backslash,'t');
			break;

		case 'r':
			if (esc_seq_enabled)
				printf("\r");
			else
				printf("%c%c",backslash,'r');
			break;

		default:
			printf("\\");
			return;
	}
	string_index+=1;
	return;

}

void print_string(char *string,int esc_seq_enabled,int trailing_newline_enabled){
	//each string is ended with '\0'
	char cur_char;
	char next_char;
	int stop_input = 0;//stop input when 1
	while ((cur_char=string[string_index])!='\0'){
		if (cur_char == '\\'){
			string_index+=1;
			next_char = string[string_index];
			print_special_symbol(next_char,esc_seq_enabled,&stop_input);
		}
		else{
			printf("%c",string[string_index]);
			string_index+=1;
			continue;
		}
		
		if (stop_input)
			break;
	}
	if (trailing_newline_enabled)
		printf("\n");
	return;
}

int main(int argc,char *argv[]){
	int arg_index = 1;
	int esc_seq_enabled = 0;
	int trailing_newline_enabled = 1;
	char enable_esc_seq_flag[]  = "-e";
	char disable_esq_seq_flag[] = "-E";
	char disable_tr_whitespace_flag[] = "-n";

	for (;arg_index<=argc-1;arg_index++){
		if (strcmp(argv[arg_index],enable_esc_seq_flag)==0)
			esc_seq_enabled=1;
		else if (strcmp(argv[arg_index],disable_esq_seq_flag)==0)
			esc_seq_enabled=0;
		else if (strcmp(argv[arg_index],disable_tr_whitespace_flag)==0)
			trailing_newline_enabled=0;
		else
			break;
	}

	for (;arg_index<=argc-1;arg_index++){
		print_string(argv[arg_index],esc_seq_enabled,trailing_newline_enabled);
	}
	return 0;
}