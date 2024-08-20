#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main() {
	while (1) {
		// Uncomment this block to pass the first stage
		printf("$ ");
		fflush(stdout);

		// Wait for user input
		char input[100];
		fgets(input, 100, stdin);

		int cmdLen = strlen("exit");
		if (!strncmp(input, "exit", cmdLen)) {
			int ret = atoi(input + cmdLen);
			return ret;
		}

		cmdLen = 0;
		for (; cmdLen < 100; cmdLen++) {
			switch (input[cmdLen]) {
				case 'a'...'z':
				case 'A'...'Z':
				case '0'...'9':
				case '-'...'.':
				case '_':
					break;
				default:
					goto cmdEnd;
			}
		}
cmdEnd:;
		char cmd[100] = {0};
		strncpy(cmd, input, cmdLen);
		printf("%s: command not found\n", cmd);
	}

	return 0;
}
