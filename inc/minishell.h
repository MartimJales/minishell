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
int	check_alnum(const char *input_string);
int	check_num(const char *input_string);
int	builtin_pipe(struct cmd *cmd);
void	setup_signals(void);
void	cleanup(void);
void	panic(char *error, int status);
int	safe_exit(int status);
int	var_exists(char **envp, char *var);
void heredoc(const char *delimiter);
t_list *create_token(int start, int end, int state);
t_list *create_space_token( int state);
int	is_redir(const char *str);
void subdivide_tokens(void);
void junta_tokens(t_list *lst);
int exec_export(struct execcmd *ecmd);
int exec_unset(struct execcmd *ecmd);
int exec_cd(struct execcmd *ecmd);
int	exec_exit(struct execcmd *ecmd);
void	exec_pipe(struct pipecmd *pcmd);
void	exec_redir(struct redircmd *rcmd);
char	*exp_dollar(char *s, char **envp);
int	is_special(const char *str, char **special);
void add_token(t_list **list, const char *token_str, int state);
int	redir_mode(int redir_signal);
int	ft_redir_signal(char *s);

//MIT functions
char **list_to_array(t_list *lst);

//EXEC functions
void    exec_tree(struct cmd *root);
int exec_env(int declare);
int	builtin_exec(struct cmd *cmd);

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

void	update_var_to_envp(char *var, char *new_value);

#endif
