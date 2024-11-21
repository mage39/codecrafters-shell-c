#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
// #include <error.h>

#define LEN 100

static const char* validChars = "abcdefghijklmnopqrstuvwxyz"
								"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
								"0123456789-_";

static char* which (const char* cmd) {
	const char* pathEnv = getenv("PATH");
	char* freePath = strdup(pathEnv), *path = freePath;
	char* ret = malloc(LEN);
	if (!ret) {
		perror("malloc failed: in function which");
		exit(1);
	}
	for (char* tok; (tok = strsep(&path, ":"));) {
		DIR* pathDir = opendir(tok);
		if (!pathDir) continue;
		for (struct dirent* t; (t = readdir(pathDir));) {
			if (!strcmp(t->d_name, cmd)) {
				closedir(pathDir);
				snprintf(ret, LEN, "%s/%s", tok, cmd);
				free(freePath);
				return ret;
			}
		}
		closedir(pathDir);
	}
	free(freePath);
	free(ret);
	return 0;
}

static void type (const char* input, char* cmd, size_t cmdLen) {
	#define CMD_COUNT 5
	char builtins[CMD_COUNT][8] = {"exit", "echo", "type", "pwd", "cd"};
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

static void cd (const char* input, size_t cmdLen) {
	size_t start = strspn(input + cmdLen, " ");
	size_t end = strcspn(input + cmdLen, "\n");
	char* path = strndup(input + cmdLen + start, end - start);
	if (!path) {
		perror("malloc failed: in function main");
		exit(1);
	}
	if (strchr(path, "~")) {
		char* home = getenv("HOME");
		size_t homePathLen = strlen(path) + strlen(home) + 1;
		char* homePath = malloc(homePathLen);
		if (!homePath)
			perror("malloc failed: in function main");
			exit(1);
		}
		snprintf(homePath, homePathLen, "%s/%s", home, path);
		if(chdir(homePath)) printf("cd: %s: No such file or directory\n", path);
		free(homePath);
		free(path);
		return;
	}
	if (chdir(path)) printf("cd: %s: No such file or directory\n", path);
	free(path);
}

static void execute (char* input, const char* prog) {
	char* argv[LEN];
	int i = 0;
	for (char* t; (t = strsep(&input, " \n")) && i < LEN; i++) argv[i] = t;
	if (i) argv[i - 1] = (char*)0;
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
		if (!strcmp(input, "echo\n")) {
			printf("\n");
			continue;
		}
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
		if (!strcmp(input, "pwd\n")) {
			char pwd[LEN];
			getcwd(pwd, sizeof(pwd));
			printf("%s\n", pwd);
			continue;
		}
		if (!strcmp(input, "cd\n")) {
			chdir(getenv("HOME"));
			continue;
		}
		strncpy(cmd, "cd ", LEN);
		cmdLen = strlen(cmd);
		if (!strncmp(input, cmd, cmdLen)) {
			cd(input, cmdLen);
			continue;
		}
		cmdLen = strspn(input, validChars);
		strncpy(cmd, input, cmdLen);
		cmd[cmdLen] = 0;
		char* prog = which(cmd);
		if (!prog) {
			printf("%s: command not found\n", cmd);
			continue;
		}
		execute(input, prog);
		free(prog);
	}

	return 0;
}
