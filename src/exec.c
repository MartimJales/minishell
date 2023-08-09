/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 00:16:50 by mjales            #+#    #+#             */
/*   Updated: 2023/08/10 00:57:25 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int    ft_exec(t_list args)
{
    char	**teste;

    teste = check_path(&args, vars()->path_arg);

    return (execve(teste[0], teste, vars()->envp));
}
