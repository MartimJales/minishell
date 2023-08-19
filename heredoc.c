#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    const char *delimiter = "ola\n";
    char inputBuffer[256];  // Buffer to store user input

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Error creating pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("Error forking");
        return 1;
    } else if (pid == 0) {
        // Child process
        close(pipefd[1]);  // Close write end of the pipe

        // Redirect pipe read end to stdin
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);  // Close the pipe read end

        // Run the cat command (or other desired command)
        execlp("cat", "cat", NULL);

        // If execlp returns (which indicates an error), print an error message
        perror("Error executing cat");
        return 1;
    } else {
        // Parent process
        close(pipefd[0]);  // Close read end of the pipe

        while (1) {
            // Prompt the user for input
            printf("Enter input matching the delimiter \"%s\":\n", delimiter);
            fflush(stdout);

            // Read user input
            fgets(inputBuffer, sizeof(inputBuffer), stdin);

            // Check if input matches the delimiter
            printf("buff = {%s}\n", inputBuffer);
            if (strcmp(inputBuffer, delimiter) == 0) {
                printf("caiu\n");
                break; // Break out of the loop if the delimiter is matched
            }

            // Write the input to the pipe
            write(pipefd[1], inputBuffer, strlen(inputBuffer));
        }

        close(pipefd[1]);  // Close the write end of the pipe

        // Wait for the child process to complete
        waitpid(pid, NULL, 0);
    }

    return 0;
}