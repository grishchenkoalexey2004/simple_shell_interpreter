#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define CHAR_BLOCK_LEN 10

//todo : add foolproof (incorrect file name)

//position of arbitrary error
long long dif_line_num = 1;
long long dif_byte_num = 1;

//returns 1 if first cmp_bytes_count of char blocks are equal
//returns 0 if different
int cmp_ch_blocks(char arr1[],char arr2[],int cmp_bytes_count){
	
	for (int i=0;i<cmp_bytes_count;i++){

		if (arr1[i]==arr2[i]){
			if (arr1[i]=='\n'){
				dif_line_num++;
				dif_byte_num = 1;
			}
			else{
				dif_byte_num++;
			}
		}
		else
			return 0;
	}
	return 1;
}

void print_difference_message(char *filename1,char *filename2){
	if (!filename2) //in case second parameter is stdin
		printf("%s differs from stdin : line %lld  byte %lld\n",filename1,dif_line_num,dif_byte_num);
	else
		printf("%s differs from %s : line %lld  byte %lld\n",filename1,filename2,dif_line_num,dif_byte_num);
	return;
}

void print_fname_error(char *filename){
	printf("No such file %s \n",filename);
	return ;
}
int main(int argc,char *argv[]){
	const int stdin_fd = 0;
	const char arg_error_message[] = "not enough arguments";

	int fd_1,fd_2;
	int difference_found = 0;
	
	int file1_ended,file2_ended;
	int bytes_read_1,bytes_read_2,min_bytes_read;
	int comparison_res;
	char char_block1[CHAR_BLOCK_LEN];
	char char_block2[CHAR_BLOCK_LEN];

	file1_ended = file2_ended = 0;

	if (argc==1){
		printf("%s",arg_error_message);
		return 1;
	}
	else{
		fd_1 = open(argv[1],O_RDONLY);
		if (!argv[2])
			fd_2 = stdin_fd;
		else
			fd_2 = open(argv[2],O_RDONLY);

		if (fd_1==-1){
			print_fname_error(argv[1]);
			return 1;
		}

		if (fd_2==-1){
			print_fname_error(argv[2]);
			return 1;
		}

		while (!difference_found && !file1_ended &&!file2_ended){
			bytes_read_1 = read(fd_1,char_block1,sizeof(char)*CHAR_BLOCK_LEN);
			bytes_read_2 = read(fd_2,char_block2,sizeof(char)*CHAR_BLOCK_LEN);
			if (bytes_read_1<CHAR_BLOCK_LEN)
				file1_ended = 1;
			if (bytes_read_2<CHAR_BLOCK_LEN)
				file2_ended = 1;

			if (bytes_read_1!=bytes_read_2){
				(bytes_read_1>bytes_read_2)?(min_bytes_read=bytes_read_2):(min_bytes_read=bytes_read_1);
				difference_found = 1;
			}
			else
				min_bytes_read = bytes_read_2;

			//the function is invoked even if bytes_read_1 != bytes_read_2 in order 
			//to find the position of the difference
			comparison_res = cmp_ch_blocks(char_block1,char_block2,min_bytes_read);

			if (comparison_res==0)
				difference_found = 1;
		}

		if (file1_ended!=file2_ended){
			print_difference_message(argv[1],argv[2]);
		}
		else{
			if (difference_found)
				print_difference_message(argv[1],argv[2]);
			else
				printf("Files are identical!\n");
		}

		close(fd_1);
		if (fd_2!=stdin_fd)
			close(fd_2);
	}


	return 0;
}