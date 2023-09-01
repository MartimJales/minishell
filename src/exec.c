/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcordovi <dcordovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 00:16:50 by mjales            #+#    #+#             */
/*   Updated: 2023/08/31 13:35:30 by dcordovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern	int	exit_status;

void	add_variable_to_envp(char *new_var)
{
	int		i;
	int		new_envp_size;
	char	**envp;
	char	**new_envp;

	new_envp_size = 0;
	envp = vars()->envp;
	while (envp[new_envp_size] != NULL)
		new_envp_size++;
	new_envp = (char **)malloc((new_envp_size + 2) * sizeof(char *));
	if (!new_envp)
	{
		perror("malloc");
		exit_status = 1;
		exit(1);
	}

	i = 0;
	while (i < new_envp_size)
	{
		new_envp[i] = strdup(envp[i]);
		if (!new_envp[i])
		{
			perror("strdup");
			exit_status = 1;
			exit(1);
		}
		i++;
	}
	new_envp[new_envp_size] = strdup(new_var);
	new_envp[new_envp_size + 1] = NULL;
	i = 0;
	while (new_envp[i] != NULL)
		i++;
	vars()->envp = new_envp;
}

int	validate_format(const char *input) {
	const char	*equal_sign;
	size_t		var_length;
	int			i;
	char		*var;

	equal_sign = strchr(input, '=');

	if (equal_sign == NULL || equal_sign == input)
	{
		return (0);
	}

	var_length = equal_sign - input;
	var = malloc(var_length + 1);
	strncpy(var, input, var_length);
	var[var_length] = '\0';

	i = 0;
	while (var[i])
	{
		if (!ft_isalnum(var[i]))
			return (0);
		i++;
	}
	return (1);
}


int	check_alnum(const char *input_string)
{
	if (!input_string || (input_string[0] >= '0' && input_string[0] <= '9'))
		return (0);
	while (*input_string)
	{
		if (!ft_isalnum(*input_string))
		{
			return (0);
		}
		input_string++;
	}
	return (1);
}

int	check_num(const char *input_string)
{
	int	i;

	if (!input_string)
		return (0);


	i = -1;
	while (input_string[++i])
	{
		if (((input_string[i] == '+') || (input_string[i] == '-')) && i == 0)
			continue ;
		if (!(input_string[i] >= '0' && input_string[i] <= '9'))
		{
			return (0);
		}
	}
	return (1);
}


int	var_exists(char **envp, char *var)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (strncmp(envp[i], var, strlen(var)) == 0 && \
envp[i][strlen(var)] == '=')
			return (1);
		i++;
	}
	return (0);
}

void	update_var_to_envp(char *var, char *new_value)
{
	char	**envp;
	int		index;

	index = 0;
	envp = vars()->envp;
	while (envp[index] != NULL)
	{
		if (strncmp(envp[index], var, strlen(var)) == 0 &&
			envp[index][strlen(var)] == '=')
		{
			free(envp[index]);
			envp[index] = (char *)malloc(strlen(new_value));
			if (!envp[index])
			{
				perror("malloc");
				exit(1);
			}
			envp[index] = strdup(new_value);
			break ;
		}
		index++;
	}
}

void delete_var_from_envp(char *var) {
	char	**envp;
	int		index;
	int		i;

	index = 0;
	envp = vars()->envp;

	while (envp[index] != NULL)
	{
		if (strncmp(envp[index], var, strlen(var)) == 0 &&
			envp[index][strlen(var)] == '=')
		{
			free(envp[index]);

			i = index;
			while (envp[i] != NULL)
			{
				envp[i] = envp[i + 1];
				i++;
			}
			break ;
		}
		index++;
	}
}



int exec_export(struct execcmd *ecmd)
{
	int i = 1;
	if (ecmd->argv[1] == NULL)
		return exec_env(1);
	while (ecmd->argv[i] != NULL)
	{
		char *var = ft_split(ecmd->argv[i], '=')[0];
		if (!check_alnum(var))
		{
			write(2, " not a valid identifier\n", ft_strlen(" not a valid identifier\n"));
			exit_status = 1;
			if (vars()->forked){
				exit(1);
			}
			return 1;
		}
		if (validate_format(ecmd->argv[i]))
		{
			if (var_exists(vars()->envp, var))
				update_var_to_envp(var, ecmd->argv[i]);
			else
				add_variable_to_envp(ecmd->argv[i]);
		}
		i++;
	}
	exit_status = 0;
	if (vars()->forked){
		exit(0);
	}
	return (0);
}

int exec_env(int declare)
{
	int	i;

	i = 0;
	while (vars()->envp[i] != NULL)
	{
		if (declare)
			printf("declare -x %s=\"%s\"\n", \
ft_split(vars()->envp[i], '=')[0], strchr(vars()->envp[i], '=') + 1);
		else
			printf("%s\n", vars()->envp[i]);
		i++;
	}
	exit_status = 0;
	if (vars()->forked)
		exit(0);
	return (0);
}

int exec_unset(struct execcmd *ecmd)
{
	int	i;

	i = 1;
	while (ecmd->argv[i] != NULL)
	{
		if (var_exists(vars()->envp, ecmd->argv[i]))
		{
			delete_var_from_envp(ecmd->argv[i]);
		}
		i++;
	}
	exit_status = 0;
	if (vars()->forked)
	{
		exit(0);
	}
	return (0);
}

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

unsigned char modulo256(const char *numberStr)
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



int exec_exit(struct execcmd *ecmd)
{
	if (ecmd->argv[2]) {
        fprintf(stderr, "exit: too many arguments\n");
        exit(127);
    }
    if (ecmd->argv[1] == NULL)
        exit(0);
    if (!check_num(ecmd->argv[1])){
        fprintf(stderr, "exit: numeric argument required\n");
        exit(2);
    }
    // printf("%lld\n", ft_atoi(ecmd->argv[1]));
    exit_status = ft_atoi(ecmd->argv[1]) % 256;
    // exit_status = modulo256(ecmd->argv[1]);
    exit(exit_status);
}

int exec_builtin(struct execcmd *ecmd)
{
    if (strcmp("export", ecmd->argv[0]) == 0)
        return exec_export(ecmd);
    if (strcmp("env", ecmd->argv[0]) == 0)
        return exec_env(0);
    if (strcmp("unset", ecmd->argv[0]) == 0)
        return exec_unset(ecmd);
    if (strcmp("cd", ecmd->argv[0]) == 0)
        return exec_cd(ecmd);
    if (strcmp("exit", ecmd->argv[0]) == 0)
        return exec_exit(ecmd);

    return -1;
}

void exec_single_command(struct execcmd *ecmd) {
    if (!ecmd->argv[0])
        exit(1);  // Case when no command is received
    if (exec_builtin(ecmd) == -1) {
        // for (int i = 0; ecmd->argv[i]; i++)
        //     printf("argv[%d] = {%s}\n", i, ecmd->argv[i]);
        execve(ecmd->argv[0], ecmd->argv, vars()->envp);
        fprintf(stderr, "exec %s failed\n", ecmd->argv[0]);
        exit(EXIT_FAILURE);
    }
}

void exec_redir(struct redircmd *rcmd) {
    close(rcmd->fd);
    if (open(rcmd->file, rcmd->mode, 0664) < 0) {
        // perror("open");
        // fprintf(stderr, "open %s failed\n", rcmd->file);
        exit_status = EXIT_FAILURE;
    }
    exec_tree(rcmd->cmd);
}

// Auxiliary function for executing a pipe command
void exec_pipe_command(struct pipecmd *pcmd) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    // printf("before pipe fork = %d\n", exit_status);
    pid_t cpid = fork();
    if (cpid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        exec_tree(pcmd->left);
    } else 
    {
        int status;
        waitpid(cpid, &status, 0);
        if (WIFEXITED(status)) {
            exit_status = WEXITSTATUS(status);
        } else {
            printf("Child process did not exit normally\n");
        }
        exit_status = exit_status % 256;
        // printf("exit_status = %d\n", status);
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        // debug_tree(pcmd->right);
        exec_tree(pcmd->right);
    }
}

void exec_tree(struct cmd *root) {
    if (!root) return;

    switch (root->type) {
        case EXEC:
            exec_single_command((struct execcmd *)root);
            break;
        case REDIR:
            exec_redir((struct redircmd *)root);
            break;
        case PIPE:
            {
                struct pipecmd *pcmd = (struct pipecmd *)root;
                if (!pcmd->right) {
                    exec_tree(pcmd->left);
                } else {
                    exec_pipe_command(pcmd);
                }
            }
            break;
        default:
            fprintf(stderr, "Unknown cmd type: %d\n", root->type);
            break;
    }
}
