#include "../inc/minishell.h"

extern int exit_status;

void heredoc(const char *delimiter, int pipe_read_fd) {
    char *inputBuffer = NULL;  // Buffer to store user input
    char *multilineInput = NULL; // Buffer to store multiline input

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
                // Send the multiline input to the pipe
                write(pipe_read_fd, multilineInput, strlen(multilineInput));
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
}