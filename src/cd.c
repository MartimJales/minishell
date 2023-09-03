#include "../inc/minishell.h"

extern int exit_status;

int exec_cd(struct execcmd *ecmd)
{
	if (ecmd->argv[2])
	{
		exit_status = 1;
		fprintf(stderr, "cd: too many arguments\n");
		return (1);
	}
	if (ecmd->argv[1] == NULL)
	{
		exit_status = 1;
		fprintf(stderr, "cd: missing argument\n");
		return (1);
	}
	if (chdir(ecmd->argv[1]) != 0)
	{
		exit_status = 1;
		perror("cd");
	}
	return (0);
}