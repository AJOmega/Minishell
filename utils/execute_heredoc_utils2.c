/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabreu-d <jabreu-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 19:08:26 by jabreu-d          #+#    #+#             */
/*   Updated: 2023/10/07 19:08:47 by jabreu-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		printf("array[%d]: %s\n", i, array[i]);
		i++;
	}
	printf("\n");
}

void	print_list2(t_simple_cmds *map)
{
	while (map)
	{
		print_array(map->str);
		map = map->next;
	}
}

int	executor(t_utils_hold *utils_hold)
{
	int		end[2];
	int		fd_in;

	fd_in = STDIN_FILENO;
	while (utils_hold->simple_cmds)
	{
		if (utils_hold->simple_cmds->next)
			pipe(end);
		send_heredoc(utils_hold, utils_hold->simple_cmds);
		ft_fork(utils_hold, end, fd_in, utils_hold->simple_cmds);
		close(end[1]);
		if (utils_hold->simple_cmds->prev)
			close(fd_in);
		fd_in = check_fd_heredoc(utils_hold, end, utils_hold->simple_cmds);
		if (utils_hold->simple_cmds->next)
			utils_hold->simple_cmds = utils_hold->simple_cmds->next;
		else
			break ;
	}
	pipe_wait(utils_hold->pid, utils_hold->pipes);
	utils_hold->simple_cmds = ft_simple_cmdsfirst(utils_hold->simple_cmds);
	return (0);
}
