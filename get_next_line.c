/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabounna <yabounna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 08:41:45 by yabounna          #+#    #+#             */
/*   Updated: 2024/11/28 12:59:19 by yabounna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i] != '\0')
	{
		i++;
	}
	return (i);
}

static char	*read_fonction(int fd, char *buf)
{
	int		compt_read;
	char	*next;
	char	*tmp;

	compt_read = 1;
	next = malloc((size_t)BUFFER_SIZE + 1);
	if (!next)
		return (free(buf), NULL);
	while (compt_read > 0)
	{
		compt_read = read(fd, next, BUFFER_SIZE);
		if (compt_read < 0)
			return (free(next), NULL);
		next[compt_read] = '\0';
		tmp = buf;
		buf = ft_strjoin(buf, next);
		if (!buf)
			return (free(next), free(tmp), NULL);
		free(tmp);
		if (ft_strchr(buf, '\n'))
			break ;
	}
	free(next);
	return (buf);
}

static char	*extract_line(char *buf)
{
	int		i;
	char	*line;

	i = 0;
	if (!buf || !buf[0])
		return (NULL);
	while (buf[i] && buf[i] != '\n')
		i++;
	if (buf[i] == '\n')
		i++;
	line = malloc(i + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (buf[i] && buf[i] != '\n')
	{
		line[i] = buf[i];
		i++;
	}
	if (buf[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

static char	*update_buffer(char *buf)
{
	int		i;
	int		j;
	char	*new_buf;

	i = 0;
	j = 0;
	while (buf[i] && buf[i] != '\n')
		i++;
	if (!buf[i])
		return (free(buf), NULL);
	new_buf = malloc(ft_strlen(buf) - i + 1);
	if (!new_buf)
		return (free(buf), NULL);
	i++;
	while (buf[i])
		new_buf[j++] = buf[i++];
	new_buf[j] = '\0';
	free(buf);
	return (new_buf);
}

char	*get_next_line(int fd)
{
	static char	*buf;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) == -1
		|| BUFFER_SIZE > 2147483647)
		return (free(buf), buf = NULL);
	buf = read_fonction(fd, buf);
	if (!buf)
		return (free(buf), buf = NULL);
	line = extract_line(buf);
	if (!line || *line == 0)
		return (free(buf), buf = NULL);
	buf = update_buffer(buf);
	return (line);
}
