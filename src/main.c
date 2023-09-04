/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 10:17:18 by mjales            #+#    #+#             */
/*   Updated: 2023/09/03 23:59:52 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int exit_status;

// Forward declarations
void free_cmd(struct cmd *command);
void free_list(t_list *lst);

void free_elems(t_elems *elem) {
    if (!elem) return;
    free(elem->s);
    free(elem);
}

void free_list(t_list *lst) {
    t_list *temp;
    while (lst) {
        temp = lst;
        lst = lst->next;
        free_elems(temp->content);
        free(temp);
    }
}

void free_vars(t_vars *vars) {
    if (!vars) return;
    free(vars->s);
    free_list(vars->tokens);
    // Assuming sc, path_arg, and envp are null-terminated arrays
    if (vars->sc) {
        for (int i = 0; vars->sc[i]; i++) {
            free(vars->sc[i]);
        }
        free(vars->sc);
    }
    if (vars->path_arg) {
        for (int i = 0; vars->path_arg[i]; i++) {
            free(vars->path_arg[i]);
        }
        free(vars->path_arg);
    }
    if (vars->envp) {
        for (int i = 0; vars->envp[i]; i++) {
            free(vars->envp[i]);
        }
        free(vars->envp);
    }
    free(vars);
}

void free_execcmd(struct execcmd *ecmd) {
    if (!ecmd) return;
    if (ecmd->argv) {
        for (int i = 0; ecmd->argv[i]; i++) {
            free(ecmd->argv[i]);
        }
        free(ecmd->argv);
    }
    free(ecmd);
}

void free_redircmd(struct redircmd *rcmd) {
    if (!rcmd) return;
    free_cmd(rcmd->cmd);
    free(rcmd->file);
    free(rcmd);
}

void free_pipecmd(struct pipecmd *pcmd) {
    if (!pcmd) return;
    free_cmd(pcmd->left);
    free_cmd(pcmd->right);
    free(pcmd);
}

void free_cmd(struct cmd *command) {
    if (!command) return;
    switch (command->type) {
        case EXEC:  // Assuming 'e' for execcmd
            free_execcmd((struct execcmd *)command);
            break;
        case REDIR:  // Assuming 'r' for redircmd
            free_redircmd((struct redircmd *)command);
            break;
        case PIPE:  // Assuming 'p' for pipecmd
            free_pipecmd((struct pipecmd *)command);
            break;
        default:
            // If there are other command types, handle them here
            break;
    }
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
		setup_signals();
		vars()->s = readline("minishell>");
		if (vars()->s == NULL)
			break ;
		if (all_space(vars()->s)) {
			cleanup();
			continue;
		}
		add_history(vars()->s);

		elems()->s = vars()->s;
		lexer(vars()->envp);
		struct cmd *tree = parsepipe(vars()->tokens);
		process_and_execute(tree);
		free_cmd(tree);
		// cleanup();
		exit (exit_status);
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
	free(vars()->sc);
	return (0);
}
