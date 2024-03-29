/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux6.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcordovi <dcordovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 01:43:36 by mjales            #+#    #+#             */
/*   Updated: 2023/09/10 20:11:08 by dcordovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_builtin_tree(struct s_cmd *cmd)
{
	if (!cmd)
		return (0);
	if (cmd->type == EXEC)
		return (builtin_exec(cmd));
	else if (cmd->type == REDIR)
		return (is_builtin_tree(((struct s_redircmd *)cmd)->cmd));
	else if (cmd->type == PIPE)
		return (builtin_pipe(cmd));
	return (0);
}

void	free_cmd(struct s_cmd *command)
{
	if (!command)
		return ;
	if (command->type == EXEC)
	{
		free_execcmd((struct s_execcmd *)command);
	}
	else if (command->type == REDIR)
	{
		free_redircmd((struct s_redircmd *)command);
	}
	else if (command->type == PIPE)
	{
		free_pipecmd((struct s_pipecmd *)command);
	}
}

void	process_and_execute(struct s_cmd *tree)
{
	int	cpid;
	int	status;

	vars()->forked = 0;
	if (is_builtin_tree(tree))
		exec_tree(tree);
	else
	{
		vars()->forked = 1;
		cpid = fork();
		if (cpid == -1)
			perror("fork");
		if (cpid == 0)
			exec_tree(tree);
		waitpid(cpid, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else
			exit(1);
		g_exit_status = g_exit_status % 256;
	}
}

// void	process_input(void)
// {
// 	int				i;
// 	struct s_cmd	*tree;

// 	while (1)
// 	{
// 		i = 0;
// 		while (vars()->envp[i] != NULL)
// 			i++;
// 		setup_signals();
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		vars()->s = readline("minishell>");
// 		if (vars()->s == NULL)
// 			break ;
// 		if (all_space(vars()->s))
// 		{
// 			cleanup();
// 			continue ;
// 		}
// 		add_history(vars()->s);
// 		lexer();
// 		tree = parsepipe(vars()->tokens);
// 		if (tree)
// 			process_and_execute(tree);
// 		else
// 			free_tokens(vars()->tokens);
// 		free_cmd(tree);
// 		free(vars()->s);
// 	}
// }

char	*initialize_and_read_input(void)
{
	int	i;

	i = 0;
	while (vars()->envp[i] != NULL)
		i++;
	setup_signals();
	rl_on_new_line();
	rl_replace_line("", 0);
	return (readline("minishell>"));
}

void	process_and_execute_input(char *input)
{
	struct s_cmd	*tree;

	if (all_space(input))
	{
		cleanup();
		return ;
	}
	add_history(input);
	lexer();
	tree = parsepipe(vars()->tokens);
	if (tree)
		process_and_execute(tree);
	else
		free_tokens(vars()->tokens);
	free_cmd(tree);
	free(input);
}
