/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabreu-d <jabreu-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 19:14:47 by jabreu-d          #+#    #+#             */
/*   Updated: 2023/10/10 23:06:51 by jabreu-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_infile(char *file)
{
	int	fd;

	printf("file = %s\n", file);
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: infile: No such file or directory\n",
			STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (fd > 0 && dup2(fd, STDIN_FILENO) < 0)
	{
		ft_putstr_fd("minishell: pipe error\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (fd > 0)
		close(fd);
	return (EXIT_SUCCESS);
}

int	handle_outfile(t_lexer *redirection, char *file)
{
	int	fd;

	printf("file = %s\n", file);
	fd = check_append_outfile(redirection, file);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: outfile: Error\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (fd > 0 && dup2(fd, STDOUT_FILENO) < 0)
	{
		ft_putstr_fd("minishell: pipe error\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (fd > 0)
		close(fd);
	return (EXIT_SUCCESS);
}

void	better_args(t_utils_hold *utils_hold)
{
	int		i;
	char	*tmp;

	i = 0;
	while (utils_hold->args[i])
	{
		if (utils_hold->args[i] == '<' || utils_hold->args[i] == '>')
		{
			tmp = ft_substr(utils_hold->args, 0, i);
			free(utils_hold->args);
			utils_hold->args = ft_strdup(tmp);
			free(tmp);
			tmp = ft_strtrim(utils_hold->args, " ");
			free(utils_hold->args);
			utils_hold->args = ft_strdup(tmp);
			free(tmp);
			return ;
		}
		i++;
	}
	return ;
}

void	free_tmp(t_utils_hold *utils_tmp)
{
	t_lexer	*current;
	t_lexer	*next;

	current = utils_tmp->lexer_list;
	while (current)
	{
		next = current->next;
		free(current->str);
		free(current);
		current = next;
	}
	free(utils_tmp->args);
}

int	check_redirections(t_simple_cmds *cmd, t_utils_hold *utils_hold)
{
	t_lexer			*start;
	t_lexer			*tmp;
	t_utils_hold	utils_tmp;

	utils_tmp.args = ft_strdup(utils_hold->args);
	token_reader(&utils_tmp);
	better_args(utils_hold);
	start = cmd->redirections;
	tmp = utils_tmp.lexer_list;
	while (cmd->redirections)
	{
		if (check_redirections_utils(cmd, tmp) == 1)
			return (1);
		cmd->redirections = cmd->redirections->next;
	}
	free_tmp(&utils_tmp);
	cmd->redirections = start;
	return (0);
}

int	check_redirections_utils(t_simple_cmds *cmd, t_lexer *tmp)
{
	if (cmd->redirections->token == 3)
	{
		while (tmp->token != 3)
			tmp = tmp->next;
		if (handle_infile(tmp->next->str))
			return (1);
	}
	else if (cmd->redirections->token == 2
		|| cmd->redirections->token == 4)
	{
		while (tmp->token != 2
			&& tmp->token != 4)
			tmp = tmp->next;
		if (handle_outfile(cmd->redirections, tmp->next->str))
			return (1);
	}
	else if (cmd->redirections->token == 5)
	{
		while (tmp->token != 5)
			tmp = tmp->next;
		if (handle_infile(tmp->next->str))
			return (1);
	}
	return (0);
}
