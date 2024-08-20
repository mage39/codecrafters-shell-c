#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdio.h>

int main() {
	// Uncomment this block to pass the first stage
	printf("$ ");
	fflush(stdout);

	// Wait for user input
	char input[100];
	fgets(input, 100, stdin);

	regex_t regex;
	regmatch_t offsets = {0};
	int err = regcomp(&regex, "^.[[:alnum:]]+", REG_NEWLINE | REG_EXTENDED);
	if (err) {
		printf("excweeze me?\n");
		exit(1);
	}
	err = regexec(&regex, input, 0, &offsets, 0);
	char output[100] = {0};
	if (!err) {
		size_t len = offsets.rm_eo - offsets.rm_so; // end offset not calculated at true end of match
		strncpy(output, input + offsets.rm_so, len);
	}
	else if (err == REG_NOMATCH) printf("no match, WTF\n");
	else {
		char buff[100];
		regerror(err, &regex, buff, sizeof(buff));
		printf("regex failed %s\n", buff);
		exit(1);
	}
	printf("%s: command not found\n", output);

	regfree(&regex);
	return 0;
}
