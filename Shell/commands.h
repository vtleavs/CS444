#ifndef _COMMANDS_H_
#define _COMMANDS_H_


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

/* ANSI Codes from 'Andrejs Cainikovs'@stackoverflow.com */
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#include "parse.h"

typedef int bool;
#define true 1
#define false 2

char  history[200][200];
int histNum;
bool colorOn;
bool pathPrompt;

int cdCommand(struct commandType * com);
int lsCommand(struct commandType * com);
int histCommand(struct commandType * com);
int pwdCommand(struct commandType * com);
int systemCommand(struct commandType * com);
int jobsCommand(struct commandType * com);
int exitCommand(struct commandType * com);
int killCommand(struct commandType * com);
int helpCommand(struct commandType * com);
int whichCommand(struct commandType * com);
int echoCommand(struct commandType * com);

int parseCommand(struct commandType * com);
bool isCommand(char * command, char * comName);
char * buildPrompt();
int recall(int rec);


#endif /* _COMMANDS_H_ */