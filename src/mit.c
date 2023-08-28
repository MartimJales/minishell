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

char **list_to_array(t_list *lst)
{
    int count = 0;
    t_list *tmp = lst;
    
    // Count the number of elements in the list
    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }

    // Allocate memory for the char ** array
    char **arr = malloc((count + 1) * sizeof(char *));
    if (!arr)
        return NULL; // Memory allocation failed

    int i = 0;
    tmp = lst;
    while (tmp)
    {
        arr[i] = tmp->content->s;
        tmp = tmp->next;
        i++;
    }
    arr[i] = NULL; // Null-terminate the array

    return arr;
}


