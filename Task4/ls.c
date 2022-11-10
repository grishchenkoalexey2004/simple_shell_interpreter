/*the program prints names of all files stored in the catalogue*/

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_PATH_LEN 1024

typedef struct dirent Dirent;

//void dirwalk(char *dir, void (*fcn)(char *));
void dirwalk(char *dirname);
void fsize(char *filenаmе);

//arg : filepath of file in the directory 
//returns: size of the file (if filepath is not catalogue)


void fsize(char *filename)
{
	struct stat stbuf;

	if (stat(filename, &stbuf) == -1) {
		fprintf(stderr, "fsize: нет доступа к %s\n",filename );
		return;
	}

	if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
		dirwalk(filename);

	printf("%8ld %s\n", stbuf.st_size, filename);
	return;
}


/* dirwalk: applies set of functions to every file in dir(filepath of directory)*/

void dirwalk(char *dir)
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
			sprintf(filepath, "%s/%s", dir, dp->d_name);//writing location of the current file into filepath
			printf("%s\n",dp->d_name);
		}
	}	

	closedir(dfd);
	return;
}

int main(){
	char path[MAX_PATH_LEN+1];

	getcwd(path,MAX_PATH_LEN+1);
	dirwalk(path);
	return 0;
}

//use dirwalk
