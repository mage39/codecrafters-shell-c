#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static inline int isValidChar (char a) {
	switch (a) {
		case 'a'...'z':
		case 'A'...'Z':
		case '0'...'9':
		case '-'...'.':
		case '_':
			return 1;
		default:
			return 0;
	}
}

static const char* typeFileSearch(const char* cmd) {
	const char* pathEnv = getenv("PATH");
	int pathEndPtr = 0, pathBeginPtr = 0;
	while (pathEndPtr < strlen(pathEnv)) {
		for (; pathEndPtr < strlen(pathEnv); pathEndPtr++) {
			if (pathEnv[i] == ':') break;
		}
		char path[100] = {0};
		strncpy(path, pathEnv + pathBeginPtr, pathBeginPtr - pathEndPtr);
		DIR* pathDir = opendir(path);
		struct dirent* temp;
		while(temp = readdir(pathDir)) {
			// compare this shit
			// im going to bed
		}
	}
}

int main () {
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
			#define CMD_COUNT 3
			char builtins[CMD_COUNT][8] = {"exit", "echo", "type"};
			for (int i = 0; i < CMD_COUNT; i++) {
				typeLen = strlen(builtins[i]);
				if (!strncmp(input + cmdLen, builtins[i], typeLen)) {
					printf("%s is a shell builtin\n", builtins[i]);
					goto whileContinue;
				}
			}
			typeLen = 0;
			for (; cmdLen < 100; cmdLen++) {
				if (isValidChar(input[cmdLen])) typeLen++;
				else break;
			}
			cmdLen = strlen(cmd);
			strncpy(cmd, "\0", cmdLen);
			strncpy(cmd, input + cmdLen, typeLen);
			if (!cmd[0]) continue;
			char* cmdLocation = typeFileSearch(cmd);
			if (cmdLocation) {
				printf("%s is %s", cmd, cmdLocation);
				free(cmdLocation);
				continue;
			}
			printf("%s: not found\n", cmd);
			continue;
		}

		cmdLen = 0;
		for (; cmdLen < 100; cmdLen++) {
			if (isValidChar(input[cmdLen]));
			else break;
		}
		strncpy(cmd, input, cmdLen);
		printf("%s: command not found\n", cmd);
whileContinue:;
	}

	return 0;
}
