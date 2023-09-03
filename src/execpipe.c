#include "../inc/minishell.h"

extern int exit_status;

void child_pipe(int pipefd[2], struct pipecmd *pcmd)
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	exec_tree(pcmd->left);
}

// Auxiliary function for executing a pipe command
void	exec_pipe_command(struct pipecmd *pcmd)
{
	int		pipefd[2];
	pid_t	cpid;
	int		status;

	if (pipe(pipefd) == -1)
		panic("pipe", EXIT_FAILURE);
	cpid = fork();
	if (cpid < 0)
		panic("fork", EXIT_FAILURE);
	if (cpid == 0)
		child_pipe(pipefd, pcmd);
	else
	{
		waitpid(cpid, &status, 0);
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
		else
			printf("Child process did not exit normally\n");
		exit_status = exit_status % 256;
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		exec_tree(pcmd->right);
	}
}

void	exec_pipe(struct pipecmd *pcmd)
{
	if (!pcmd->right)
		exec_tree(pcmd->left);
	else 
		exec_pipe_command(pcmd);
}

int	builtin_pipe(struct cmd *cmd)
{
	struct pipecmd	*pcmd;

	pcmd = (struct pipecmd *)cmd;
	if (pcmd->right)
		return (0);
	return (is_builtin_tree(pcmd->left));
}