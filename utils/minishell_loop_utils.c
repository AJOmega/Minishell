/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabreu-d <jabreu-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 19:21:31 by jabreu-d          #+#    #+#             */
/*   Updated: 2023/10/07 19:21:46 by jabreu-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	implement_utils_hold(t_utils_hold *utils_hold)
{
	utils_hold->simple_cmds = NULL;
	utils_hold->lexer_list = NULL;
	utils_hold->reset = false;
	utils_hold->pid = NULL;
	utils_hold->heredoc = false;
	utils_hold->pipes = 0;
	utils_hold->index_list = 0;
	g_global.stop_heredoc = 0;
	g_global.in_cmd = 0;
	g_global.in_heredoc = 0;
	path_find(utils_hold);
	return (1);
}

void	*ft_lstclear_cmd(t_lexer **lst)
{
	if ((*lst)->str)
	{
		free((*lst)->str);
		(*lst) = NULL;
	}
	free(*lst);
	*lst = NULL;
	return (NULL);
}

void	ft_simple_cmdsclear(t_simple_cmds **lst)
{
	t_simple_cmds	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		if ((*lst)->str)
			free_array((*lst)->str);
		if ((*lst)->hd_file_name)
			free((*lst)->hd_file_name);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

void	ft_lstclear2(t_lexer **lst)
{
	t_lexer	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		ft_lstclear_cmd(lst);
		*lst = tmp;
	}
	*lst = NULL;
}

int	reset_utils_hold(t_utils_hold *utils_hold)
{
	ft_simple_cmdsclear(&utils_hold->simple_cmds);
	free(utils_hold->args);
	if (utils_hold->pid)
		free(utils_hold->pid);
	free_array(utils_hold->paths);
	implement_utils_hold(utils_hold);
	utils_hold->reset = true;
	minishell_loop(utils_hold);
	return (1);
}
