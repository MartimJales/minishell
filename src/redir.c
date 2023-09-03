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

	if (rcmd->mode == HEREDOC) 
		heredoc(rcmd->file);
	else
	{
		close(rcmd->fd);
		if (open(rcmd->file, rcmd->mode, 0664) < 0)
			exit_status = EXIT_FAILURE;
	}
	exec_tree(rcmd->cmd);
}