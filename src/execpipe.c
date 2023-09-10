/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execpipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcordovi <dcordovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 23:04:38 by mjales            #+#    #+#             */
/*   Updated: 2023/09/10 20:12:11 by dcordovi         ###   ########.fr       */
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

// void	execute_parent(struct s_pipecmd *pcmd, int pipefd[])
// {
// 	pid_t	cpid;
// 	pid_t	cpid2;
// 	int		status;

// 	cpid = fork();
// 	if (cpid < 0)
// 		panic("fork", EXIT_FAILURE);
// 	if (cpid == 0)
// 		execute_child(pcmd, pipefd);
// 	cpid2 = fork();
// 	vars()->forked = 1;
// 	if (cpid2 < 0)
// 		panic("fork", EXIT_FAILURE);
// 	if (cpid2 == 0)
// 	{
// 		dup2(pipefd[0], STDIN_FILENO);
// 		close(pipefd[0]);
// 		close(pipefd[1]);
// 		exec_tree(pcmd->right);
// 	}
// 	close(pipefd[0]);
// 	close(pipefd[1]);
// 	waitpid(cpid, &status, 0);
// 	if (WIFEXITED(status))
// 		g_exit_status = WEXITSTATUS(status);
// 	else
// 		exit(1);
// 	waitpid(cpid2, &status, 0);
// 	if (WIFEXITED(status))
// 		g_exit_status = WEXITSTATUS(status);
// 	else
// 		exit(1);
// 	g_exit_status = g_exit_status % 256;
// }

pid_t	create_first_child(struct s_pipecmd *pcmd, int pipefd[])
{
	pid_t	cpid;

	cpid = fork();
	if (cpid < 0)
		panic("fork", EXIT_FAILURE);
	if (cpid == 0)
		execute_child(pcmd, pipefd);
	return (cpid);
}

pid_t	create_second_child(struct s_pipecmd *pcmd, int pipefd[])
{
	pid_t	cpid2;

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
	return (cpid2);
}

void	update_exit_status(pid_t cpid)
{
	int	status;

	waitpid(cpid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else
		exit(1);
}

void	execute_parent(struct s_pipecmd *pcmd, int pipefd[])
{
	pid_t	cpid;
	pid_t	cpid2;

	cpid = create_first_child(pcmd, pipefd);
	cpid2 = create_second_child(pcmd, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	update_exit_status(cpid);
	update_exit_status(cpid2);
	g_exit_status = g_exit_status % 256;
}
