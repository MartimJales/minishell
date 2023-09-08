/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 23:05:19 by mjales            #+#    #+#             */
/*   Updated: 2023/09/08 23:26:31 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

size_t	next_pos(char const *s, int c)
{
	size_t	i;

	i = -1;
	while (s[++i])
		if (s[i] == c)
			return (i);
	return (i);
}

size_t	string_number(char const *s, int c)
{
	size_t	i;
	size_t	num;
	size_t	flag;

	flag = 0;
	i = -1;
	num = 0;
	while (s[++i])
	{
		if (s[i] != c)
		{
			if (!flag)
				num++;
			flag = 1;
		}
		else
			flag = 0;
	}
	return (num);
}

char	**ft_split(char const *s, char c)
{
	size_t	num;
	size_t	i;
	size_t	len;
	char	**arr;

	num = string_number(s, c);
	arr = malloc(sizeof(char *) * (num + 1));
	if (!arr)
		return (NULL);
	i = -1;
	while (++i < num)
	{
		while (*s == c)
			s++;
		len = next_pos(s, c);
		arr[i] = malloc(len + 1);
		ft_strlcpy(arr[i], s, len + 1);
		s = s + len + 1;
	}
	arr[i] = NULL;
	return (arr);
}

t_list	*handle_token_subdivision(char *content, int j, int state)
{
	t_list	*new_tokens;
	char	**subtokens;
	size_t	i;
	char	*part;

	i = -1;
	subtokens = ft_split(content, ' ');
	new_tokens = NULL;
	while (subtokens[++i] != NULL)
	{
		part = subtokens[i];
		if (ft_strstr(part, vars()->sc[j]))
		{
			add_token(&new_tokens, \
ft_strndup(part, ft_strstr(part, vars()->sc[j]) - part), state);
			add_token(&new_tokens, vars()->sc[j], state);
			add_token(&new_tokens, \
ft_strstr(part, vars()->sc[j]) + ft_strlen(vars()->sc[j]), state);
		}
		else
			add_token(&new_tokens, part, state);
		free(part);
	}
	free(subtokens);
	return (new_tokens);
}

t_list	*subdivide_current_token(t_list *current, int j)
{
	t_list	*new_tokens;
	int		state;

	new_tokens = NULL;
	state = current->content->state;
	if (state == DEF && !is_special(current->content->s, vars()->sc))
		new_tokens = handle_token_subdivision(current->content->s, j, state);
	else
		add_token(&new_tokens, current->content->s, state);
	return (new_tokens);
}
