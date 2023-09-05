/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 10:17:18 by mjales            #+#    #+#             */
/*   Updated: 2023/09/05 01:50:18 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int exit_status;

// Forward declarations
void	free_cmd(struct cmd *command);
void	free_list(t_list *lst);

void	free_elems(t_elems *elem)
{
	if (!elem)
		return ;
	free(elem->s);
	free(elem);
}

void	free_list(t_list *lst)
{
	t_list	*temp;

	while (lst) 
	{
		temp = lst;
		lst = lst->next;
		free_elems(temp->content);
		free(temp);
	}
}

void	free_2d_array(char **array)
{
	int	i;

	i = 0;
	while (array && array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_vars(t_vars *vars)
{
	if (!vars)
		return ;
	free(vars->s);
	free_list(vars->tokens);
	free_2d_array(vars->sc);
	free_2d_array(vars->path_arg);
	free_2d_array(vars->envp);
	free(vars);
}

void	free_execcmd(struct execcmd *ecmd)
{
	int	i;

	if (!ecmd)
		return ;
	if (ecmd->argv) 
	{
		i = -1;
		while (ecmd->argv[++i])
			free(ecmd->argv[i]);
		free(ecmd->argv);
	}
	free(ecmd);
}

void	free_redircmd(struct redircmd *rcmd)
{
	if (!rcmd) 
		return ;
	free_cmd(rcmd->cmd);
	free(rcmd->file);
	free(rcmd);
}

void	free_pipecmd(struct pipecmd *pcmd)
{
	if (!pcmd) 
		return ;
	free_cmd(pcmd->left);
	free_cmd(pcmd->right);
	free(pcmd);
}


void	free_cmd(struct cmd *command)
{
	if (!command) 
		return ;

	if (command->type == EXEC)
	{
		free_execcmd((struct execcmd *)command);
	}
	else if (command->type == REDIR)
	{
		free_redircmd((struct redircmd *)command);
	}
	else if (command->type == PIPE)
	{
		free_pipecmd((struct pipecmd *)command);
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

void	process_and_execute(struct cmd *tree) 
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
			exit_status = WEXITSTATUS(status);
		else
			printf("Child process did not exit correctly\n");
		exit_status = exit_status % 256;
	}
}

void process_input(void)
{
	int			i;
	struct cmd	*tree;

	while (1)
	{
		i = 0;
		while (vars()->envp[i] != NULL)
			i++;
		setup_signals();
		vars()->s = readline("minishell>");
		if (vars()->s == NULL)
			break ;
		if (all_space(vars()->s))
		{
			cleanup();
			continue ;
		}
		add_history(vars()->s);
		elems()->s = vars()->s;
		lexer(vars()->envp);
		tree = parsepipe(vars()->tokens);
		process_and_execute(tree);
		free_cmd(tree);
		exit (exit_status);
	}
}

char **duplicate_envp(char **envp)
{
	int		envp_size;
	int		i;
	char	**new_envp;

	envp_size = 0;
	while (envp[envp_size] != NULL)
		envp_size++;
	new_envp = (char **)malloc((envp_size + 1) * sizeof(char *));
	i = 0;
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
	vars()->num_sc = 14;
	create_sc();
	process_input();
	return (0);
}
