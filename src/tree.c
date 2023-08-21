/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 00:14:27 by mjales            #+#    #+#             */
/*   Updated: 2023/08/21 13:30:22 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

struct cmd *parsepipe(t_list *lst)
{
    t_list *begin;

    begin = lst;
    if (!lst)
        return NULL;
    while (lst->next && *(lst->next->content->s) != '|'){
        lst = lst->next;
    }
    struct pipecmd *pipe;
    pipe = malloc(sizeof(*pipe));
    pipe->type = PIPE;
    t_list *current = begin;    
    if (lst->next)
        pipe->right = parsepipe(lst->next->next);
    else    
        pipe->right = parseredir(current);
    lst->next = NULL;
    pipe->left = parseredir(current);
    printf("Begin:\n");
    while (current != NULL) {
        printf("token(%d) = {%s}\n", current->content->state, current->content->s);
        current = current->next;
    }
    return (struct cmd*)pipe;
}

int ft_redir_signal(char *s)
{
    if (strncmp(">>", s, 2) == 0)
        return APPEND;
    if (strncmp("<<", s, 2) == 0)
        return HEREDOC;
    if (*s == '>')
        return OUT;
    if (*s == '<')
        return IN;
    return 0; 
}

struct cmd *create_redircmd(t_list *lst, char *filename, int redir_signal)
{
    struct redircmd *cmd;

    cmd = malloc(sizeof(*cmd));
    memset(cmd, 0, sizeof(*cmd));
    cmd->type = REDIR;
    cmd->file = filename;
    cmd->mode = redir_signal;
    cmd->cmd = parseredir(lst);
    return (struct cmd*)cmd;
}

struct cmd *parseredir(t_list *lst)
{
    t_list *old = lst;
    t_list *prev;
    int redir_signal;

    prev->next = lst;
    if (!lst)
        return NULL;
    while (lst)
    {
        redir_signal = ft_redir_signal(lst->content->s);
        if (redir_signal != 0)
        {
            prev->next = prev->next->next->next;
            // prev = lst->next;
            // lst = lst->next->next;
            return create_redircmd(old, lst->next->content->s, redir_signal);            
        }
        else {
            prev = prev->next;
            lst = lst->next;
        }
    }
    return parseexec(old);
}