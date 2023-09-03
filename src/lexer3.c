#include "../inc/minishell.h"

extern int	exit_status;

t_list *create_token(int start, int end, int state)
{
	int		size;
	t_list *aux;

	size = end - start + 1;
	aux = ft_lstnew(NULL);
	aux->content = malloc(sizeof(t_elems));
	aux->content->s = malloc(size);
	ft_strlcpy(aux->content->s, elems()->s + start, size);
	aux->content->state = state;
	return (aux);
    // ft_lstadd_back(&vars()->tokens, aux);
}


t_list *create_space_token( int state)
{
	t_list	*aux;

	aux = ft_lstnew(NULL);
	aux->content = malloc(sizeof(t_elems));
	aux->content->s = malloc(2);
	aux->content->s = strdup(" ");
	aux->content->state = state;
	return (aux);
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

void	find_dollar(char	**envp)
{
	t_list	*aux;

	aux = vars()->tokens;
	while (aux != NULL)
	{
		if (aux->content->state != SQ)
			aux->content->s = replace_dollar(aux->content->s, envp);
		aux = aux->next;
	}
}

void add_token(t_list **list, const char *token_str, int state)
{
	t_list	*new_node;
	t_elems	*content;

	new_node = (t_list *)malloc(sizeof(t_list));
	content = (t_elems *)malloc(sizeof(t_elems));
	content->s = strdup(token_str);
	content->state = state;
	new_node->content = content;
	new_node->next = *list;
	*list = new_node;
}