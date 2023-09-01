/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/10 00:20:47 by mjales            #+#    #+#             */
/*   Updated: 2023/08/10 00:48:15 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	**list_to_array(t_list *lst)
{
	int		count;
	t_list	*tmp;
	char	**arr; 
	int		i;

	count = 0;
	tmp = lst;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	arr = malloc((count + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	i = 0;
	tmp = lst;
	while (tmp)
	{
		arr[i] = tmp->content->s;
		tmp = tmp->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
