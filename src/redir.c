#include "../inc/minishell.h"

extern int exit_status;

int	ft_redir_signal(char *s)
{
	if (strncmp(">>", s, 2) == 0)
		return (APPEND);
	if (strncmp("<<", s, 2) == 0)
		return (HEREDOC);
	if (*s == '>')
		return (OUT);
	if (*s == '<')
		return (IN);
	return (0); 
}

int	redir_mode(int redir_signal)
{
	if (redir_signal == IN)
		return (O_RDONLY);
	if (redir_signal == OUT)
		return (O_WRONLY | O_CREAT | O_TRUNC);
	if (redir_signal == APPEND)
		return (O_WRONLY | O_CREAT);
	if (redir_signal == HEREDOC)
		return (HEREDOC);
	return (0);
}

void	exec_redir(struct redircmd *rcmd)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("Error creating pipe");
		exit_status = EXIT_FAILURE;
		return ;
	}
	close(rcmd->fd);
	if (rcmd->mode == HEREDOC) 
	{
		heredoc(rcmd->file, pipefd[0]);
	}
	else if (open(rcmd->file, rcmd->mode, 0664) < 0)
	{
		exit_status = EXIT_FAILURE;
	}
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	exec_tree(rcmd->cmd);
}