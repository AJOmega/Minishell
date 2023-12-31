/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: micarrel <micarrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 05:01:26 by micarrel          #+#    #+#             */
/*   Updated: 2023/07/23 05:01:26 by micarrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_parser_utils	init_parser(t_lexer *lexer_list, t_utils_hold *utils_hold)
{
	t_parser_utils	parser_utils;

	parser_utils.lexer_list = lexer_list;
	parser_utils.redirections = NULL;
	parser_utils.num_redirections = 0;
	parser_utils.utils = utils_hold;
	return (parser_utils);
}

t_simple_cmds	*init_cmd(t_parser_utils *parser_utils)
{
	char			*str;
	int				arg_size;
	t_lexer			*tmp;
	t_simple_cmds	*result;

	rm_redirections(parser_utils);
	arg_size = count_args(parser_utils->lexer_list);
	str = ft_calloc(arg_size + 1, sizeof(char *));
	if (!str)
		return (NULL);
	tmp = parser_utils->lexer_list;
	result = init_cmd_utils(parser_utils, str, arg_size, tmp);
	return (result);
}

t_simple_cmds	*init_cmd_utils(t_parser_utils *parser_utils,
							char *str, int arg_size, t_lexer *tmp)
{
	int	i;

	i = 0;
	while (arg_size > 0 && tmp)
	{
		if (tmp->str)
		{
			str[i] = ft_strdup(tmp->str);
			if (!str[i])
			{
				while (i > 0)
					free(str[--i]);
				free(str);
				return (NULL);
			}
			lexer_delete_one(&parser_utils->lexer_list, tmp->i);
			tmp = parser_utils->lexer_list;
			i++;
		}
		arg_size--;
	}
	tmp = parser_utils->lexer_list;
	str[i] = NULL;
	return (ft_simple_cmdsnew(str,
			parser_utils->num_redirections, parser_utils->redirections));
}

void	simple_cmdsadd_back(t_simple_cmds **lst, t_simple_cmds *new)
{
	t_simple_cmds	*tmp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp)
	{
		if (tmp->next)
		{
			tmp = tmp->next;
		}
		else
			break ;
	}
	tmp->next = new;
	new->prev = tmp;
}

int	parser(t_utils_hold *utils_hold)
{
	t_simple_cmds	*simple_cmds;
	t_parser_utils	parser_utils;

	utils_hold->simple_cmds = NULL;
	if (utils_hold->lexer_list->token == 1)
		return (printf("Pipe cant be first\n"));
	while (utils_hold->lexer_list)
	{
		if (utils_hold->lexer_list->token == 1)
			lexer_delete_one(&utils_hold->lexer_list,
				utils_hold->lexer_list->i);
		if (check_error(utils_hold) != 1)
			return (0);
		parser_utils = init_parser(utils_hold->lexer_list, utils_hold);
		simple_cmds = init_cmd(&parser_utils);
		if (!simple_cmds)
			return (0);
		if (!utils_hold->simple_cmds)
			utils_hold->simple_cmds = simple_cmds;
		else
			simple_cmdsadd_back(&utils_hold->simple_cmds, simple_cmds);
		utils_hold->lexer_list = parser_utils.lexer_list;
	}
	return (1);
}
