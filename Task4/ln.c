#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#define PATH_MAX 1024


/*doesn't print error when creating symlink
 if file with filename stored in target_file_name doesn't exist */

int main(int argc,char *argv[]){

	int create_sym_link = 0;
	int index = 1;
	int error;
	char *target_file_name;
	char *link_file_name;
	char symlink_flag[] = "-s";
	char dirpath[PATH_MAX+1];
	char target_path[PATH_MAX+1];
	char link_path[PATH_MAX+1];
	struct stat stbuf;

	getcwd(dirpath,PATH_MAX+1);

	if (strcmp(argv[index],symlink_flag)==0){
		create_sym_link = 1;
		index++;
	}

	target_file_name = argv[index];
	link_file_name = argv[index+1];

	sprintf(target_path, "%s/%s", dirpath, target_file_name);
	sprintf(link_path, "%s/%s", dirpath, link_file_name);

	if (create_sym_link){
		/*this condition is checked in order to avoid creation of dangling link 
		(link that doesn't have target file)
		if the condition is not cheched the program will create dangling link "link_path" in case 
		link_path and target_path don't exist*/
		if (stat(target_path,&stbuf) == -1){
			printf("Target file doesn't exist!\n");
			return 0;
		}

		error = symlink(target_path,link_path);
		if (error){
			perror("Error");
		}
	}
	else{
		error = link(target_path,link_path);
		if (error){
			perror("Error");
		}
	}

	return 0;
}