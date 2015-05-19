#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int getdirectory (char *cwd, size_t size) {
	if (getcwd(cwd, size) != NULL)
       return 1;
   else
       perror("getcwd() error");
   return 0;
}

int main(int argc, char *argv[], char *envp[]) {
	char c = '\0';
	size_t size = 1024;
	char cwd[size];
	int error;

	error = getdirectory(cwd, size);
    printf("RSI: %s >", cwd);
	while(c!=EOF) {
		c = getchar();
		if(c == '\n') {
			printf("RSI: %s >", cwd);
		}	
	}

	printf("\n");
	return 0;
    
	
}