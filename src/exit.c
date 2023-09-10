/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 23:04:45 by mjales            #+#    #+#             */
/*   Updated: 2023/09/10 17:57:16 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	panic(char *error, int status)
{
	perror(error);
	g_exit_status = status;
	exit(status);
}

int	safe_exit(int status)
{
	g_exit_status = status;
	if (vars()->forked)
		exit(status);
	return (status);
}

unsigned char	modulo256(const char *numberStr)
{
	int	result;
	int	start;
	int	i;

	result = 0;
	start = 0;
	if (numberStr[0] == '-' || numberStr[0] == '+')
		start = 1;
	i = start;
	while (numberStr[i] != '\0')
	{
		result = (result * 10 + (numberStr[i] - '0')) % 256;
		i++;
	}
	if (result < 0)
		result += 256;
	return ((unsigned char)result);
}

void	free_envp(char **envp)
{
	int	i;

	i = 0;
	if (envp == NULL)
		return ;
	while (envp[i] != NULL)
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

int	exec_exit(struct s_execcmd *ecmd)
{
	clear_history();
	if (ecmd->argv[1] && ecmd->argv[2])
	{
		write(2, "exit: too many arguments\n", 25);
		exit(127);
	}
	if (ecmd->argv[1] == NULL)
		exit(0);
	if (!check_num(ecmd->argv[1]))
	{
		write(2, "exit: numeric argument required\n", 32);
		exit(2);
	}
	g_exit_status = ft_atoi(ecmd->argv[1]) % 256;
	free(ecmd->argv[0]);
	if (ecmd->argv[1])
		free(ecmd->argv[1]);
	free(ecmd->argv);
	free(vars()->sc);
	free_envp(vars()->envp);
	exit(g_exit_status);
}
