#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main() {
	// Uncomment this block to pass the first stage
	printf("$ ");
	fflush(stdout);

	// Wait for user input
	char input[100];
	fgets(input, 100, stdin);

	int cmdLen = 0;
	for (; cmdLen < 100; cmdLen++) {
		switch (input[cmdLen]) {
			case 'a'...'z':
			case 'A'...'Z':
			case '0'...'9':
				break;
			default:
				goto cmdEnd;
		}
	}
cmdEnd:;
	char cmd[100] = {0};
	strncpy(cmd, input, cmdLen);
	printf("%s: command not found\n", cmd);

	return 0;
}
