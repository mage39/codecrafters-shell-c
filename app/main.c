#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main() {
	while (1) {
		printf("$ ");
		fflush(stdout);
		char input[100];
		fgets(input, 100, stdin);

		if (!strcmp(input, "exit\n")) return 0;
		char cmd[100] = "exit ";
		int cmdLen = strlen(cmd);
		if (!strncmp(input, cmd, cmdLen)) {
			int ret = atoi(input + cmdLen);
			return ret;
		}
		strncpy(cmd, "echo ", 100);
		cmdLen = strlen(cmd);
		if (!strncmp(input, cmd, cmdLen)) {
			printf("%s", input + cmdLen);
			continue;
		}
		if (!strcmp(input, "type\n")) continue;
		strncpy(cmd, "type ", 100);
		cmdLen = strlen(cmd);
		if (!strncmp(input, cmd, cmdLen)) {
			int typeLen;
			char builtins[][25] = {"exit", "echo", "type"};
			for (int i = 0; i < 3; i++) {
				typeLen = strlen(builtins[i]);
				if (!strncmp(input + cmdLen, builtins[i], typeLen)) {
					printf("%s is a shell builtin\n", builtins[i]);
					goto whileContinue;
				}
			}
			typeLen = 0;
			for (; cmdLen < 100; cmdLen++) {
				switch (input[cmdLen]) {
					case 'a'...'z':
					case 'A'...'Z':
					case '0'...'9':
					case '-'...'.':
					case '_':
						typeLen++;
						break;
					default:
						goto typeEnd;
				}
			}
typeEnd:
			cmdLen = strlen(cmd);
			strncpy(cmd, input + cmdLen, typeLen);
			printf("%s: not found\n", cmd);
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
whileContinue:;
	}

	return 0;
}
