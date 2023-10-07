/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabreu-d <jabreu-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 19:24:32 by jabreu-d          #+#    #+#             */
/*   Updated: 2023/10/07 19:25:15 by jabreu-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_quotes(t_utils_hold *utils_hold)
{
	if (any_quote(utils_hold->args) == 0)
		return (0);
	if (single_quote(utils_hold->args) == 1 \
	|| double_quote(utils_hold->args) == 1)
		return (1);
	utils_hold->args = delete_quotes(utils_hold->args, '"');
	utils_hold->args = delete_quotes(utils_hold->args, '\'');
	return (0);
}

char	**ft_envpdup(char **envp)
{
	char	**aux;
	size_t	i;

	i = 0;
	while (envp[i] != NULL)
		i++;
	aux = ft_calloc(sizeof(char *), i + 1);
	if (!aux)
		return (NULL);
	i = 0;
	while (envp[i] != NULL)
	{
		aux[i] = ft_strdup(envp[i]);
		if (aux[i] == NULL)
		{
			free_array(aux);
			return (aux);
		}
		i++;
	}
	return (aux);
}

void	free_array(char **split_array)
{
	int	i;

	i = 0;
	while (split_array[i])
	{
		if (split_array[i])
			free(split_array[i]);
		i++;
	}
	if (split_array)
		free(split_array);
}
