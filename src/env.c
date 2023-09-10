/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcordovi <dcordovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 09:29:14 by mjales            #+#    #+#             */
/*   Updated: 2023/09/10 20:13:00 by dcordovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	exec_env(int declare)
{
	int	i;

	i = 0;
	while (vars()->envp[i] != NULL)
	{
		if (declare)
			printf("declare -x %s=\"%s\"\n", \
ft_split(vars()->envp[i], '=')[0], ft_strchr(vars()->envp[i], '=') + 1);
		else
			printf("%s\n", vars()->envp[i]);
		i++;
	}
	g_exit_status = 0;
	if (vars()->forked)
		exit(0);
	return (0);
}

void	exec_pipe_command(struct s_pipecmd *pcmd)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		panic("pipe", EXIT_FAILURE);
	execute_parent(pcmd, pipefd);
	exit(g_exit_status);
}

void	exec_pipe(struct s_pipecmd *pcmd)
{
	if (!pcmd->right)
		exec_tree(pcmd->left);
	else
		exec_pipe_command(pcmd);
}

int	builtin_pipe(struct s_cmd *cmd)
{
	struct s_pipecmd	*pcmd;

	pcmd = (struct s_pipecmd *)cmd;
	if (pcmd->right)
		return (0);
	return (is_builtin_tree(pcmd->left));
}

void	execute_child(struct s_pipecmd *pcmd, int pipefd[])
{
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	exec_tree(pcmd->left);
}
