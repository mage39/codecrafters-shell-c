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

		char cmd[100] = "exit";
		int cmdLen = strlen(cmd);
		if (!strncmp(input, cmd, cmdLen)) {
			int ret = atoi(input + cmdLen);
			return ret;
		}
		strncpy(cmd, "echo", 100);
		cmdLen = strlen(cmd);
		if (!strncmp(input, cmd, cmdLen)) {
			printf("%s", input + cmdLen + 1);
			continue;
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
		strncpy(cmd, input, cmdLen);
		printf("%s: command not found\n", cmd);
	}

	return 0;
}
