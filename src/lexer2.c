#include "../inc/minishell.h"

extern int	exit_status;

void	print_tokens(t_list *lst)
{
	t_list	*current;

	current = lst;
	printf("Tokens:\n");
	while (current != NULL)
	{
		printf("token(%d) = {%s}\n", current->content->state, \
current->content->s);
		current = current->next;
	}
}

int	is_redir(const char *str)
{
	if (strncmp(str, ">>", 2) == 0)
		return (1);
	if (strncmp(str, "<<", 2) == 0)
		return (1);
	if (strncmp(str, ">", 1) == 0)
		return (1);
	if (strncmp(str, "<", 1) == 0)
		return (1);
	return (0);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*exp_dollar(char *s, char **envp)
{
	char	*path;
	int		i;

	i = 0;
    // printf("s = {%s}\n", s);
	if (s == NULL)
		return ("");
	s = junta_strings(s, "=");
	while (envp[i])
	{
		path = ft_strnstr(envp[i], s, ft_strlen(s));
		if (path)
			break ;
		i++;
	}
	if (!path)
		return ("$");
	return (path + ft_strlen(s));
}

void junta_tokens(t_list *lst)
{
	t_list	*cur;
	t_list	*prev;
	t_list	*tmp;
	char	*concatenated_str;

	cur = lst;
	prev = malloc(sizeof(t_list *));
	prev->next = lst;
	while (cur && cur->next)
	{
		if (strcmp(cur->content->s, " ") != 0 && \
strcmp(cur->next->content->s, " ") != 0)
		{
			concatenated_str = \
junta_strings(cur->content->s, cur->next->content->s);

			cur->content->s = concatenated_str;
			if (cur->next->content->state > cur->content->state)
				cur->content->state = cur->next->content->state;

			tmp = cur->next;
			cur->next = tmp->next;
		}
		else
		{
			prev = cur;
			cur = cur->next;
		}
	}
}