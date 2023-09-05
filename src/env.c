/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:29:14 by mjales            #+#    #+#             */
/*   Updated: 2023/09/05 09:29:34 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern int exit_status;

int	exec_env(int declare)
{
	int	i;

	i = 0;
	while (vars()->envp[i] != NULL)
	{
		if (declare)
			printf("declare -x %s=\"%s\"\n", \
ft_split(vars()->envp[i], '=')[0], strchr(vars()->envp[i], '=') + 1);
		else
			printf("%s\n", vars()->envp[i]);
		i++;
	}
	exit_status = 0;
	if (vars()->forked)
		exit(0);
	return (0);
}