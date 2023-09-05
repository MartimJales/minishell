/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 23:05:51 by mjales            #+#    #+#             */
/*   Updated: 2023/09/04 23:06:33 by mjales           ###   ########.fr       */
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
		arr[i] = strdup(tmp->content->s);
		tmp = tmp->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
