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
		return jobsCommand(com);
	else if(isCommand(command, "exit") == true)
		return exitCommand(com);
	else if(isCommand(command, "kill") == true)
		return killCommand(com);
	else if(isCommand(command, "help") == true)
		return helpCommand(com);
	else if(isCommand(command, "which") == true)
		return whichCommand(com);
	else if(isCommand(command, "echo") == true)
		return echoCommand(com);
	
	return -1;
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
        return -1;
    }
    return chdir(com->VarList[1]);
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
    return 0;
}

int recall(int rec)
{
    int err;
    err = 0;
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

	return 0;
}

int pwdCommand(struct commandType * com)
{
    char dir[100];
    getcwd(dir, sizeof(dir));
    printf("%s\n", dir);
    return 0;
}

int systemCommand(struct commandType * com)
{
	if(com->VarNum < 2)
	{
		fprintf(stderr, "Command 'system' requires arguments\n");
		return -1;
	}
	else if(strncmp(com->VarList[1], "color", strlen("color")) == 0)
	{
		if(com->VarNum < 3)
		{
			fprintf(stderr, "Command 'system color' requires arguments 'true' or 'false'\n");
			return -1;
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
		    return -1;
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
			return -1;
		}
	}

	return 0;
}



int jobsCommand(struct commandType * com) { return 0; }

int exitCommand(struct commandType * com) 
{ 
	exit(1);
}

int killCommand(struct commandType * com) { return 0; }
int helpCommand(struct commandType * com) { return 0; }
int whichCommand(struct commandType * com) { return 0; }

int echoCommand(struct commandType * com) 
{
	int i;
	for(i = 1; i < com->VarNum; i++)
	{
		printf("%s ", com->VarList[i]);
	}
	printf("\n");
	return 0; 
}





