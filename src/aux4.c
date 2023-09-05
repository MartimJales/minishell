/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjales <mjales@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 23:04:17 by mjales            #+#    #+#             */
/*   Updated: 2023/09/04 23:41:26 by mjales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern int exit_status;

static int	ft_nbrlen(int nbr)
{
	int	len;

	len = 0;

	if (nbr <= 0)
		len = 1;

	while (nbr)
	{
		nbr /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int nbr)
{
	int		len;
	char	*result;

	if (nbr == -2147483648)
		return ("-2147483648\0");
	len = ft_nbrlen(nbr);
	result = (char *)malloc(sizeof(char) * (len + 1));
	result[len] = '\0';
	if (nbr == 0)
	{
		result[0] = '0';
		return (result);
	}
	if (nbr < 0)
	{
		result[0] = '-';
		nbr = -nbr;
	}
	while (nbr)
	{
		result[--len] = nbr % 10 + '0';
		nbr /= 10;
	}
	return (result);
}



long long int	ft_atoi(char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + *str - '0';
		str++;
	}
	return (sign * result);
}

int	check_alnum(const char *input_string)
{
	if (!input_string || (input_string[0] >= '0' && input_string[0] <= '9'))
		return (0);
	while (*input_string)
	{
		if (!ft_isalnum(*input_string))
		{
			return (0);
		}
		input_string++;
	}
	return (1);
}

int	check_num(const char *input_string)
{
	int	i;

	if (!input_string)
		return (0);
	i = -1;
	while (input_string[++i])
	{
		if (((input_string[i] == '+') || (input_string[i] == '-')) && i == 0)
			continue ;
		if (!(input_string[i] >= '0' && input_string[i] <= '9'))
		{
			return (0);
		}
	}
	return (1);
}

// void	debug_tree(struct cmd *tree)
// {
// 	if (!tree) return ;

// 	switch (tree->type) {
// 		case EXEC:
// 			{
// 				struct execcmd *e = (struct execcmd *)tree;
// 				printf("EXEC cmd:\n");
// 				for (int i = 0; e->argv && e->argv[i]; i++) {
// 					printf("  argv[%d] = %s\n", i, e->argv[i]);
// 				}
// 			}
// 			break;
// 		case REDIR:
// 			{
// 				struct redircmd *r = (struct redircmd *)tree;
// 				printf("REDIR cmd:\n");
// 				printf("  file: %s\n", r->file);
// 				printf("  mode: %d\n", r->mode);
// 				// Uncomment the line below if you add the fd field later
// 				// printf("  fd: %d\n", r->fd);
// 				printf("  -> Nested cmd:\n");
// 				//debug_tree(r->cmd);
// 			}
// 			break;
// 		case PIPE:
// 			{
// 				struct pipecmd *p = (struct pipecmd *)tree;
// 				printf("PIPE cmd:\n");
// 				printf("  -> Left cmd:\n");
// 				debug_tree(p->left);
// 				printf("  -> Right cmd:\n");
// 				debug_tree(p->right);
// 			}
// 			break;
// 		default:
// 			printf("Unknown cmd type: %d\n", tree->type);
// 			break;
// 	}
// }