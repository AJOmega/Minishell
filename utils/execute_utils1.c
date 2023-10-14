/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabreu-d <jabreu-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 19:09:47 by jabreu-d          #+#    #+#             */
/*   Updated: 2023/10/07 19:09:47 by jabreu-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_env_value(char *env_name, char **envp)
{
	int		i;

	i = 0;
	if (!env_name)
		return (NULL);
	if (env_name[0] == '?')
		return (ft_itoa(g_global.exit_code));
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], env_name, ft_strlen(env_name)))
			return (ft_strdup(envp[i] + ft_strlen(env_name) + 1));
		i++;
	}
	return (ft_strdup(""));
}

void	clean_exit(t_utils_hold *utils_hold, int exit_code)
{
	t_lexer	*current;
	t_lexer	*next;

	current = utils_hold->simple_cmds->redirections;
	while (current)
	{
		next = current->next;
		free(current->str);
		free(current);
		current = next;
	}
	free(utils_hold->args);
	free(utils_hold->pwd);
	free(utils_hold->old_pwd);
	free_array(utils_hold->paths);
	rl_clear_history();
	free(utils_hold->simple_cmds);
	if (utils_hold->pipes)
		free(utils_hold->pid);
	exit(exit_code);
}

void	print_arrayy(char **array)
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

void	handle_cmd(t_simple_cmds *cmd, t_utils_hold *utils_hold)
{
	int	exit_code;

	exit_code = 0;
	if (cmd->redirections)
	{
		if (check_redirections(cmd, utils_hold))
		{
			clean_exit(utils_hold, exit_code);
			perror("error\n");
		}
	}
	utils_hold->args = join_split_str(cmd->str, NULL);
	if (cmd->str[0][0] != '\0' && check_builtins(utils_hold) == 1)
	{
		which_command(utils_hold);
		if (cmd->str)
			free_array(cmd->str);
	}
	else if (cmd->str[0][0] != '\0')
		exit_code = find_cmd(cmd, utils_hold);
	free_array(utils_hold->envp);
	clean_exit(utils_hold, exit_code);
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
