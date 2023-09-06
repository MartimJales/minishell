/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 14:08:21 by mjales            #+#    #+#             */
/*   Updated: 2023/09/06 02:18:18 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_export(char **result, char *str_aux, int *index_result)
{
	char	*export;

	export = NULL;
	if (strcmp(str_aux, "?") == 0)
		export = ft_itoa(g_exit_status);
	else
		export = exp_dollar(str_aux, vars()->envp);
	strcat(*result + *index_result, export);
	*index_result += strlen(export);
}

char	*create_str_aux(char *input, int i, int j)
{
	char	*str_aux;

	str_aux = malloc(j - i);
	strncpy(str_aux, input + i + 1, j - i - 1);
	str_aux[j - i - 1] = '\0';
	return (str_aux);
}

int	update_j(char *input, int j)
{
	while (ft_isalnum(input[j]))
		j++;
	if (input[j] == '?')
		j++;
	return (j);
}

char	*replace_dollar(char *input)
{
	size_t	length;
	char	*result;
	size_t	i;
	size_t	j;
	int		index_result;

	length = strlen(input);
	result = malloc(length * 10 + 1);
	result[0] = '\0';
	index_result = 0;
	i = -1;
	while (++i < length)
	{
		if (input[i] == '$')
		{
			j = update_j(input, i + 1);
			handle_export(&result, create_str_aux(input, i, j), &index_result);
			i = j - 1;
		}
		else
			result[index_result++] = input[i];
	}
	free(input);
	result[index_result] = '\0';
	return (result);
}
