#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
// #include <error.h>

#define LEN 100

const char* validChars = "abcdefghijklmnopqrstuvwxyz"
						 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
						 "0123456789-_";

static char* which (const char* cmd) {
	const char* pathEnv = getenv("PATH");
	size_t pathEnvLen = strlen(pathEnv);
	size_t pathEndPtr = 0, pathBeginPtr = 0;
	char* ret = malloc(LEN);
	if (!ret) {
		perror("malloc failed: in function which");
		exit(1);
	}
	while (pathEndPtr < pathEnvLen) {
		pathEndPtr += strcspn(pathEnv + pathBeginPtr, ":");
		strncpy(ret, pathEnv + pathBeginPtr, pathEndPtr - pathBeginPtr);
		ret[pathEndPtr - pathBeginPtr] = 0;
		DIR* pathDir = opendir(ret);
		if (!pathDir) {
			/* debug printing
			if (errno == EACCES) printf("permission denied for directory %s\n", ret);
			if (errno == EMFILE) printf("too many directories open\n");
			if (errno == ENOENT || errno == ENOTDIR) printf("%s: directory not found or is not a directory\n", ret);
			if (errno == ENOMEM) printf("OOM\n");
			printf("WARNING: if %s exists in %s, it will not be found\n", cmd, ret);
			*/
			pathEndPtr++;
			pathBeginPtr = pathEndPtr;
			continue;
		}
		for (struct dirent* temp; (temp = readdir(pathDir));) {
			if (!strcmp(temp->d_name, cmd)) {
				closedir(pathDir);
				strcat(ret, "/");
				strcat(ret, cmd);
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

static void type (const char* input, char* cmd, int cmdLen) {
	#define CMD_COUNT 4
	char builtins[CMD_COUNT][8] = {"exit", "echo", "type", "pwd"};
	for (int i = 0; i < CMD_COUNT; i++) {
		if (!strncmp(input + cmdLen, builtins[i], strlen(builtins[i]))) {
			printf("%s is a shell builtin\n", builtins[i]);
			return;
		}
	}
	size_t typeLen = strspn(input + cmdLen, validChars);
	strncpy(cmd, input + cmdLen, typeLen);
	cmd[typeLen] = 0;
	if (!cmd[0]) return; // error: no argument given
	char* cmdLocation = which(cmd);
	if (cmdLocation) {
		printf("%s is %s\n", cmd, cmdLocation);
		free(cmdLocation);
		return;
	}
	printf("%s: not found\n", cmd);
	#undef CMD_COUNT
}

static void execute (char* input, const char* prog) {
	char* argv[LEN];
	int i = 0;
	for (char* t = (char*)1; t && i < LEN; i++) {
		t = strsep((char**)&input, " \n");
		argv[i] = t;
	}
	if (i > 1) argv[i - 2] = (char*)0;
	pid_t child = fork();
	waitpid(child, NULL, 0);
	if (!child) execve(prog, argv, (char**)0);
}

int main () {
	while (1) {
		printf("$ ");
		fflush(stdout);
		char input[LEN];
		fgets(input, LEN, stdin);

		if (!strcmp(input, "exit\n")) return 0;
		char cmd[LEN] = {"exit "};
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
		char pwd[LEN];
		getcwd(pwd, sizeof(pwd));
		if (!strcmp(input, "pwd\n")) {
			printf("%s\n", pwd);
			continue;
		}
		cmdLen = strspn(input, validChars);
		strncpy(cmd, input, cmdLen);
		cmd[cmdLen] = 0;
		char* prog = which(cmd);
		if (!prog) {
			printf("%s: command not found\n", cmd);
			free(prog);
			continue;
		}
		execute(input, prog);
		free(prog);
	}

	return 0;
}
