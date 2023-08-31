/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 13:00:05 by mjales            #+#    #+#             */
/*   Updated: 2023/08/30 22:09:39 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern int exit_status;

void print_tokens(t_list * lst)
{
    t_list *current = lst;
 
    printf("Tokens:\n");
    while (current != NULL) {
        printf("token(%d) = {%s}\n", current->content->state, current->content->s);
        current = current->next;
    }
}

int is_special(const char *str, char **special) {
    for (int i = 0; i < vars()->num_sc; i++) {
        if (strncmp(str, vars()->sc[i], strlen(vars()->sc[i])) == 0) {
            *special = vars()->sc[i];
            return strlen(vars()->sc[i]);
        }
    }
    return 0;
}

int is_redir(const char *str) {
    if (strncmp(str, ">>", 2) == 0)
        return 1;
    if (strncmp(str, "<<", 2) == 0)
        return 1;
    if (strncmp(str, ">", 1) == 0)
        return 1;
    if (strncmp(str, "<", 1) == 0)
        return 1;
    return 0;
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char *exp_dollar(char *s, char **envp)
{
    char	*path;
    int     i;

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
        return "$";
    return (path + ft_strlen(s));
}


t_list *create_token(int start, int end, int state)
{            
    int size; 
    size = end - start + 1;
    
    t_list *aux;
    aux = ft_lstnew(NULL);
    aux->content = malloc(sizeof(t_elems));
    aux->content->s = malloc(size);
	ft_strlcpy(aux->content->s, elems()->s + start, size);
    aux->content->state = state;
    return aux;
    // ft_lstadd_back(&vars()->tokens, aux);
}


t_list *create_space_token( int state)
{                
    t_list *aux;
    aux = ft_lstnew(NULL);
    aux->content = malloc(sizeof(t_elems));
    aux->content->s = malloc(2);
    aux->content->s = strdup(" ");
    aux->content->state = state;
    return aux;
    // ft_lstadd_back(&vars()->tokens, aux);
}

// char* replace_dollar(const char* input, char **envp) {
//     size_t length = strlen(input);
//     char* result = malloc(length * sizeof(char) * 10); // Alocar espaço suficiente
//     char* str_aux;
//     size_t index_result = 0;

//     for (size_t i = 0; i < length; i++) {
//         if (input[i] == '$') {
//             size_t start = i + 1;
//             size_t j = start;
//             while (ft_isalnum(input[j])) {
//                 j++;
//             }
//             if (input[j] == '?')
//                 j++;
//             str_aux = malloc(j - start + 1);
//             strncpy(str_aux, input + start, j - start);
//             str_aux[j - start] = '\0';
//             char* export = NULL;
//             if (strcmp(str_aux, "?") == 0) 
//                 export = ft_itoa(exit_status);
//             else
//                 export = exp_dollar(str_aux, envp);

//             strcat(result + index_result, export);
//             index_result += strlen(export);
//             i = j - 1; // Atualizar o índice principal para refletir a mudança
//         } else {
//             result[index_result++] = input[i];
//         }
//     }
//     result[index_result] = '\0';        
//     return result;
// }

char *replace_dollar(const char *input, char **envp) {
    size_t length = strlen(input);
    char *result = malloc(length * 10 + 1); // Allocate sufficient space
    result[0] = '\0'; // Ensure the result string starts empty
    char *str_aux;
    size_t index_result = 0;

    for (size_t i = 0; i < length; i++) {
        if (input[i] == '$') {
            size_t start = i + 1;
            size_t j = start;
            while (ft_isalnum(input[j])) {
                j++;
            }
            if (input[j] == '?') {
                j++;
            }
            str_aux = malloc(j - start + 1);
            strncpy(str_aux, input + start, j - start);
            str_aux[j - start] = '\0';
            char *export = NULL;
            if (strcmp(str_aux, "?") == 0) {
                export = ft_itoa(exit_status);
            } else {
                export = exp_dollar(str_aux, envp);
            }

            strcat(result + index_result, export);
            index_result += strlen(export);
            free(str_aux); // Free the temporary string memory
            i = j - 1; // Update the main index to reflect the change
        } else {
            result[index_result++] = input[i];
        }
    }
    result[index_result] = '\0';        
    return result;
}

void find_dollar(char** envp)
{
    t_list *aux = vars()->tokens;
    aux = vars()->tokens;
    // print_tokens(vars()->tokens);
    while(aux != NULL)
    {
        if (aux->content->state != SQ) {
            aux->content->s = replace_dollar(aux->content->s, envp);
        }
        aux = aux->next;
    }
    // printf("AFTER FIND\n");
    // print_tokens(vars()->tokens);    
}

void add_token(t_list **list, const char *token_str, int state) {
	t_list *new_node = (t_list *)malloc(sizeof(t_list));
	t_elems *content = (t_elems *)malloc(sizeof(t_elems));
	content->s = strdup(token_str);
	content->state = state;
	new_node->content = content;
	new_node->next = *list;
	*list = new_node;
}


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
    t_list *current = vars()->tokens;
    while (current != NULL) {
        t_list *next = current->next;
        free(current->content->s); // Libera a string do token
        free(current->content);    // Libera a estrutura do conteúdo
        free(current);            // Libera o nó da lista
        current = next;
    }
    vars()->tokens = NULL; // Certifique-se de definir a lista de tokens como NULL depois de liberar
}

void junta_tokens(t_list *lst)
{
    t_list *cur = lst;
    t_list *prev = malloc(sizeof(t_list *));
    
    prev->next = lst;
    while (cur && cur->next)
    {
        // If current token is not space and next token is also not a space
        if (strcmp(cur->content->s, " ") != 0 && strcmp(cur->next->content->s, " ") != 0)
        {
            char *concatenated_str = junta_strings(cur->content->s, cur->next->content->s);

            cur->content->s = concatenated_str;
            if (cur->next->content->state > cur->content->state)
                cur->content->state = cur->next->content->state;

            t_list *tmp = cur->next;
            cur->next = tmp->next;
        }
        else
        {
            prev = cur;
            cur = cur->next;
        }
    }
}

void process_quote_state(int *old, int i, int *state) {
    if (*state == 0) {
        ft_lstadd_back(&vars()->tokens, create_token(*old, i, *state));
        *old = i + 1;
        *state = 1;
    } else if (*state == 1) {
        ft_lstadd_back(&vars()->tokens, create_token(*old, i, *state));
        *old = i + 1;
        *state = 0;
    }
}

void process_double_quote_state(int *old, int i, int *state) {
    if (*state == 0) {
        ft_lstadd_back(&vars()->tokens, create_token(*old, i, *state));
        *old = i + 1;
        *state = 2;
    } else if (*state == 2) {
        ft_lstadd_back(&vars()->tokens, create_token(*old, i, *state));
        *old = i + 1;
        *state = 0;
    }
}

void process_space_state(int *old, int i, int *state, int *space) {
    if (*state == 0) {
        ft_lstadd_back(&vars()->tokens, create_token(*old, i, *state));
        *old = i;
        *state = 0;
        *space = 1;
    }
}

void process_new_space(int *state, int *space) {
    ft_lstadd_back(&vars()->tokens, create_space_token(*state));
    *state = 0;
    *space = 0;
}

void lexer(char **envp) {
    int i = -1;
    int old = 0;
    int state = 0;
    int space = 0;

    while (elems()->s[++i]) {
        if (elems()->s[i] == '\'') {
            process_quote_state(&old, i, &state);
            space = 0;
        } else if (elems()->s[i] == '\"') {
            process_double_quote_state(&old, i, &state);
            space = 0;
        } else if (elems()->s[i] == ' ' && state == 0) {
            process_space_state(&old, i, &state, &space);
        } else if (is_redir(&vars()->s[i]) && state == 0) {
            process_space_state(&old, i, &state, &space);
            i++;
            if (is_redir(&vars()->s[i]) && state == 0)
                i++;
            process_space_state(&old, i, &state, &space);
            process_new_space(&state, &space);
        } else if (space) {
            ft_lstadd_back(&vars()->tokens, create_token(old, i, state));
            old = i;
            state = 0;
            space = 0;
        }
    }
    ft_lstadd_back(&vars()->tokens, create_token(old, i, state));
    find_dollar(envp);
    // print_tokens(vars()->tokens);
    junta_tokens(vars()->tokens);
    // printf("JUNTA\n");
    // print_tokens(vars()->tokens);
    subdivide_tokens();
    // printf("DIVIDE \n");
    // print_tokens(vars()->tokens);
}


