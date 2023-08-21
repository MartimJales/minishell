/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 00:14:27 by mjales            #+#    #+#             */
/*   Updated: 2023/08/21 01:17:37 by mjales           ###   ########.fr       */
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

struct cmd *parseredir(t_list *lst)
{
    (void)lst;

    struct redircmd *redir;
    

    redir = malloc(sizeof(*redir));
    redir->type = REDIR;
//   redi->cmd = subcmd;
//   cmd->file = file;
//   cmd->efile = efile;
//   cmd->mode = mode;
//   cmd->fd = fd;
    redir->debug = lst->content->s;
    return (struct cmd *)redir;
}