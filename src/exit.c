#include "../inc/minishell.h"

extern int exit_status;

void	panic(char *error, int status)
{
	perror(error);
	exit_status = status;
	exit(status);
}

int	safe_exit(int status)
{
	exit_status = status;
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

int	exec_exit(struct execcmd *ecmd)
{
	if (ecmd->argv[2])
	{
		fprintf(stderr, "exit: too many arguments\n");
		exit(127);
	}
	if (ecmd->argv[1] == NULL)
		exit(0);
	if (!check_num(ecmd->argv[1]))
	{
		fprintf(stderr, "exit: numeric argument required\n");
		exit(2);
	}
	exit_status = ft_atoi(ecmd->argv[1]) % 256;
	exit(exit_status);
}