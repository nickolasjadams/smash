#define _GNU_SOURCE
#define MAXLINE 4096
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "smash.h"
#include "history.h"



int main(int argc, char const *argv[])
{
	setvbuf(stdout,NULL,_IONBF,0); //Disable buffering in the stdout stream
	
	init_history();
	
	char bfr[MAXLINE];
	int spacesOnly = 0; // false
	
	fputs("$ ",stderr); //Output the first prompt
	
	//Loop reading commands until EOF or error
	while (fgets(bfr, MAXLINE, stdin) != NULL) {
		
		bfr[strlen(bfr) - 1] = '\0'; //Replace newline with NUL
		
		if (bfr[0] != '\0') {
			if (isspace(bfr[0]) != 0) {
				spacesOnly = 0;
				for (int i = 0; i < strlen(bfr)-1; i++) {
					if (isspace(bfr[i]) != 0) {
						spacesOnly++;
					}
				}
				if (spacesOnly != strlen(bfr)-1) {
					executeCommand(bfr);
				}
			} else {
				executeCommand(bfr);
			}
		}
		
		
		fputs("$ ",stderr);

	}
	
	//clear_history(); // doesn't do anything yet. No explicit mallocs
	exit(0);
	return 0;
	

}
