#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern int errno;

int getdirectory (char *cwd, size_t size) {
	if (getcwd(cwd, size) != NULL)
       return 1;
   else
       perror("getcwd() error");
   return 0;
}

void fill_argv(char **argv, char *word, int i) {
	char buffer[100] = "";
	strncpy(buffer, word, strlen(word));


	argv[i] = (char *)malloc(strlen(buffer) + 1);
	bzero(argv[i], strlen(buffer) + 1);

	strncpy(argv[i], buffer, strlen(buffer));
	/*printf("%s\n", argv[i]);
	int f = strlen(argv[i]);
	printf("%d\n", f);*/
	bzero(word,100);
}

void free_argv(char *argv[])
{
	int i;
	for(i=0;argv[i]!=NULL;i++) {
		bzero(argv[i], strlen(argv[i])+1);
		argv[i] = NULL;
		free(argv[i]);
	}
}

int main(void) {
	char c = '\0';
	size_t size = 1024;
	char cwd[size];
	int error;
	char *argv[100];
	char *word = (char *)malloc(sizeof(char) * 100);
	word[0] = '\0';
	char *cmd = (char *)malloc(sizeof(char) *100);

	error = getdirectory(cwd, size);

	if(fork() == 0) {
		execvp("clear", argv);
		exit(1);
	} else {
		wait(NULL);
	}

	printf("RSI: %s > ", cwd);

    int i = 0;
	while(c != EOF) {
		/* get each character */
		c = getchar();
		switch(c) {
			/* if user presses ENTER */
			case '\n':
			if(word[0] == '\0')
				printf("RSI: %s > ", cwd);
			else {
				fill_argv(argv,word,i);
				argv[i+1] = NULL;
				//argv
				i=0;
				strncpy(cmd, argv[0], strlen(argv[0]));
				if(fork() == 0) {
					int n;
					n = execvp(cmd, argv);
					printf("errno: %d\n", errno);
					if(n<0) {
						printf("%s: %s\n", cmd, "command not found");
						exit(1);
					}
				} else {
					wait(NULL);
				}
				error = getdirectory(cwd,size);
				printf("Your command is %s \n", *argv);
				printf("RSI: %s > ", cwd);
				free_argv(argv);
			}
			break;

			default: 
			if(!isspace(c)) {
				strncat(word,&c, 1);
			} else {
				fill_argv(argv, word, i);	
				i++;
			}
			break;
		}


			
	}

	printf("\n");
	return 0;
    
	
}