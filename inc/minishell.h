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

extern int	g_exit_status;

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
	t_list	*prev;
}	t_vars;

struct	s_cmd {
	int	type;
};

struct s_execcmd {
	int		type;
	char	**argv;
};

struct	s_redircmd {
	int				type;
	struct s_cmd	*cmd;
	char			*file;
	int				mode;
	int				fd;
};

struct s_pipecmd {
	int				type;
	struct s_cmd	*left;
	struct s_cmd	*right;
};

//Lexer functions
void			lexer(void);
void			find_dollar(void);

//Auxiliar functions
size_t			ft_strlen(const char *str);
t_vars			*vars(void);
t_elems			*elems(void);
int				all_space(char *s);
void			create_sc(void);
size_t			ft_strlcpy(char *dst, const char *src, size_t size);
int				ft_isalnum(int c);
char			*ft_strnstr(const char *big, const char *little, size_t len);
size_t			is_delimiter(const char *s, char **sc, size_t *len);
char			**create_path(char *envp[]);
char			**check_path(t_list *args, char **path_arg);
char			*junta_strings(char *s1, char *s2);
char			*ft_strjoin(char const *s1, char const *s2);
char			**ft_split(char const *s, char c);
void			free_tokens(t_list *lst);
void			signal_cmd(int sig);
void			print_tokens(t_list *current);
char			*ft_itoa(int nbr);
int				is_builtin(char *cmd);
void			process_and_execute(struct s_cmd *tree);
long long int	ft_atoi(char *str);
int				check_alnum(const char *input_string);
int				check_num(const char *input_string);
int				builtin_pipe(struct s_cmd *cmd);
void			setup_signals(void);
void			cleanup(void);
void			panic(char *error, int status);
int				safe_exit(int status);
int				var_exists(char **envp, char *var);
void			heredoc(const char *delimiter);
t_list			*create_token(int start, int end, int state);
t_list			*create_space_token( int state);
int				is_redir(const char *str);
void			subdivide_tokens(void);
void			junta_tokens(t_list *lst);
int				exec_export(struct s_execcmd *ecmd);
int				exec_unset(struct s_execcmd *ecmd);
int				exec_cd(struct s_execcmd *ecmd);
int				exec_exit(struct s_execcmd *ecmd);
void			exec_pipe(struct s_pipecmd *pcmd);
void			exec_redir(struct s_redircmd *rcmd);
char			*exp_dollar(char *s, char **envp);
int				is_special(const char *str, char **special);
void			add_token(t_list **list, const char *token_str, int state);
int				redir_mode(int redir_signal);
int				ft_redir_signal(char *s);
int				ft_strcmp(char *s1, const char *s2);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
char			*ft_strdup(const char *s);
char			*ft_strncpy(char *dest, const char *src, unsigned int n);
void			*ft_realloc(void *ptr, size_t size);
void			*ft_memcpy(void *s1, const void *s2, size_t n);
char			*ft_strchr(const char *s, int c);
char			*ft_strstr(const char *s1, const char *s2);
char			*ft_strndup(const char *str, size_t n);
void			*ft_memset(void *s, int c, size_t n);
void			execute_child(struct s_pipecmd *pcmd, int pipefd[]);
char			*initialize_and_read_input(void);
void			process_and_execute_input(char *input);
t_list			*handle_token_subdivision(char *content, int j, int state);
void			execute_parent(struct s_pipecmd *pcmd, int pipefd[]);
void			init_pipe(void);
void			write_to_pipe_and_cleanup(char *mi);
char			*append_to_multiline(char *mi, char *input_buffer);
void			heredoc_signals(void);

//MIT functions
char			**list_to_array(t_list *lst);

//EXEC functions
void			exec_tree(struct s_cmd *root);
int				exec_env(int declare);
int				builtin_exec(struct s_cmd *cmd);

//Linked Lists functions
void			ft_lstadd_back(t_list **lst, t_list *new);
t_list			*ft_lstnew(void *content);
t_list			*ft_lstlast(t_list *lst);

//Tree functions
struct s_cmd	*parsepipe(t_list *lst);
struct s_cmd	*parseredir(t_list *lst);
struct s_cmd	*parseexec(t_list *lst);
void			debug_tree(struct s_cmd *root);

int				is_builtin_tree(struct s_cmd *cmd);

void			update_var_to_envp(char *var, char *new_value);
void			token_conditions(int *old, int *i, int *state, int *space);
char			*replace_dollar(char *input);
void			add_token(t_list **list, const char *token_str, int state);
char			**duplicate_envp(char **envp);
void			process_input(void);
t_list			*subdivide_current_token(t_list *current, int j);
void			free_nodes(t_list *to_free1, t_list *to_free2);
struct s_cmd	*create_redircmd(t_list *lst, char *filename, int redir_signal);
int				detect_redirection(t_list *current);

void			free_execcmd(struct s_execcmd *ecmd);
void			free_redircmd(struct s_redircmd *rcmd);
void			free_pipecmd(struct s_pipecmd *pcmd);
void			free_list(t_list *lst);
void			free_cmd(struct s_cmd *command);
void			free_envp(char **envp);
int				validate_format(const char *input);

#endif
