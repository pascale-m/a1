/* Simple Command Line Application

Functionality:

Change Directory
Get Name of Current Directory

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>

extern int errno;

int getdirectory (char *cwd, size_t size) {
	if (getcwd(cwd, size) != NULL)
       return 0;
   else
       perror("getcwd() error");
   return 1;
}

void fill_argv(char **argv, char *word, int argc) {
	char buffer[100] = "";
	strncpy(buffer, word, strlen(word));


	argv[argc] = (char *)malloc(strlen(buffer) + 1);
	bzero(argv[argc], strlen(buffer) + 1);

	strncpy(argv[argc], buffer, strlen(buffer));
	/*printf("%s\n", argv[argc]);
	int f = strlen(argv[argc]);
	printf("%d\n", f);*/
	bzero(word,100);
}

void free_argv(char *argv[])
{
	int argc;
	for(argc=0;argv[argc]!=NULL;argc++) {
		bzero(argv[argc], strlen(argv[argc])+1);
		argv[argc] = NULL;
		free(argv[argc]);
	}
}

/*void start_up(char *argv[], char *cwd, size_t size) {
	
}*/

void execute(char **argv) {
	int n;
	n = execvp(argv[0], argv);
	printf("errno: %d\n", errno);
	if(n<0) {
		printf("%s: %s\n", argv[0], "command not found");
		exit(1);
	}
}

int check_cd(char *cmd, char *cwd, int argc) {
	if(strcmp(cmd, "cd") == 0) {
		if(argc == 0) {
			printf("You must enter a directory to switch into.");
		} else if(argc > 1) {
			printf("The command cd only takes 2 arguments.");
		} else {
			return 1;
		}
	}
	return 0;
}

void change_directory(char *cwd, char *directory) {
	struct passwd *password = getpwuid(getuid());
	char *homedirectory = password->pw_dir;
	bzero(cwd, strlen(cwd));

	if(strncmp(directory, "~", 1)==0) {
		//change to the home directory
		strncpy(cwd, homedirectory, strlen(homedirectory));

		//add path to directory
		if(strcmp(directory, "~/") > 0) {
			char path[strlen(directory) - 1];
			int i;
			for(i=0; i<strlen(directory)-1; i++) {
				path[i] = directory[i+1];
			}
			strncat(cwd, path, strlen(path));
		}	
	} else if(strncmp(directory, "..", 2)==0) {
	  
	        getdirectory(cwd, 1024);
		char *path = (char*)malloc(strlen(cwd) + 1);
		int i = strlen(cwd)-1;

		while(cwd[i]!= '/') {
			--i;
		}
		strncpy(path, cwd, i);

		bzero(cwd, strlen(cwd));
		strncpy(cwd, path, strlen(path));
	        } else {
		strcpy(cwd, directory);
	}



	if(chdir(cwd) == -1) {
		printf("errno: %d\n", errno);
	}
}

int main(void) {
	char c = '\0';
	size_t size = 1024;
	char cwd[size];
	char *argv[100];
	argv[0] = NULL;
	char *word = (char *)malloc(sizeof(char) * 100);
	word[0] = '\0';
	char *cmd = (char *)malloc(sizeof(char) *100);
	int is_valid_cd;

	//start_up(argv, cwd, size);
	int error = 1;
	do {
		error = getdirectory(cwd, size);
	} while(error);

	if(fork() == 0) {
	  if(execvp("clear", argv) == -1) {
	    printf("clearing screen did not work. errno: %d\n", errno);
	  }
		exit(1);
	} else {
		wait(NULL);
	}

	printf("RSI: %s > ", cwd);
	fflush(stdout);

    int argc = 0;
	while(c != EOF) {
		/* get each character */
		c = getchar();
		switch(c) {
			/* if user presses ENTER */
			case '\n':
			if(word[0] == '\0') {
				printf("you typed in no command\n");
				printf("RSI: %s > ", cwd);
			}else {
				fill_argv(argv,word,argc);
				argv[argc+1] = NULL;

				strncpy(cmd, argv[0], strlen(argv[0]));

				//check if command is cd
				is_valid_cd = check_cd(cmd, cwd, argc);

				//if it is a cd function with 2 arguments, change directory
				if(is_valid_cd) {	
					change_directory(cwd, argv[1]);
				//else, just fork and execute process
				} else {
					if(fork() == 0) {
					execute(argv);
					} else {
						wait(NULL);
					} 
				}
				getdirectory(cwd, size);
				printf("RSI: %s > ", cwd);
				free_argv(argv);

				//reset number of arguments
				argc=0;
			}
			break;

			default: 
			if(!isspace(c)) {
				strncat(word,&c, 1);
			} else {
				fill_argv(argv, word, argc);	
				argc++;
			}
			break;
		}


			
	}

	printf("\n");
	return 0;
    
	
}
