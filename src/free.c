/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 01:47:10 by mjales            #+#    #+#             */
/*   Updated: 2023/09/06 02:22:55 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
