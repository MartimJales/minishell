/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 00:16:50 by mjales            #+#    #+#             */
/*   Updated: 2023/09/06 02:40:48 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	builtin_exec(struct s_cmd *cmd)
{
	struct s_execcmd	*ecmd;
	char				*command;

	ecmd = (struct s_execcmd *)cmd;
	command = ecmd->argv[0];
	if (command)
	{
		if (is_builtin(command))
			return (1);
		else
			return (0);
	}
	return (-1);
}

int	exec_builtin(struct s_execcmd *ecmd)
{
	if (strcmp("export", ecmd->argv[0]) == 0)
		return (exec_export(ecmd));
	if (strcmp("env", ecmd->argv[0]) == 0)
		return (exec_env(0));
	if (strcmp("unset", ecmd->argv[0]) == 0)
		return (exec_unset(ecmd));
	if (strcmp("cd", ecmd->argv[0]) == 0)
		return (exec_cd(ecmd));
	if (strcmp("exit", ecmd->argv[0]) == 0)
		return (exec_exit(ecmd));
	return (-1);
}

void	exec_single_command(struct s_execcmd *ecmd)
{
	if (!ecmd->argv[0])
		exit(1);
	if (exec_builtin(ecmd) == -1)
	{
		execve(ecmd->argv[0], ecmd->argv, vars()->envp);
		g_exit_status = 127;
		fprintf(stderr, "%s: command not found\n", ecmd->argv[0]);
		exit(127);
	}
}

void	exec_tree(struct s_cmd *root)
{
	if (!root)
		return ;
	if (root->type == EXEC)
		exec_single_command((struct s_execcmd *)root);
	else if (root->type == REDIR)
		exec_redir((struct s_redircmd *)root);
	else if (root->type == PIPE)
		exec_pipe((struct s_pipecmd *)root);
	else
		fprintf(stderr, "Unknown cmd type: %d\n", root->type);
}
