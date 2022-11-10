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

void print_detailed_info(char *filepath,int show_group);
void print_file_name(Dirent *dp);
void dirwalk(char *dir,int show_det_stat,int print_recursively,int show_group);
void fsize(char *filenаmе);


void print_file_name(Dirent *dp){
	printf("%s",dp->d_name);
	return;
}

void print_detailed_info(char *filepath,int show_group){
	struct stat stbuf;
	short file_type;
	struct passwd *author_info;
	struct group *group_info;
	mode_t file_rights_flags[9] = {S_IRUSR,S_IWUSR,S_IXUSR,S_IRGRP,S_IWGRP,S_IXGRP,S_IROTH,S_IWOTH,S_IXOTH};

	if (stat(filepath, &stbuf) == -1) {
		fprintf(stderr, "No access to %s\n",filepath );
		return;
	}

	if (S_ISREG(stbuf.st_mode))
		file_type =1;  
	else if (S_ISDIR(stbuf.st_mode))
		file_type = 2;
	else if (S_ISCHR(stbuf.st_mode))  
		file_type = 3;
	else if (S_ISBLK(stbuf.st_mode))
        file_type = 4;
    else if (S_ISFIFO(stbuf.st_mode))
    	file_type = 5;
    else if (S_ISLNK(stbuf.st_mode))
    	file_type = 6;
   	else
   		file_type = 7;//socket type

	printf(" %d",file_type);
	printf(" ");

	//applying various masks to st_mode field in order to ascertain owner, group and other users' rights
	for (int i=0;i<=8;i++){
		if (file_rights_flags[i] & stbuf.st_mode){
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
	author_info = getpwuid(stbuf.st_uid);
	printf("%s",author_info->pw_name);

	printf(" ");
	if (show_group){
		group_info = getgrgid(stbuf.st_gid);
		printf("%s",group_info->gr_name);
		printf(" ");
	}
	printf("\n");


	return;	
}

//arg : filepath of file in the directory 
//returns: size of the file (or catalogue)

void fsize(char *filepath)
{
	struct stat stbuf;

	if (stat(filepath, &stbuf) == -1) {
		fprintf(stderr, "No access to %s\n",filepath );
		return;
	}

	if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
		dirwalk(filepath);

	printf("%8ld %s\n", stbuf.st_size, filepath);
	return;
}


/* dirwalk: applies set of functions to every file in dir(filepath of directory)*/

void dirwalk(char *dir,int show_det_stat,int print_recursively,int show_group)
{

	char filepath [MAX_PATH_LEN];
	Dirent *dp;
	DIR *dfd;

	/*opendir opens a stream to directory*/
	if ((dfd = opendir(dir))== NULL) {
		fprintf(stderr, "can't open %s\n!", dir);
		return;
	}

	/*readdir returns a pointer to the structure that contains 
	information about a file (or a catalogue)*/
	while ((dp = readdir(dfd)) != NULL) {
		if (strcmp(dp->d_name, ".") ==0 || strcmp(dp->d_name, "..") ==0)
			continue; /* dismiss oneself and parent directory */

		if (strlen(dir)+strlen(dp->d_name)+2 > sizeof(filepath))
			fprintf(stderr,"dirwalk: too long path %s/%s\n",dir, dp->d_name);

		else {
			sprintf(filepath, "%s/%s", dir, dp->d_name);
			//writing location of the current file into filepath
			print_file_name(dp);
			if (show_det_stat)
				print_detailed_info(filepath,show_group);
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
	dirwalk(path,show_detailed_stat,process_recursively,show_group);
	return 0;
}

//use dirwalk
