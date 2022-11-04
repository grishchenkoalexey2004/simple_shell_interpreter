#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>


int main(){
	char path[PATH_MAX+1];
	getcwd(path,PATH_MAX+1);

	printf("\n%s\n",path);
	return 0;

}