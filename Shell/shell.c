#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <limits.h>
#include "parse.h"   /*include declarations for parse-related structs*/
#include "commands.h"

enum BUILTIN_COMMANDS { NO_SUCH_BUILTIN=0, 
						EXIT,
						JOBS};
void init()
{
	histNum = 0;
	#ifdef UNIX
		colorOn = true;
		pathPrompt = true;

		system("clear");
		if(colorOn)
			fprintf(stdout, ANSI_COLOR_CYAN "BenjOS Shell V1.0\n" ANSI_COLOR_RESET);
		else 
			fprintf(stdout, "BenjOS Shell V1.0\n");
		fprintf(stdout, "This is the UNIX version\n");
	#endif

	#ifdef WINDOWS
    	fprintf(stdout, "This is the WINDOWS version\n");
	#endif
}

int shell()
{
	char * cmdLine;
  	parseInfo *info; /*info stores all the information returned by parser.*/
  	struct commandType *com; /*com stores command name and Arg list for one command.*/
	int commandErr;
  	
	while(1)
  	{
#ifdef UNIX
    	cmdLine = readline(buildPrompt());
    	if (cmdLine == NULL) 
		{
      		fprintf(stderr, "Unable to read command\n");
      		continue;
    	}
#endif

    	/*insert your code about history and !x !-x here*/

    	/*calls the parser*/
    	info = parse(cmdLine);
    	if (info == NULL)
		{
      		free(cmdLine);
      		continue;
    	}
		/*prints the info struct*/
		/*print_info(info); */

    	/*com contains the info. of the command before the first "|"*/
    	com=&info->CommArray[0];
    	if ((com == NULL)  || (com->command == NULL)) 
		{
      		free_info(info);
      		free(cmdLine);
      		continue;
    	}
    
		commandErr = parseCommand(com);
	
		if(commandErr != 0)
		{
			fprintf(stderr, "Command '%s' did not complete successfully.\n", com->command);
		}


		strcpy(history[histNum], cmdLine);
		histNum = histNum + 1;

    	free_info(info);
    	free(cmdLine);

	} /* while (1) */
}


int main (int argc, char **argv)
{
	int err;
  	
	init();
	err = shell();

	return err;
}

