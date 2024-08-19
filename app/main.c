#include <stdio.h>

int main() {
	// Uncomment this block to pass the first stage
	printf("$ ");
	fflush(stdout);

	// Wait for user input
	char input[100];
	fgets(input, 100, stdin);

	for (int i = 0; i != ' ' && i != '\0' && i < 100; i++) {
		printf("%c", input[i]);
	}
	printf(": command not found\n");

	return 0;
}
