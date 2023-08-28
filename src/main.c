/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 10:17:18 by mjales            #+#    #+#             */
/*   Updated: 2023/08/28 00:53:45 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int exit_status;

t_vars	*vars(void)
{
	static t_vars	var;
    
	return (&var);
}

t_elems	*elems(void)
{
	static 	t_elems elem;

	return (&elem);
}

void debug_tree(struct cmd *tree)
{
    if (!tree) return ;

    switch (tree->type) {
        case EXEC:
            {
                struct execcmd *e = (struct execcmd *)tree;
                printf("EXEC cmd:\n");
                for (int i = 0; e->argv && e->argv[i]; i++) {
                    printf("  argv[%d] = %s\n", i, e->argv[i]);
                }
            }
            break;
        case REDIR:
            {
                struct redircmd *r = (struct redircmd *)tree;
                printf("REDIR cmd:\n");
                printf("  file: %s\n", r->file);
                printf("  mode: %d\n", r->mode);
                // Uncomment the line below if you add the fd field later
                // printf("  fd: %d\n", r->fd);
                printf("  -> Nested cmd:\n");
                debug_tree(r->cmd);
            }
            break;
        case PIPE:
            {
                struct pipecmd *p = (struct pipecmd *)tree;
                printf("PIPE cmd:\n");
                printf("  -> Left cmd:\n");
                debug_tree(p->left);
                printf("  -> Right cmd:\n");
                debug_tree(p->right);
            }
            break;
        default:
            printf("Unknown cmd type: %d\n", tree->type);
            break;
    }
}

void setup_signals() {
    signal(SIGINT, signal_cmd);
    signal(SIGQUIT, SIG_IGN);
}

void cleanup() {
    free(vars()->s);
    free_tokens();
}

int is_builtin_tree(struct cmd *cmd) {
    if (!cmd) {
        return 0;
    }
    switch (cmd->type) {
        case EXEC:
            {
                struct execcmd *ecmd = (struct execcmd *)cmd;
                char *command = ecmd->argv[0];
                if (command) {
                    // Replace these conditions with your own logic to check for builtins
                    if (is_builtin(command)) {
                        return 1;  // It's a builtin command
                    } else {
                        return 0;  // It's an external command
                    }
                }
            }
            break;
        case REDIR:
            {
                struct redircmd *rcmd = (struct redircmd *)cmd;
                return is_builtin_tree(rcmd->cmd);  // Check the command inside the redirection
            }
            break;
        case PIPE:
            {
                struct pipecmd *pcmd = (struct pipecmd *)cmd;
                return is_builtin_tree(pcmd->left);  // Check the left command of the pipe
            }
            break;
        default:
            break;
    }

    return 0;  // Default to external command
}

void process_and_execute(struct cmd *tree) 
{
    // Temos que ter cuidado com este fork
    if (is_builtin_tree(tree))
        exec_tree(tree);
    else
    {
        if (fork1() == 0)
            exec_tree(tree);
        wait(0);
    }
}

void process_input() {
    while (1) {
        int i = 0;
        while(vars()->envp[i] != NULL)
        {
            i++;
        }
        vars()->num_sc = 14;
        create_sc();
        vars()->s = readline("minishell>");
        setup_signals();
        if (vars()->s == NULL)
            break;
        if (strcmp(vars()->s, "exit") == 0) {
            cleanup();
            break;
        }
        if (all_space(vars()->s)) {
            cleanup();
            continue;
        }
        add_history(vars()->s);

        elems()->s = vars()->s;
        lexer(vars()->envp);
        struct cmd *tree = parsepipe(vars()->tokens);
        // debug_tree(tree);
        process_and_execute(tree);
        cleanup();
        // exit(exit_status); // This is for the tester, needs to be reviewed
    }
}

int main(int argc, char **argv, char **envp) {
    (void)argc;
    (void)argv;
    (void)envp;
    exit_status = 0;

    vars()->path_arg = create_path(envp);
    vars()->envp = envp;

    process_input();

    return 0;
}
