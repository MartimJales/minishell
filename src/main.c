/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 10:17:18 by mjales            #+#    #+#             */
/*   Updated: 2023/09/03 14:09:00 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int exit_status;

int	is_builtin_tree(struct cmd *cmd)
{
	if (!cmd)
		return (0);
	if (cmd->type == EXEC)
		return (builtin_exec(cmd));
	else if (cmd->type == REDIR)
		return (is_builtin_tree(((struct redircmd *)cmd)->cmd));
	else if (cmd->type == PIPE)
		return (builtin_pipe(cmd));
	return (0);
}

void process_and_execute(struct cmd *tree) 
{
	// Temos que ter cuidado com este fork
	vars()->forked = 0;
	if (is_builtin_tree(tree))
		exec_tree(tree);
	else
	{
		vars()->forked = 1;
		int cpid = fork();
		if (cpid == -1)
			perror("fork");
		if (cpid == 0)
		{
			exec_tree(tree);
		}
		// printf("before execve = %d\n", exit_status);
		int status;
		waitpid(cpid, &status, 0);
		if (WIFEXITED(status)) {
			exit_status = WEXITSTATUS(status);
			// printf("Child process exited with status: %d\n", exit_status);
		} else {
			printf("Child process did not exit correctly\n");
		}
		exit_status = exit_status % 256;
		// printf("exit_main = %d\n", exit_status);
	}
}

void process_input() {
	while (1) {
		int i = 0;
		while(vars()->envp[i] != NULL)
		{
			i++;
		}
		vars()->num_sc = 14;
		create_sc();
		setup_signals();
		vars()->s = readline("minishell>");
		if (vars()->s == NULL)
			break;
		// if (strncmp(vars()->s, "exit", ft_strlen("exit")) == 0) {
		//     printf("vamos sair desta merda\n");
		//     exit_program(vars()->s);
		//     cleanup();
		//     break;
		// }
		if (all_space(vars()->s)) {
			cleanup();
			continue;
		}
		add_history(vars()->s);

		elems()->s = vars()->s;
		lexer(vars()->envp);
		struct cmd *tree = parsepipe(vars()->tokens);
		// debug_tree(tree);
		process_and_execute(tree);
		cleanup();
		//exit (exit_status);
	}
}

char **duplicate_envp(char **envp) {
	int	envp_size = 0;
	while (envp[envp_size] != NULL) {
		envp_size++;
	}

	char **new_envp = (char **)malloc((envp_size + 1) * sizeof(char *));
	if (!new_envp)
	{
		perror("malloc");
		exit(1);
	}
	int i = 0;
	while (envp[i] != NULL)
	{
		new_envp[i] = strdup(envp[i]);
		if (!new_envp[i]) 
		{
			perror("strdup");
			exit(1);
		}
		i++;
	}
	new_envp[envp_size] = NULL;
	return (new_envp);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	exit_status = 0;
	vars()->path_arg = create_path(envp);
	vars()->envp = duplicate_envp(envp);
	process_input();
	return (0);
}
