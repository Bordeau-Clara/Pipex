/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <cbordeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 09:17:42 by cbordeau          #+#    #+#             */
/*   Updated: 2025/02/07 12:37:34 by cbordeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	fail_exit(char *message, t_struct args, int *pipefd, int mode)
{
	if (args.cmd != 0 || args.here_doc)
		close(args.last_fd);
	if (mode)
		(close(pipefd[0]), close(pipefd[1]), perror(message), exit(FAILURE));
	else
		(perror(message), exit(FAILURE));
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

char	*find_path(t_struct args)
{
	char	**path;
	int		i;
	char	*path_cmd;
	char	**tab_cmd;

	i = -1;
	tab_cmd = ft_split(args.av[args.cmd], ' ');
	if (!tab_cmd)
		return (NULL);
	if (!access(tab_cmd[0], X_OK))
	{
		path_cmd = ft_strdup(tab_cmd[0]);
		return (free(tab_cmd), path_cmd);
	}
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
		{
			path_cmd = ft_strdup(path[i]);
			if (!path_cmd)
				return (ft_freeall(path), ft_freeall(tab_cmd), NULL);
			return (ft_freeall(path), ft_freeall(tab_cmd), path_cmd);
		}
	}
	perror("access");
	return (ft_freeall(path), ft_freeall(tab_cmd), NULL);
}
