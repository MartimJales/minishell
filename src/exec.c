/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 00:16:50 by mjales            #+#    #+#             */
/*   Updated: 2023/09/10 16:37:53 by mjales           ###   ########.fr       */
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
	if (ft_strcmp("export", ecmd->argv[0]) == 0)
		return (exec_export(ecmd));
	if (ft_strcmp("env", ecmd->argv[0]) == 0)
		return (exec_env(0));
	if (ft_strcmp("unset", ecmd->argv[0]) == 0)
		return (exec_unset(ecmd));
	if (ft_strcmp("cd", ecmd->argv[0]) == 0)
		return (exec_cd(ecmd));
	if (ft_strcmp("exit", ecmd->argv[0]) == 0)
		return (exec_exit(ecmd));
	return (-1);
}

int	isdirectory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
	{
		perror("stat");
		return (0);
	}

	if (S_ISREG(path_stat.st_mode))
	{
		return (0);
	}
	else if (S_ISDIR(path_stat.st_mode))
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

void	exec_single_command(struct s_execcmd *ecmd)
{
	if (!ecmd->argv[0])
		exit(1);
	if (exec_builtin(ecmd) == -1)
	{
		execve(ecmd->argv[0], ecmd->argv, vars()->envp);
		if (isdirectory(ecmd->argv[0]))
		{
			g_exit_status = 126;
			write(2, " Is a directory\n", ft_strlen(" Is a directory\n"));
			exit(g_exit_status);
		}
		g_exit_status = 127;
		write(2, ecmd->argv[0], ft_strlen(ecmd->argv[0]));
		write(2, ": command not found\n", 20);
		free(vars()->s);
		free_cmd((struct s_cmd *)ecmd);
		exit(g_exit_status);
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
	{
		write(2, "Unknown cmd type: ", 18);
		write(2, ft_itoa(root->type), ft_strlen(ft_itoa(root->type)));
		write(2, "\n", 1);
	}
}

void	execute_child(struct s_pipecmd *pcmd, int pipefd[])
{
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	exec_tree(pcmd->left);
}
