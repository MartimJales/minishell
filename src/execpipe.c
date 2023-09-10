/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execpipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 23:04:38 by mjales            #+#    #+#             */
/*   Updated: 2023/09/10 15:24:05 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	child_pipe(int pipefd[2], struct s_pipecmd *pcmd)
{
	struct s_redircmd	*rcmd;

	rcmd = (struct s_redircmd *)pcmd->left;
	if (!(pcmd->left->type == REDIR && rcmd->mode == HEREDOC))
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
	}
	exec_tree(pcmd->left);
}

//void	exec_pipe_command(struct s_pipecmd *pcmd)
//{
//	int		pipefd[2];
//	pid_t	cpid;
//	int		status;

//	if (pipe(pipefd) == -1)
//		panic("pipe", EXIT_FAILURE);
//	cpid = fork();
//	if (cpid < 0)
//		panic("fork", EXIT_FAILURE);
//	if (cpid == 0)
//		child_pipe(pipefd, pcmd);
//	else
//	{
//		waitpid(cpid, &status, 0);
//		if (WIFEXITED(status))
//			g_exit_status = WEXITSTATUS(status);
//		else
//		{
//			printf("Child process did not exit normally\n");
//			exit(1);
//		}
//		g_exit_status = g_exit_status % 256;
//		close(pipefd[1]);
//		dup2(pipefd[0], STDIN_FILENO);
//		close(pipefd[0]);
//		exec_tree(pcmd->right);
//	}
//}

void	execute_parent(struct s_pipecmd *pcmd, int pipefd[])
{
	pid_t	cpid;
	pid_t	cpid2;
	int		status;

	cpid = fork();
	if (cpid < 0)
		panic("fork", EXIT_FAILURE);
	if (cpid == 0)
		execute_child(pcmd, pipefd);
	cpid2 = fork();
	vars()->forked = 1;
	if (cpid2 < 0)
		panic("fork", EXIT_FAILURE);
	if (cpid2 == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		exec_tree(pcmd->right);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(cpid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else
	{
		//printf("Child process did not exit normally\n");
		exit(1);
	}
	waitpid(cpid2, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else
	{
		//printf("Child process did not exit normally\n");
		exit(1);
	}
	g_exit_status = g_exit_status % 256;
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
