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

//Allocates (with malloc(3)) and returns an array
//of strings obtained by splitting ’s’ using the
//character ’c’ as a delimiter. The array must be
//ended by a NULL pointer.
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
    for (int j = 0; j < 14; j++){
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
                        add_token(&new_tokens, part, state);
                    }
                    free(part);
                }
                free(subtokens);
            }
            else {
                add_token(&new_tokens, current->content->s, state);
            }
            current = current->next;
        }
        vars()->tokens = new_tokens;
    }
    // print_tokens(vars()->tokens);
}



// t_list *add_subtoken(t_list *tokens, char *part, int state, char *sc_element) {
//     if (strstr(part, sc_element)) {
//         add_token(&tokens, strndup(part, strstr(part, sc_element) - part), state);
//         add_token(&tokens, sc_element, state);
//         add_token(&tokens, strstr(part, sc_element) + ft_strlen(sc_element), state);
//     } else {
//         add_token(&tokens, part, state);
//     }
//     return tokens;
// }

// t_list *process_subtokens(t_list *current, char *sc_element, t_list *new_tokens) {
//     int state = current->content->state;

//     if (state == DEF && !is_special(current->content->s, vars()->sc)) {
//         char **subtokens = ft_split(current->content->s, ' ');
//         for (size_t i = 0; subtokens[i] != NULL; i++) {
//             char *part = subtokens[i];
//             new_tokens = add_subtoken(new_tokens, part, state, sc_element);
//             free(part);
//         }
//         free(subtokens);
//     } else {
//         new_tokens = add_subtoken(new_tokens, current->content->s, state, sc_element);
//     }

//     return new_tokens;
// }

// void process_special_element(int j) {
//     t_list *new_tokens = NULL;
//     t_list *current = vars()->tokens;

//     while (current != NULL) {
//         new_tokens = process_subtokens(current, vars()->sc[j], new_tokens);
//         current = current->next;
//     }

//     vars()->tokens = new_tokens;
// }

// void subdivide_tokens(void) {
//     for (int j = 0; j < 14; j++) {
//         process_special_element(j);
//     }
//     // print_tokens(vars()->tokens);
// }

void free_tokens(void)
{
	t_list	*current;
	t_list	*next;

	current = vars()->tokens;
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