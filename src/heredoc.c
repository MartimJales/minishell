/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 23:05:01 by mjales            #+#    #+#             */
/*   Updated: 2023/09/04 23:05:02 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern int exit_status;

void heredoc(const char *delimiter)
{
    char *inputBuffer = NULL;      // Buffer to store user input
    char *multilineInput = NULL;  // Buffer to store multiline input

    if (pipe(vars()->pipefd) == -1)
    {
        perror("Error creating pipe");
        exit(EXIT_FAILURE); // Exit gracefully on error
    }

    while (1)
    {
        inputBuffer = readline(">");
        if (inputBuffer == NULL)
            break;
		printf("%s == %s\n", inputBuffer, delimiter);
        if (strcmp(inputBuffer, delimiter) == 0)
        {
            if (multilineInput)
            {
                write(vars()->pipefd[1], multilineInput, strlen(multilineInput));
                free(multilineInput);
                multilineInput = NULL;
            }
            free(inputBuffer);
            break;
        }

        if (multilineInput)
        {
            char *temp = realloc(multilineInput, strlen(multilineInput) + strlen(inputBuffer) + 2);
            if (temp == NULL)
            {
                perror("Error reallocating memory");
                free(inputBuffer);
                break;
            }
            multilineInput = temp;
			multilineInput = junta_strings(multilineInput, inputBuffer);
			multilineInput = junta_strings(multilineInput, "\n");
            // strcat(multilineInput, inputBuffer);
            // strcat(multilineInput, "\n");
        }
        else
        {
            multilineInput = strdup(inputBuffer);
            if (multilineInput == NULL)
            {
                perror("Error allocating memory");
                free(inputBuffer);
                break;
            }
            multilineInput = junta_strings(multilineInput, "\n");// strcat(multilineInput, "\n");
        }
        add_history(inputBuffer);
        free(inputBuffer);
    }

    close(vars()->pipefd[1]); // Close the write end of the pipe when done
    dup2(vars()->pipefd[0], STDIN_FILENO);
    close(vars()->pipefd[0]); // Close the read end of the pipe when done
}
