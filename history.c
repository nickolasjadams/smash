#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
// #include "smash.h"
#include "history.h"


struct Cmd cmdArray[MAXHISTORY];
int cmdCount;

//Builds data structures for recording cmd history
void init_history(void) {

	cmdCount = 0;

}


//Adds an entry to the history
void add_history(char *cmd, int exitStatus) {

	char *dupedCmd = strdup(cmd);

	// check if less than MAXHISTORY

	if (cmdCount < MAXHISTORY) {

		cmdArray[cmdCount].cmd = dupedCmd;
		cmdArray[cmdCount].exitStatus = exitStatus;
		cmdArray[cmdCount].seqNum = cmdCount+1;

	} else {

		for (int n = 0; n < MAXHISTORY; n++) {
			if (n != MAXHISTORY-1) {
				cmdArray[n] = cmdArray[n+1];
			} else {
				cmdArray[MAXHISTORY-1].cmd = dupedCmd;
				cmdArray[MAXHISTORY-1].exitStatus = exitStatus;
				cmdArray[MAXHISTORY-1].seqNum = cmdCount+1;
			}
		}

	}

	cmdCount++;

}


//Frees all malloc’d memory in the history
void clear_history(void) {
	// nothing to free
}


//Prints the history to stdout
void print_history(int firstSequenceNumber) {
	
	for (int i = firstSequenceNumber; i < cmdCount; i++) {
		if (i < MAXHISTORY) {
			// print_struct(i);
			printf("%d [%d] %s\n", cmdArray[i].seqNum, cmdArray[i].exitStatus, cmdArray[i].cmd);
		}
	}

}

void print_struct(int index) {
	if (cmdCount < MAXHISTORY) {
		printf("!%d [%d] %s\n", index+1, cmdArray[index].exitStatus, cmdArray[index].cmd);
	} else {
		printf(".%d [%d] %s\n", index+1, cmdArray[index].exitStatus, cmdArray[index].cmd);
	}
}
