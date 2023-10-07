/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabreu-d <jabreu-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 18:49:47 by jabreu-d          #+#    #+#             */
/*   Updated: 2023/10/07 18:49:59 by jabreu-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strdup2(const char *src, char c)
{
	char	*dest;
	int		i;
	int		len;

	len = 0;
	while (src[len] != '\0')
		len++;
	i = 0;
	dest = (char *)malloc(len * sizeof(char) + 2);
	if (!dest)
		return (0);
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = c;
	i++;
	dest[i] = '\0';
	return (dest);
}

int	ft_strcharcmp2(char *s1, const char *s2, char c)
{
	int		i;
	int		flag;
	char	*new_s1;

	i = 0;
	flag = 0;
	if (s1[ft_strlen(s1)] == c)
	{
		free(s1);
		return (1);
	}
	else
	{
		new_s1 = ft_strdup2(s1, c);
		s1 = new_s1;
	}
	while (s1[i] && s2[i] && s1[i] != c && s2[i] != c)
	{
		if (s1[i] != s2[i])
		{
			free(new_s1);
			return (0);
		}
		i++;
	}	
	if (s1[i] == c && s2[i] == c)
		flag = 1;
	free(new_s1);
	if (flag == 1)
		return (1);
	return (0);
}
