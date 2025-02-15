/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <cbordeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 09:17:42 by cbordeau          #+#    #+#             */
/*   Updated: 2025/02/15 12:05:03 by cbordeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "pipex.h"
#include <stdio.h>

void	fail_exit(char *message, t_struct args, int *pipefd, int mode)
{
	if (args.cmd != 0 || args.here_doc)
		close(args.last_fd);
	if (mode == 1)
		(close(pipefd[0]), close(pipefd[1]), perror(message), exit(FAILURE));
	if (mode == 0)
		(perror(message), exit(FAILURE));
	if (mode == 2)
	{
		write(2, "command not found: ", 19);
		write(2, args.av[args.cmd], ft_strlen(args.av[args.cmd]));
		(close(pipefd[0]), close(pipefd[1]), exit(FAILURE));
	}
}

int	max(char *s, char *limiter)
{
	if (ft_strlen(s) > ft_strlen(limiter))
		return (ft_strlen(s) - 1);
	else
		return (ft_strlen(limiter));
}

t_struct	init_args(int ac, char **av, char **env)
{
	t_struct	args;

	if (ac < 5)
		(write(2, WRONG_ARG, 21), exit(FAILURE));
	if (!env)
		(write(2, "No environment\n", 15), exit(FAILURE));
	args.cmd = 0;
	args.n_cmd = ac - 2;
	args.save_pid = malloc((ac - 2) * sizeof(int));
	args.infile = av[1];
	args.outfile = av[ac - 1];
	args.env = env;
	if (!ft_strncmp(av[1], "here_doc", ft_strlen(av[1])))
	{
		if (ac < 6)
			(free(args.save_pid), write(2, WRONG_ARG, 21), exit(FAILURE));
		args.here_doc = 1;
		args.limiter = av[2];
		args.n_cmd -= 1;
		av++;
	}
	else
		args.here_doc = 0;
	args.av = av + 2;
	return (args);
}

char	*find_path(t_struct args, int i)
{
	char	**path;
	char	**tab_cmd;

	tab_cmd = ft_split(args.av[args.cmd], ' ');
	if (!tab_cmd)
		return (NULL);
	if (!access(tab_cmd[0], X_OK))
		return (free(tab_cmd), ft_strdup(tab_cmd[0]));
	while (*args.env && !ft_strnstr(*args.env, "PATH=", 5))
		args.env++;
	path = ft_split(*args.env + 5, ':');
	if (!path)
		return (ft_freeall(tab_cmd), NULL);
	while (path[++i])
	{
		path[i] = ft_strjoin(path[i], "/");
		if (!path[i])
			return (ft_freeall(path), ft_freeall(tab_cmd), NULL);
		path[i] = ft_strjoin(path[i], tab_cmd[0]);
		if (!path[i])
			return (ft_freeall(path), ft_freeall(tab_cmd), NULL);
		if (!access(path[i], X_OK))
			return (ft_freeall(path), ft_freeall(tab_cmd), ft_strdup(path[i]));
	}
	return (ft_freeall(path), ft_freeall(tab_cmd), NULL);
}
