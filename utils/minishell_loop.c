/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabreu-d <jabreu-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 19:18:33 by jabreu-d          #+#    #+#             */
/*   Updated: 2023/10/07 19:18:33 by jabreu-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*detect_dollar_sigs(char *args, char *var_value, char *var_name, int *i)
{
	if (ft_strlen(var_value) == 0)
		args = empty_env(args, var_name, (*i));
	else
		args = update_args(args, var_value, var_name, (*i));
	(*i) = (*i) + ft_strlen(var_value);
	free(var_value);
	return (args);
}

char	*replace_env_vars(char *args, char **envp)
{
	char	*var_name;
	char	*var_value;
	int		i;
	bool	in_quotes;

	if (ft_strchr(args, '$') == NULL)
		return (args);
	i = 0;
	in_quotes = false;
	while (args[i] != '\0')
	{
		if (args[i] == '\"')
			in_quotes = !in_quotes;
		if (args[i] == '\'' && in_quotes == false)
		{
			i++;
			while (args[i] != '\'')
				i++;
		}
		if (args[i] == '$')
		{
			var_name = take_var_name(args, i);
			var_value = get_env_value(var_name, envp);
			args = detect_dollar_sigs(args, var_value, var_name, &i);
			free(var_name);
		}
		i++;
	}
	args[i] = '\0';
	return (args);
}

int	minishell_loop(t_utils_hold *utils_hold)
{
	char	*tmp;

	utils_hold->args = readline("minishell> ");
	tmp = ft_strtrim(utils_hold->args, " ");
	free(utils_hold->args);
	utils_hold->args = tmp;
	if (!utils_hold->args)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		exit(0);
	}
	if (ft_strlen(utils_hold->args) == 0)
		reset_utils_hold(utils_hold);
	add_history(utils_hold->args);
	utils_hold->args = replace_env_vars(utils_hold->args, utils_hold->envp);
	if (count_quotes(utils_hold) == 1)
		return (ft_error(2, utils_hold));
	if (ft_strlen(utils_hold->args) == 0)
		reset_utils_hold(utils_hold);
	if (token_reader(utils_hold) == 0)
		return (ft_error(1, utils_hold));
	parser(utils_hold);
	prepare_executor(utils_hold);
	reset_utils_hold(utils_hold);
	return (1);
}
