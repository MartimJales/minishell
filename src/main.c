/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 10:17:18 by mjales            #+#    #+#             */
/*   Updated: 2023/09/10 12:53:13 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	g_exit_status;

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

char	*ft_strndup(const char *str, size_t n)
{
	char	*new_str;
	size_t	length;

	if (str == NULL || n == 0)
		return (NULL);
	length = strnlen(str, n);
	new_str = (char *)malloc(length + 1);
	if (new_str == NULL)
		return (NULL);
	ft_strncpy(new_str, str, length);
	new_str[length] = '\0';
	return (new_str);
}

void	*ft_memset(void *s, int c, size_t n)
{
	char	*ptr;
	size_t	i;

	ptr = s;
	i = 0;
	while (i < n)
		ptr[i++] = c;
	return (s);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	g_exit_status = 0;
	vars()->path_arg = create_path(envp);
	vars()->envp = duplicate_envp(envp);
	vars()->num_sc = 14;
	create_sc();
	process_input();
	return (0);
}
