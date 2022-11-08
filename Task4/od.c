#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define CHAR_BLOCK_LEN 10
void print_first_sym_pos(int position){
	printf("%06d",position);
}

void print_sym(char sym,int in_octal_format){
	if (in_octal_format)
		printf(" %3o",sym);
	
	else{
		switch(sym){
			case '\n':
				printf(" \\n");
				break;
			case '\t':
				printf(" \\t");
				break;

			default:printf(" %2c",sym);
		}
	}
	return;
}

void print_fname_error(char *filename){
	printf("No such file: %s \n",filename);
	return;
}

int main(int argc,char *argv[]){

	char *filename;
	char char_block[CHAR_BLOCK_LEN];
	int fd;
	int print_octal = 0;
	int bytes_read = 0;
	int to_break = 0;
	int first_sym_pos = 1;
	int iteration_count =0;

	if (argc==2)
		filename = argv[1];
	else{
		filename = argv[2];
		print_octal = 1;
	}
	
	fd = open(filename,O_RDONLY);
	if (fd == -1){
		print_fname_error(filename);
		return 1;
	}

	while (!to_break){
		bytes_read = read(fd,char_block,CHAR_BLOCK_LEN);

		if (bytes_read<CHAR_BLOCK_LEN){
			if (bytes_read ==0){//no symbols to print
				if (iteration_count==0)
					printf("000000\n");
				break;
			}
			else
				to_break = 1;
		}

		print_first_sym_pos(first_sym_pos);

		for (int i=0;i<bytes_read;i++){
			print_sym(char_block[i],print_octal);
		}

		printf("\n");
		first_sym_pos+=10;
		iteration_count++;
	}

	close(fd);
	return 0;
}