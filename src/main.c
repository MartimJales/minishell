/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_fail.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 10:17:18 by mjales            #+#    #+#             */
/*   Updated: 2023/09/01 13:10:41 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int exit_status;

t_vars	*vars(void)
{
	static t_vars	var;

	return (&var);
}

t_elems	*elems(void)
{
	static t_elems	elem;

	return (&elem);
}

void	debug_tree(struct cmd *tree)
{
	if (!tree) return ;

	switch (tree->type) {
		case EXEC:
			{
				struct execcmd *e = (struct execcmd *)tree;
				printf("EXEC cmd:\n");
				for (int i = 0; e->argv && e->argv[i]; i++) {
					printf("  argv[%d] = %s\n", i, e->argv[i]);
				}
			}
			break;
		case REDIR:
			{
				struct redircmd *r = (struct redircmd *)tree;
				printf("REDIR cmd:\n");
				printf("  file: %s\n", r->file);
				printf("  mode: %d\n", r->mode);
				// Uncomment the line below if you add the fd field later
				// printf("  fd: %d\n", r->fd);
				printf("  -> Nested cmd:\n");
				debug_tree(r->cmd);
			}
			break;
		case PIPE:
			{
				struct pipecmd *p = (struct pipecmd *)tree;
				printf("PIPE cmd:\n");
				printf("  -> Left cmd:\n");
				debug_tree(p->left);
				printf("  -> Right cmd:\n");
				debug_tree(p->right);
			}
			break;
		default:
			printf("Unknown cmd type: %d\n", tree->type);
			break;
	}
}

void	setup_signals(void)
{
	signal(SIGINT, signal_cmd);
	signal(SIGQUIT, SIG_IGN);
}

void	cleanup(void)
{
	free(vars()->s);
	free_tokens();
}

int	builtin_exec(struct cmd *cmd)
{
	struct execcmd	*ecmd;
	char			*command;

	ecmd = (struct execcmd *)cmd;
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

int	builtin_pipe(struct cmd *cmd)
{
	struct pipecmd	*pcmd;

	pcmd = (struct pipecmd *)cmd;
	if (pcmd->right)
		return (0);
	return (is_builtin_tree(pcmd->left));
}

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
		vars()->s = readline("minishell>");
		setup_signals();
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
		exit (exit_status);
	}
}

char **duplicate_envp(char **envp) {
	int envp_size = 0;
	while (envp[envp_size] != NULL) {
		envp_size++;
	}

	char **new_envp = (char **)malloc((envp_size + 1) * sizeof(char *));
	if (!new_envp) {
		perror("malloc");
		exit(1);
	}

	int i = 0;
	while (envp[i] != NULL) {
		new_envp[i] = strdup(envp[i]);
		if (!new_envp[i]) {
			perror("strdup");
			exit(1);
		}
		i++;
	}
	new_envp[envp_size] = NULL;

	return new_envp;
}

int main(int argc, char **argv, char **envp) {
	(void)argc;
	(void)argv;
	(void)envp;
	exit_status = 0;

	vars()->path_arg = create_path(envp);
	vars()->envp = duplicate_envp(envp);

	process_input();

	return 0;
}
