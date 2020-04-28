#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "smash.h"
#include "history.h"

// Defines the function prototypes for the executeExternalCommand function below
int executeExternalCommand(char *str);

// Define the function prototypes implemented in smash.c
int executeExternalCommand(char *str) {

	char *extArgs = strdup(str);

	char * args[1000] = {0}; // null terminates all unfilled spaces
	char * token;
	int tokenCount = -1;
	int argc;

	token = strtok(extArgs, " ");

	while (token != NULL) {
		++tokenCount;
		args[tokenCount] = token;
		token = strtok(NULL, " ");
	}

	argc = tokenCount + 1;

	// now we have a nice args array with the command and args

	// for (int i = 0; i < argc; i++) {
	// 	printf("%s\n", args[i]);
	// }

	//Create a new, duplicate process that will execute the code below
	// fflush(stdout);    //Flush parent's stdout buffer into kernel
  	int pid = fork();  //Fork will return twice, once in each process

  	//Both the parent and the child are executing now.  If they are executing on
  	//a multi-core CPU, both processes may execute this code simultaneously.

  	// Am I the parent process?
	if (pid > 0) {

		// reset the args from the previous command
		// for (int i = 0; i < 1000; i++) {
		// 	args[i] = '\0';
		// }
		// args[argc-1] = '\0';

	    int retrievedStatus;     //Child's status from wait()
	    wait(&retrievedStatus);  //Wait for child to exit
	    return WEXITSTATUS(retrievedStatus); //Isolate exit status bits                          //The parent's normal exit

  	// Am I the child process?
	} else if (pid == 0) {
		// execvp(const char *file, char *const argv[]);
		// char *args[] = {"-la", NULL}; 
		if (execvp(args[0], args) < 0) {
			// execvp failed.
			printf("-smash: %s: command not found\n", args[0]);
			free(extArgs);
			exit(127);
		}
		free(extArgs);
		fclose(stdout);
		exit(0);  //The child exits

	// Did the fork fail?
	} else {
		perror("Fork failed");
	}
	return -1;
}

void executeCommand(char *str) {


	char *allArgs = strdup(str);
	char *extArgs = strdup(str); // make a copy in case we need to pass to external command



	// Always checks for "exit" in the first 4 characters of bfr
	if (strncmp(str, "exit", 4) == 0) {
		exit(0);
	}


	
	char * args[1000];
	char * token;
	int tokenCount = -1;
	// int argc;

	token = strtok(str, " ");
	
	while (token != NULL) {
		++tokenCount;
		args[tokenCount] = token;

		token = strtok(NULL, " ");
	}

	// argc = tokenCount + 1;



	// internal commands
	// - cd
	// - history


	// Check for cd command
	if (strncmp(args[0], "cd", 2) == 0 && strlen(args[0]) == 2) {

		char s[100];


		if (tokenCount == 0) { // no directory provided
			// do nothing
			add_history(allArgs, 0);
		} else if (tokenCount > 1) {
			fprintf(stderr, "Too many parameters.\n");
			add_history(allArgs, 1);
		} else {

			// params good
			// check special chars

			if (strncmp(args[1], "/", 1) == 0 && strlen(args[1]) == 1) {
				chdir("/");
				printf("%s\n", getcwd(s, 100));
				add_history(allArgs, 0);
			}

			else if (strncmp(args[1], "~", 1) == 0 && strlen(args[1]) == 1) {
				chdir(getenv("HOME"));
				printf("%s\n", getcwd(s, 100));
				add_history(allArgs, 0);
			}

			else if (strncmp(args[1], "..", 2) == 0 && strlen(args[1]) <= 3) {
				chdir("..");
				printf("%s\n", getcwd(s, 100));
				add_history(allArgs, 0);
			}

			else {

				// check if directory exists

				if (chdir(args[1]) == -1) {
					add_history(allArgs, 1);
					perror(args[1]);
				} else {
					// does exist
					chdir(args[1]);
					printf("%s\n", getcwd(s, 100));
					add_history(allArgs, 0);
				}

			}

		}

		

	}

	// else if history

	else if (strncmp(args[0], "history", 7) == 0 && strlen(args[0]) == 7) {

		add_history(allArgs, 0);

		print_history(0);

	}



	// else might be external

	else {

		// executeExternalCommand(extArgs);

		add_history(extArgs, executeExternalCommand(extArgs));

	}
	
	free(allArgs);
	free(extArgs);


}
