#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <signal.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <limits.h>
#include "parse.h"   /*include declarations for parse-related structs*/
#include "commands.h"
#include "datastruct.h"
/*
char * processes[100];
int processIds[100];
int processNum;
*/
void init()
{
	histNum = 0;
	colorOn = true;
	pathPrompt = true;
	

	processes[0] = "benjos-shell-unix-0.1";
	processIds[0] = 0;
	processNum = 1;

	system("clear");
	if(colorOn)
		fprintf(stdout, ANSI_COLOR_CYAN "BenjOS Shell UNIX V0.1\n" ANSI_COLOR_RESET);
	else 
		fprintf(stdout, "BenjOS Shell UNIX V1.0\n");
}

int shell()
{
	char * cmdLine;
  	parseInfo *info; /*info stores all the information returned by parser.*/
	
	while(1)
  	{
    	cmdLine = readline(buildPrompt());
    	if (cmdLine == NULL) 
		{
      		fprintf(stderr, "Unable to read command\n");
      		continue;
    	}
    	/*calls the parser*/
    	info = parse(cmdLine);
    	if (info == NULL)
		{
      		free(cmdLine);
      		continue;
    	}
		
		runMultiCommands(info, cmdLine);

    	free_info(info);
    	free(cmdLine);

	} /* while (1) */
}

int runMultiCommands(parseInfo * info, char * cmdLine)
{
	struct commandType * com;
	bool background;
	bool inFile;
	bool outFile;
	int commandErr;
	FILE * infile;
	FILE * outfile;

	int old_stdout;
	int old_stdin;

	inFile = false;
	outFile = false;

   	/*com contains the info. of the command before the first "|"*/
   	com=&info->CommArray[0];
   	if ((com == NULL)  || (com->command == NULL)) 
	{
     	free_info(info);
     	free(cmdLine);
     	return ERR_COMMFAIL;
   	}
	
	commandErr = parseCommand(com);
	
	if(strlen(info->outFile) > 0)
	{
		outfile = fopen(info->outFile,"w");
		
		if(outfile == NULL)
		{
			fprintf(stderr, "Output file '%s' failed to open.", info->outFile);
			return ERR_COMMFAIL;
		}
		
		outFile = true;
		old_stdout = dup(STDOUT_FILENO);
		dup2(fileno(outfile), STDOUT_FILENO);
	}
	if(strlen(info->inFile) > 0)
	{
		infile = fopen(info->inFile, "r");
		
		if(infile == NULL)
		{
			fprintf(stderr, "Input file '%s' failed to open.", info->inFile);
		}
		
		inFile = true;
		old_stdin = dup(STDIN_FILENO);
		dup2(fileno(infile), STDIN_FILENO);
	}


	if(info->boolBackground == 0)
		background = false;
	else
		background = true;
	if(commandErr == ERR_NOCOMM)
	{
		printf("'%s' is not a built-in command; Checking path programs...\n", com->command);
		commandErr = runProgram(com, background);
	}	
	else if(commandErr == ERR_COMMFAIL)
	{
		fprintf(stderr, "Command '%s' did not complete successfully.\n", com->command);
	}
		if(commandErr == ERR_NOPROG)
	{
		fprintf(stderr, "Program '%s' does not exist.\n", com->command);
	}
	else if(commandErr == ERR_PROGFAIL)
	{
		fprintf(stderr, "Program '%s' did not complete successfully.\n", com->command);
	}
	
	strcpy(history[histNum], cmdLine);
	histNum = histNum + 1;

	if(outFile == true)
	{	
		info->outFile[0] = '\0';
		dup2(old_stdout, STDOUT_FILENO);
		close(old_stdout);
		fclose(outfile);
	}
	if(inFile == true)
	{
		info->inFile[0] = '\0';
		dup2(old_stdin, STDIN_FILENO);
		close(old_stdin);
		fclose(infile);
	}

	return commandErr;
}

int runCommands(struct commandType * com)
{

}

int runPrograms(parseInfo * info)
{
	int i;
	for(i = 0; i < info->pipeNum; i++)
	{
		print_info(info);
	}
}

int runProgram(struct commandType * com, bool bg)
{
	int processId;
	int execErr;
	char command[100];

	int pipefd[2];
	pipe(pipefd);

/*
	child_stdin = dup(stdin);
	child_stdout = dup(stdout);
	child_stderr = dup(stderr);
*/
	strcpy(command, com->command);
	command[strlen(com->command)] = '\0';
	
	processId = fork();
	if(processId < 0)
	{
		fprintf(stderr, "Unable to create new process.\n");
		return ERR_PROGFAIL;
	}
	else if(processId == 0)
	{
		/*CHILD*/
		dup2(pipefd[0], STDIN_FILENO);
		
		if(bg == true)
			dup2(pipefd[1], STDOUT_FILENO);

		close(pipefd[0]);
		close(pipefd[1]);

		if(execvp(command, com->VarList) != 0)
		{
			return -1;
		}
		exit(1);
	}
	else
	{
		/*PARENT*/
		
		close(pipefd[0]);
		close(pipefd[1]);

		if(bg == false)
		{
			waitpid(processId);
		}
		else
		{
			addProcess(command, processId);
			printf("New process for '%s' with processId '%i' created.\n", command, processId);
		}
	}

	return ERR_SUCCESS;
}

int addProcess(char * name, int processId)
{
	if(processNum == 100)
		return -1;
	

	processIds[processNum] = processId;
	processes[processNum] = name;
	processNum++;
	
	return 0;
}

int removeProcess(int index)
{
	if(index < 0 || index >= 100)
		return -1;
	
	processes[index] = "";
	processIds[index] = 0;

	if(index < processNum)
	{
		int i;
		for(i = index; i < processNum; i++)
		{
			processes[i] = processes[i+1];
			processIds[i] = processIds[i+1];
		}
	}

	processNum--;

	return 0;
}

int findProcessById(int processId)
{
	int i;
	for(i = 0; i < processNum; i++)
	{
		if(processIds[i] == processId)
			return i;
	}
	return -1;
}

int findProcessByName(char * name)
{
	int i;
	for(i = 0; i < processNum; i++)
	{
		if(strncmp(processes[i], name, strlen(name)))
			return i;
	}
	return -1;
}

int main (int argc, char **argv)
{
#ifndef UNIX
	fprintf(stderr, "Shell only compatible with UNIX systems.\n");
	return 1;
#endif

	int err;
  	
	init();
	err = shell();

	return err;
}

