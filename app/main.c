#include <stdio.h>

int main() {
	// Uncomment this block to pass the first stage
	printf("$ ");
	fflush(stdout);

	// Wait for user input
	char input[100];
	fgets(input, 100, stdin);

	for (int i = 0; i != ' ' && i != NULL && i < 100; i++) {
		print("%c", input[i]);
	}
	print(": command not found\n");

	return 0;
}
