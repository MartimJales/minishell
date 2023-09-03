#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

void heredoc(const char *delimiter) {
    char *inputBuffer = NULL;  // Buffer to store user input
    char *multilineInput = NULL; // Buffer to store multiline input

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Error creating pipe");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("Error forking");
        exit(1);
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
        exit(1);
    } else {
        // Parent process
        close(pipefd[0]);  // Close read end of the pipe

        while (1) {
            // Prompt the user for input
            inputBuffer = readline(">");
            if (inputBuffer == NULL) {
                // EOF or error reading input
                break;
            }

            // Check if input matches the delimiter
            if (strcmp(inputBuffer, delimiter) == 0) {
                if (multilineInput) {
                    // Send the multiline input
                    write(pipefd[1], multilineInput, strlen(multilineInput));
                    free(multilineInput);
                    multilineInput = NULL;
                }
                free(inputBuffer);
                break; // Break out of the loop if the delimiter is matched
            }

            // Append input to multiline buffer
            if (multilineInput) {
                multilineInput = realloc(multilineInput, strlen(multilineInput) + strlen(inputBuffer) + 1);
                strcat(multilineInput, inputBuffer);
				strcat(multilineInput, "\n");
            } else {
                multilineInput = strdup(inputBuffer);
				strcat(multilineInput, "\n");
            }

            // Add input to history
            add_history(inputBuffer);
            free(inputBuffer);
        }

        close(pipefd[1]);  // Close the write end of the pipe

        // Wait for the child process to complete
        waitpid(pid, NULL, 0);
    }
}

int main()
{
    const char *delimiter = "ola";
    heredoc(delimiter);
    return 0;
}



