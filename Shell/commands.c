#include "commands.h"

int parseCommand(struct commandType * com)
{
	char * command = com->command;

	if(command[0] == '!')
		return histCommand(com);

	if(isCommand(command, "cd") == true)
		return cdCommand(com);
	else if(isCommand(command, "ls") == true)
		return lsCommand(com);
	else if(isCommand(command, "hist") == true || isCommand(command, "history") == true)
		return histCommand(com);
	else if(isCommand(command, "pwd") == true)
		return pwdCommand(com);
	else if(isCommand(command, "system") == true)
		return systemCommand(com);
	else if(isCommand(command, "jobs") == true)
		return psCommand(com);
	else if(isCommand(command, "exit") == true)
		return exitCommand(com);
	else if(isCommand(command, "kill") == true)
		return killCommand(com);
	else if(isCommand(command, "help") == true)
		return helpCommand(com);
	else if(isCommand(command, "which") == true)
		return whichCommand(com);
	else if(isCommand(command, "ps") == true)
		return psCommand(com);
	else if(isCommand(command, "echo") == true)
		return echoCommand(com);
	
	return ERR_NOCOMM;
}

bool isCommand(char * command, char * comName)
{
	if(strncmp(command, comName, strlen(comName)) == 0)
		return true;
	return false;
}


char * buildPrompt()
{
	if(pathPrompt == true)
	{
		char dir[100];
		getcwd(dir, sizeof(dir));
		if(colorOn == true)
			printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_CYAN, dir);
		else
			printf("%s", dir);
	}

	return "% " ANSI_COLOR_RESET;
}


int cdCommand(struct commandType * com)
{
    if(com->VarNum < 2)
    {
        fprintf(stderr, "Command 'cd' requires arguments.\n");
        return ERR_COMMFAIL;
    }
	if(chdir(com->VarList[1]) == 0)
	{
		return ERR_SUCCESS;
	}
	return ERR_COMMFAIL;
}

int lsCommand(struct commandType * com)
{
    char * directory;
    DIR * d;
    struct dirent * dir;
    struct stat fileStats;

	char fileType;
	char * fileTypeName;
	char * fileName;
	unsigned int fileSize;

	if(com->VarNum < 2)
	{
		directory = ".";
	}
	else
	{
		directory = com->VarList[1];
    }

	printf(ANSI_COLOR_CYAN "type\tsize\t\tpath\n" ANSI_COLOR_RESET);

	d = opendir(directory);

	while( (dir = readdir(d)) != NULL )
    {
        fileName = dir->d_name;
        fileType = dir->d_type;
        stat(fileName, &fileStats);
        fileSize = fileStats.st_size;

        switch( fileType )
	    {
            case 0:
            	fileTypeName = "???";
				break;
	        case 4:
                fileTypeName = "dir";
            	break;
	        case 8:
       		    fileTypeName = "reg";
	            break;
            default : fileTypeName = "???";
       }
       
	   printf("%s\t%.2f KB\t\t%s\n", fileTypeName, (float)fileSize/1024.0, fileName);
	}
    closedir(d);
    return ERR_SUCCESS;
}

int recall(int rec)
{
    int err;
    err = ERR_COMMFAIL;
    if(rec <= histNum)
    {
        parseInfo * info;
        struct commandType * comType;

        info = parse(history[rec]);
        comType=&info->CommArray[0];

        printf("%s", buildPrompt());
        printf("%s\n", history[rec]);
        err = parseCommand(comType);

		comType->command;
		free_info(info);
	}
	return err;
}

int histCommand(struct commandType * com)
{
	if(com->command[0] == '!')
    {	
		char num[5];
		int i;

		for(i = 1; i < strlen(com->command); i++)
		{
			num[i-1] = (com->command)[i];
		}
 		if(strlen(com->command) == 1)
	 		return recall(histNum-1);
	 	else
		 	return recall(atoi(num));
	}

	if(com->VarNum < 2)
	{
		int i;
		for(i = 0; i < histNum; i = i + 1)
		{
			printf("%i\t%s\n", i, history[i]);
		}
		printf("%i\t%s\n", histNum, com->command);
	}
	else
	{
		int comNum;
		comNum = atoi(com->VarList[1]);
		return recall(comNum);
	}

	return ERR_SUCCESS;
}

int pwdCommand(struct commandType * com)
{
	char dir[100];
	getcwd(dir, sizeof(dir));
    printf("%s\n", dir);
    return ERR_SUCCESS;
}

int systemCommand(struct commandType * com)
{
	if(com->VarNum < 2)
	{
		fprintf(stderr, "Command 'system' requires arguments\n");
		return ERR_COMMFAIL;
	}
	else if(strncmp(com->VarList[1], "color", strlen("color")) == 0)
	{
		if(com->VarNum < 3)
		{
			fprintf(stderr, "Command 'system color' requires arguments 'true' or 'false'\n");
			return ERR_COMMFAIL;
		}
		else if(strncmp(com->VarList[2], "true", strlen("true")) == 0)
		{
		    colorOn = true;
		}
		else if(strncmp(com->VarList[2], "false", strlen("false")) == 0)
		{
		    colorOn = false;
		}
		else
		{
		    fprintf(stderr, "Command 'system color' requires arguments 'true' or 'false'\n");
		    return ERR_COMMFAIL;
		}
	}
	else if(strncmp(com->VarList[1], "prompt", strlen("prompt")) == 0)
	{
		if(com->VarNum < 3)
		{
			pathPrompt = false;
		}
		else if(strncmp(com->VarList[2], "path", strlen("path")) == 0)
		{
			pathPrompt = true;
		}
		else
		{
			fprintf(stderr, "Command 'system prompt' requires the arguments '' or 'path'\n");
			return ERR_COMMFAIL;
		}
	}

	return ERR_SUCCESS;
}

int exitCommand(struct commandType * com) 
{ 
	exit(1);
}

int killCommand(struct commandType * com) 
{ 
	if(com->VarNum < 2)
	{
		fprintf(stderr, "Command 'kill' requires arguments.\n");
		return ERR_COMMFAIL;
	}
	else
	{
		int i;
		for(i = 1; i < processNum; i++)
		{
			if(processIds[i] == atoi(com->VarList[1]))
			{
				kill(processIds[i], SIGKILL);
				processes[i] = "--dead--";
				processIds[i] = -1;
			}
		}
	}


	return 0;
}

int helpCommand(struct commandType * com) 
{ 
	if(com->VarNum < 2)
	{
		printf("BenjOS Shell v.01 UNIX Edition Manual\n\n"
				"Built-in Commands:\n\n");

		printf("\t" ANSI_COLOR_CYAN "help <" ANSI_COLOR_MAGENTA "command" ANSI_COLOR_CYAN ">" ANSI_COLOR_RESET " Access shell manual.\n"
				"\t\t" ANSI_COLOR_MAGENTA "no args" ANSI_COLOR_RESET ": show this manual\n"
				"\t\t" ANSI_COLOR_MAGENTA "command" ANSI_COLOR_RESET ": specify a built-in command to view the manual for that command\n\n");

		printf("\t" ANSI_COLOR_CYAN "which <" ANSI_COLOR_MAGENTA "program" ANSI_COLOR_CYAN ">" ANSI_COLOR_RESET " Locate active program.\n"
				"\t\t" ANSI_COLOR_MAGENTA "program" ANSI_COLOR_RESET ": specify program to locate\n\n");
				
		printf("\t" ANSI_COLOR_CYAN "ps" ANSI_COLOR_RESET " View active processes.\n"
				"\t\t" ANSI_COLOR_MAGENTA "no args" ANSI_COLOR_RESET ": show all active child processes of this shell\n\n");
				
		printf("\t" ANSI_COLOR_CYAN "!" ANSI_COLOR_MAGENTA "###"  ANSI_COLOR_RESET " Repeat specific command (find command number with 'hist').\n"
				"\t\t" ANSI_COLOR_MAGENTA "###" ANSI_COLOR_RESET ": specify id of the command in history to repeat\n\n");

		printf("INCOMPLETE LIST\n\n");
	}

	return ERR_SUCCESS; 
}

int whichCommand(struct commandType * com) 
{	
	char * path;
	char pathDir[100];
	int i;
	int j;

	if(com->VarNum < 2)
		return ERR_COMMFAIL;

	path = getenv("PATH");
	
	j = 0;
	for(i = 0; i < strlen(path); i++)
	{
		if(path[i] != ':')
		{
			pathDir[i-j] = path[i];
		}
		else
		{
			int k;
			DIR * d;
			struct dirent * dir;
			int comp;

			pathDir[i-j] = '\0';
			j = i+1;

			d = opendir(pathDir);
		
			while( d != NULL && (dir = readdir(d)) != NULL )
			{
				comp = strncmp(com->VarList[1], dir->d_name, strlen(dir->d_name));
				if(comp == 0)
				{
					printf("%s/%s\n", pathDir, dir->d_name);
					return ERR_SUCCESS;
				}

			}
			
			closedir(d);
		}
	}

	if(i == strlen(path))
	{
		printf("'%s' is not a program in the path.\n", com->VarList[1]);
		return ERR_SUCCESS;
	}

	return ERR_COMMFAIL;

}

int psCommand(struct commandType * com) 
{
	int i;
	printf("%i/100 processes used.\n", processNum);
	for(i = 0; i < processNum; i++)
	{
		printf("%i\t%s\n", processIds[i], processes[i]);
	}
	return ERR_SUCCESS;
}

int echoCommand(struct commandType * com) 
{
	int i;
	for(i = 1; i < com->VarNum; i++)
	{
		printf("%s ", com->VarList[i]);
	}
	printf("\n");
	return ERR_SUCCESS; 
}






