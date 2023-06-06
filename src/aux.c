/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 12:28:03 by mjales            #+#    #+#             */
/*   Updated: 2023/06/06 11:23:09 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	create_sc(void)
{
	vars()->sc = malloc(sizeof(char *) * 14);
	vars()->sc[0] = "#";
	vars()->sc[1] = "$";
	vars()->sc[2] = "&";
	vars()->sc[3] = "*";
	vars()->sc[4] = "@";
	vars()->sc[5] = "\\";
	vars()->sc[6] = ":";
	vars()->sc[7] = ";";
	vars()->sc[8] = "<";
	vars()->sc[9] = ">";
	vars()->sc[10] = "|";
	vars()->sc[11] = "~";
	vars()->sc[12] = "<<";
	vars()->sc[13] = ">>";
}

int	all_space(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] != 32 && s[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}
