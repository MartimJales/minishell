/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 18:37:22 by mjales          #+#    #+#             */
/*   Updated: 2023/05/10 10:20:39 by mjales         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>


#define DEF 0
#define SQ 1
#define DQ 2

//Structs
typedef struct s_elems
{
	char *s;
	int state;
}	t_elems;

typedef struct s_list
{
	t_elems			*content;
	struct s_list	*next;
}	t_list;


typedef struct s_vars
{
	char	*s;
	t_list  *tokens;
	char	**sc;
    char	**path_arg;
    char    **envp;
	int		num_sc;
    int     token_len;
}	t_vars;

//Lexer functions
void lexer(char **envp);
void find_dollar(char** envp);

//Auxiliar functions
size_t ft_strlen(const char* str);
t_vars	*vars(void);
t_elems	*elems(void);
int		all_space(char *s);
void	create_sc(void);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
int	ft_isalnum(int c);
char	*ft_strnstr(const char *big, const char *little, size_t len);
size_t	is_delimiter(const char *s, char **sc, size_t *len);
size_t	next_pos_sc(char const *s, char **sc);
char	**split_sc(char const *s, char **sc);
char	**create_path(char *envp[]);
char	**check_path(t_list *args, char **path_arg);
char	*junta_strings(char *s1, char *s2);
char	*ft_strjoin(char const *s1, char const *s2);
char	**ft_split(char const *s, char c);
void	free_tokens(void);

//MIT functions
int fork1(void);
char **list_to_array(t_list *lst);

//EXEC functions
int    ft_exec(t_list args);

//Linked Lists functions
void	ft_lstadd_back(t_list **lst, t_list *new);
t_list	*ft_lstnew(void *content);

#endif
