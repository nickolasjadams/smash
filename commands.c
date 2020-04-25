#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "smash.h"
#include "history.h"


void executeCommand(char *str) {
	
	char *allArgs = strdup(str);
	

	// Always checks for "exit" in the first 4 characters of bfr
	if (strncmp(str, "exit", 4) == 0) {
		exit(0);
	}



	
	char * args[1000];
	char * token;

	int tokenCount = -1;
	int argc;



	token = strtok(str, " ");
	


	while (token != NULL) {
		++tokenCount;
		args[tokenCount] = token;

		token = strtok(NULL, " ");
	}

	argc = tokenCount + 1;



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



	// else must be external

	else {

		add_history(allArgs, 127);

		int i = 0;

		while (i < argc) {
			printf("[%d] %s\n", i, args[i]);
			++i;
		}

	}
	
	free(allArgs);


}
