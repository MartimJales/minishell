/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 14:08:21 by mjales            #+#    #+#             */
/*   Updated: 2023/09/05 14:35:23 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern int	exit_status;

char *replace_dollar(char *input, char **envp)
{
	size_t length = strlen(input);
	char *result = malloc(length * 10 + 1);
	result[0] = '\0';
	char *str_aux;
	size_t index_result = 0;

	for (size_t i = 0; i < length; i++) {
		if (input[i] == '$') {
			size_t start = i + 1;
			size_t j = start;
			while (ft_isalnum(input[j])) {
				j++;
			}
			if (input[j] == '?') {
				j++;
			}
			str_aux = malloc(j - start + 1);
			strncpy(str_aux, input + start, j - start);
			str_aux[j - start] = '\0';
			char *export = NULL;
			if (strcmp(str_aux, "?") == 0)
			{
				export = ft_itoa(exit_status);
			}
			else
			{
				export = exp_dollar(str_aux, envp);
			}

			strcat(result + index_result, export);
			index_result += strlen(export);
			i = j - 1;
		}
		else
		{
			result[index_result++] = input[i];
		}
	}
	free(input);
	result[index_result] = '\0';
	return result;
}






