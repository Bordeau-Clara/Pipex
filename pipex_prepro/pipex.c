/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <cbordeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:55:39 by cbordeau          #+#    #+#             */
/*   Updated: 2025/01/22 10:36:58 by cbordeau         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "Libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

char		*find_path(t_struct args);
void		execute_cmd(t_struct args);
t_struct	init_args(int ac, char **av, char **env);
void		child_process(t_struct args, int *pipefd);
void		parent_process(t_struct args, int *pipefd);

int	main(int ac, char **av, char **env)
{
	int			pipefd[2];
	t_struct	args;
	int			status;
	int			i;

	if (ac < 5)
		return (1);
	i = -1;
	args = init_args(ac, av, env);
	while (args.i < ac - 1)
	{
		if (pipe(pipefd) == -1)
			(perror("pipe"), exit(EXIT_FAILURE));
		args.save_pid[args.i - 2] = fork();
		if (args.save_pid[args.i - 2] == -1)
			(perror("fork"), exit(EXIT_FAILURE));
		if (args.save_pid[args.i - 2] == 0)
			child_process(args, pipefd);
		//else
			//parent_process(args, pipefd);
		close(pipefd[1]);
		if (args.i != 2)
			close(args.last_fd);
		args.last_fd = pipefd[0];
		args.i += 1;
	}
	close(args.last_fd);
	status = 0;
	while (++i < ac - 3)
		waitpid(args.save_pid[i], &status, 0);
	free(args.save_pid);
	return (WEXITSTATUS(status));
}

/*void	parent_process(t_struct *args, int *pipefd)
{
	close(pipefd[1]);
	if (args->i != 2)
		close(args->last_fd);
	args->last_fd = pipefd[0];
	args->i += 1;
}*/

void	child_process(t_struct args, int *pipefd)
{
	close(pipefd[0]);
	free(args.save_pid);
	args.path_cmd = find_path(args);
	if (!args.path_cmd)
		exit(127);
	if (args.i == 2)
	{
		args.fd[0] = open(args.av[1], O_RDONLY);
		if (args.fd[0] < 0)
			(perror("open"), exit(EXIT_FAILURE));
		(dup2(args.fd[0], STDIN_FILENO), close(args.fd[0]));
	}
	else
		(dup2(args.last_fd, STDIN_FILENO), close(args.last_fd));
	if (args.i == args.ac - 2)
	{
		args.fd[1] = open(args.av[args.ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (args.fd[1] < 0)
			(perror("open"), exit(EXIT_FAILURE));
		(dup2(args.fd[1], STDOUT_FILENO), close(args.fd[1]), close(pipefd[1]));
	}
	else
		(dup2(pipefd[1], STDOUT_FILENO), close(pipefd[1]));
	execute_cmd(args);
}

t_struct	init_args(int ac, char **av, char **env)
{
	t_struct	args;

	args.i = 2;
	args.ac = ac;
	args.save_pid = malloc((ac - 2) * sizeof(int));
	args.av = av;
	args.env = env;
	return (args);
}

char	*find_path(t_struct args)
{
	char	**path;
	int		i;
	char	*path_cmd;
	char	**tab_cmd;

	i = -1;
	tab_cmd = ft_split(args.av[args.i], ' ');
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
		return (NULL);
	while (path[++i])
	{
		path[i] = ft_strjoin(path[i], "/");
		path[i] = ft_strjoin(path[i], tab_cmd[0]);
		if (!access(path[i], X_OK))
		{
			path_cmd = ft_strdup(path[i]);
			if (!path_cmd)
				return (ft_freeall(path), NULL);
			return (ft_freeall(path), ft_freeall(tab_cmd), path_cmd);
		}
	}
	return (ft_freeall(path), ft_freeall(tab_cmd), NULL);
}

void	execute_cmd(t_struct args)
{
	char	**split;

	split = ft_split(args.av[args.i], ' ');
	if (!split)
		exit(127);
	execve(args.path_cmd, split, args.env);
	perror("execve");
	(void)args.env;
	free(args.path_cmd);
	(void)args;
	ft_freeall(split);
	exit(EXIT_FAILURE);
}
