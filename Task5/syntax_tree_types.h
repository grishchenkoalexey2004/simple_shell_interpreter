typedef struct cmd_inf *cmd_link;

struct cmd_inf {
	char *command_name;
	//char ** argv; // list containing command name and its arguments
	char *infile; // reassigned input file descriptor
	char *outfile; // reassigned output file descriptor
	int backgrnd; // =1 background process flag
	cmd_link psubcmd; // commands for launch in subshell
	cmd_link pipe; // next command after “|”
	cmd_link next; // next command after “;” (of after &&)
	char *separator; // equals to ; && & or || or \0 (if there is no link)
};