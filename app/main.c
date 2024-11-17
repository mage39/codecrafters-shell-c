#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define LEN 100

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

static char* which (const char* cmd) {
	const char* pathEnv = getenv("PATH");
	size_t pathEnvLen = strlen(pathEnv);
	size_t pathEndPtr = 0, pathBeginPtr = 0;
	char* ret = malloc(LEN);
	while (pathEndPtr < pathEnvLen) {
		for (; pathEndPtr < pathEnvLen; pathEndPtr++) {
			if (pathEnv[pathEndPtr] == ':') break;
		}
		strncpy(ret, pathEnv + pathBeginPtr, pathEndPtr - pathBeginPtr);
		ret[pathEndPtr - pathBeginPtr] = 0;
		DIR* pathDir = opendir(ret);
		if (!pathDir) {
			/*
			if (errno == EACCES) printf("permission denied for directory %s\n", ret);
			if (errno == EMFILE) printf("too many directories open\n");
			if (errno == ENOENT || errno == ENOTDIR) printf("%s: file not found or is not a directory\n", ret);
			if (errno == ENOMEM) printf("OOM\n");
			printf("WARNING: if %s exists in %s, it will not be found\n", cmd, ret);
			*/
			pathEndPtr++;
			pathBeginPtr = pathEndPtr;
			continue;
		}
		for (struct dirent* temp; temp = readdir(pathDir);) {
			if (!strcmp(temp->d_name, cmd)) {
				strcat(ret, "/");
				strcat(ret, cmd);
				strcat(ret, "\n");
				return ret;
			}
		}
		closedir(pathDir);
		pathEndPtr++;
		pathBeginPtr = pathEndPtr;
	}
	free(ret);
	return 0;
}

static void type (char* input, char* cmd, int cmdLen) {
	size_t typeLen;
	#define CMD_COUNT 3
	char builtins[CMD_COUNT][8] = {"exit", "echo", "type"};
	for (int i = 0; i < CMD_COUNT; i++) {
		if (!strcmp(input + cmdLen, builtins[i])) {
			printf("%s is a shell builtin\n", builtins[i]);
			return;
		}
	}
	for (typeLen = 0; cmdLen < LEN; cmdLen++) {
		if (isValidChar(input[cmdLen])) typeLen++;
		else break;
	}
	cmdLen = strlen(cmd);
	memset(cmd, 0, cmdLen);
	strncpy(cmd, input + cmdLen, typeLen);
	if (!cmd[0]) return; // error: no argument given
	char* cmdLocation = which(cmd);
	if (cmdLocation) {
		printf("%s is %s", cmd, cmdLocation);
		free(cmdLocation);
		return;
	}
	printf("%s: not found\n", cmd);
	return;
	#undef CMD_COUNT
}

int main () {
	while (1) {
		printf("$ ");
		fflush(stdout);
		char input[LEN];
		fgets(input, LEN, stdin);

		if (!strcmp(input, "exit\n")) return 0;
		char cmd[LEN] = "exit ";
		size_t cmdLen = strlen(cmd);
		if (!strncmp(input, cmd, cmdLen)) {
			int ret = atoi(input + cmdLen);
			return ret;
		}
		if (!strcmp(input, "echo\n")) continue;
		strncpy(cmd, "echo ", LEN);
		cmdLen = strlen(cmd);
		if (!strncmp(input, cmd, cmdLen)) {
			printf("%s", input + cmdLen);
			continue;
		}
		if (!strcmp(input, "type\n")) continue;
		strncpy(cmd, "type ", LEN);
		cmdLen = strlen(cmd);
		if (!strncmp(input, cmd, cmdLen)) {
			type(input, cmd, cmdLen);
			continue;
		}
		for (cmdLen = 0; cmdLen < LEN; cmdLen++) {
			if (isValidChar(input[cmdLen]));
			else break;
		}
		strncpy(cmd, input, cmdLen);
		cmd[cmdLen] = 0;
		printf("%s: command not found\n", cmd);
	}

	return 0;
}
