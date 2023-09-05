/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 23:05:19 by mjales            #+#    #+#             */
/*   Updated: 2023/09/05 15:26:19 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern int	exit_status;

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

void subdivide_tokens(void)
{
    t_list *new_tokens;
    t_list *current = vars()->tokens;
    for (int j = 0; j < 14; j++)
    {
        new_tokens = NULL;
        current = vars()->tokens;
        while (current != NULL) {
            int state = current->content->state;

            // Subdividir por espaços em branco se o estado for DEF
            if (state == DEF && !is_special(current->content->s, vars()->sc)) {
                char **subtokens = ft_split(current->content->s, ' ');
                for (size_t i = 0; subtokens[i] != NULL; i++) {
                    char *part = subtokens[i];
                    if (strstr(part, vars()->sc[j])) {
                        add_token(&new_tokens, strndup(part, strstr(part, vars()->sc[j]) - part), state);
                        add_token(&new_tokens, vars()->sc[j], state);
                        add_token(&new_tokens, strstr(part, vars()->sc[j]) + ft_strlen(vars()->sc[j]), state);
                    } else {
						// printf("part = {%s}\n",part);
                        add_token(&new_tokens, part, state);
                    }
                    free(part);
                }
                free(subtokens);
            }
            else {
				// printf("LEAK\n");
				// print_tokens(new_tokens);
                add_token(&new_tokens, current->content->s, state);
				// print_tokens(new_tokens);
            }
            current = current->next;
        }
        free_tokens(vars()->tokens);
        vars()->tokens = new_tokens;

    }
    // print_tokens(vars()->tokens);
}

void	free_tokens(t_list *lst)
{
	t_list	*current;
	t_list	*next;

	current = lst;
	while (current != NULL)
	{
		next = current->next;
		free(current->content->s);
		free(current->content);
		free(current);
		current = next;
	}
	vars()->tokens = NULL;
}
