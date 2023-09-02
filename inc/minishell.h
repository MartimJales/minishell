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
# include <sys/stat.h>
# include <fcntl.h>


# define DEF 0
# define SQ 1
# define DQ 2

# define EXEC  1
# define REDIR 2
# define PIPE  3

# define OUT  1
# define IN 2
# define APPEND  3
# define HEREDOC  4

# define MAXARGS 10

//Structs
typedef struct s_elems
{
	char	*s;
	pid_t	ppid;
	int		state;
}	t_elems;

typedef struct s_list
{
	t_elems			*content;
	struct s_list	*next;
}	t_list;


typedef struct s_vars
{
	char	*s;
	t_list	*tokens;
	char	**sc;
	char	**path_arg;
	char	**envp;
	int		num_sc;
	int		token_len;
	int		forked;
	int		pipefd[2];
}	t_vars;

struct	cmd {
	int	type;
};

struct execcmd {
	int		type;
	char	**argv;
};

struct	redircmd {
	int			type;
	struct cmd	*cmd;
	char		*file;
	int			mode;
	int			fd;
};

struct pipecmd {
	int			type;
	struct cmd	*left;
	struct cmd	*right;
};

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
void	signal_cmd(int sig);
void print_tokens(t_list * current);
char *ft_itoa(int nbr);
int is_builtin(char *cmd);
void process_and_execute(struct cmd *tree);
long long int	ft_atoi(char *str);

//MIT functions
char **list_to_array(t_list *lst);

//EXEC functions
void    exec_tree(struct cmd *root);
int exec_env(int declare);

//Linked Lists functions
void	ft_lstadd_back(t_list **lst, t_list *new);
t_list	*ft_lstnew(void *content);
t_list	*ft_lstlast(t_list *lst);

//Tree functions
struct cmd *parsepipe(t_list *lst);
struct cmd *parseredir(t_list *lst);
struct cmd *parseexec(t_list *lst);
void debug_tree(struct cmd *root);


int	is_builtin_tree(struct cmd *cmd);

#endif
