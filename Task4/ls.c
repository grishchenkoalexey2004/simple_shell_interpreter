/*the program prints names of all files stored in the catalogue*/

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>


#define MAX_PATH_LEN 1024

typedef struct dirent Dirent;

//void dirwalk(char *dir, void (*fcn)(char *));

void print_detailed_info(struct stat stbuf,int show_group);

void print_file_name(Dirent *dp);

void dirwalk(char *dir,int show_det_stat,int print_recursively,int show_group,int tab_count);

void fsize(char *filenаmе);




void print_file_name(Dirent *dp){
	printf("%s",dp->d_name);
	return;
}

int is_directory(struct stat stbuf){
	if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
		return 1;
	else
		return 0;
}

//field "st_mode" of function returned by stat is passed as an argument
void print_file_type(mode_t file_type_field){
	short file_type;

	if (S_ISREG(file_type_field))
		file_type =1;  
	else if (S_ISDIR(file_type_field))
		file_type = 2;
	else if (S_ISCHR(file_type_field))  
		file_type = 3;
	else if (S_ISBLK(file_type_field))
        file_type = 4;
    else if (S_ISFIFO(file_type_field))
    	file_type = 5;
    else if (S_ISLNK(file_type_field))
    	file_type = 6;
   	else
   		file_type = 7;//socket type

	printf(" %d",file_type);
	printf(" ");

	return;
}

//field "st_mode" of function returned by stat is passed as an argument
void print_file_rights(mode_t file_type_field){

	mode_t file_rights_flags[9] = {S_IRUSR,S_IWUSR,S_IXUSR,S_IRGRP,S_IWGRP,S_IXGRP,S_IROTH,S_IWOTH,S_IXOTH};

	//applying various masks to st_mode field in order to ascertain owner, group and other users' rights
	for (int i=0;i<=8;i++){
		if (file_rights_flags[i] & file_type_field){
			if (i%3==0)
				printf("r");
			else if (i%3==1)
				printf("w");
			else
				printf("x");
		}
		else
			printf("-");
	}
	printf(" ");

	return;
}

void print_author_name(uid_t user_index){

	struct passwd *author_info;

	author_info = getpwuid(user_index);

	printf("%s",author_info->pw_name);
	printf(" ");
	return;
}

void print_group_name(gid_t group_index){

	struct group *group_info;

	group_info = getgrgid(group_index);

	printf("%s",group_info->gr_name);
	printf(" ");

	return;

}	

void print_file_size(struct stat stbuf){
	mode_t file_type_field = stbuf.st_mode;
	off_t catalogue_size;
	//if file is a directory
	if ((file_type_field & S_IFMT) == S_IFDIR){
		catalogue_size = stbuf.st_size;
		printf("%8ld bytes", catalogue_size);
	}
	else
		printf("%8ld bytes", stbuf.st_size);

	return;
}

void print_detailed_info(struct stat stbuf,int show_group){
	
	print_file_type(stbuf.st_mode);
	print_file_rights(stbuf.st_mode);
	print_author_name(stbuf.st_uid);

	if (show_group)
		print_group_name(stbuf.st_gid);

	print_file_size(stbuf);

	return;	
}

/* dirwalk: applies set of functions to every file in directory)*/
/* set of functions is defined by flags*/
void dirwalk(char *dir,int show_det_stat,int print_recursively,int show_group,int tab_count)
{

	char filepath [MAX_PATH_LEN];
	struct stat stbuf;
	Dirent *dp;
	DIR *dfd;

	/*opendir opens a stream to directory*/
	if ((dfd = opendir(dir))== NULL) {
		fprintf(stderr, "can't open %s\n!", dir);
		_exit(1);
	}

	/*readdir returns a pointer to the structure that contains 
	information about a file (or a catalogue)*/
	while ((dp = readdir(dfd)) != NULL) {
		if (strcmp(dp->d_name, ".") ==0 || strcmp(dp->d_name, "..") ==0)
			continue; /* dismiss oneself and parent directory */

		if (strlen(dir)+strlen(dp->d_name)+2 > sizeof(filepath)){
			fprintf(stderr,"dirwalk: too long path %s/%s\n",dir, dp->d_name);
		}

		else {

			sprintf(filepath, "%s/%s", dir, dp->d_name);
			if (stat(filepath,&stbuf)==-1){
				printf("couldn't access %s",filepath);
			}

			else{
				for (int i=0;i<tab_count;i++){
					printf("\t");
				}

				//writing location of the current file into filepath
				print_file_name(dp);
				if (show_det_stat)
					print_detailed_info(stbuf,show_group);

				printf("\n");

				if (print_recursively && is_directory(stbuf))
					dirwalk(filepath,show_det_stat,print_recursively,show_group,tab_count+1);
			}
		}
	}	
	closedir(dfd);
	return;
}

int main(int argc,char *argv[]){
	char path[MAX_PATH_LEN+1];
	char detailed_stat_flag[] = "-l";
	char recursive_processing_flag[] = "-R";
	char show_group_flag[] = "-g";
	int show_detailed_stat,process_recursively,show_group;
	int tab_count =0;
	int index = 1;
	show_detailed_stat = process_recursively = show_group = 0;

	for (;index<=argc-1;index++){
		if (strcmp(argv[index],detailed_stat_flag)==0)
			show_detailed_stat = 1;
		
		else if(strcmp(argv[index],recursive_processing_flag)==0)
			process_recursively = 1;

		else if (strcmp(argv[index],show_group_flag)==0)
			show_group = 1;

		else
			break;
	}


	getcwd(path,MAX_PATH_LEN+1);
	dirwalk(path,show_detailed_stat,process_recursively,show_group,tab_count);

	return 0;
}

//use dirwalk
