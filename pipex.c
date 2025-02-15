/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <cbordeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:55:39 by cbordeau          #+#    #+#             */
/*   Updated: 2025/02/15 09:34:59 by cbordeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "Libft/libft.h"

int	main(int ac, char **av, char **env)
{
	int			pipefd[2];
	t_struct	args;
	int			i;

	i = -1;
	args = init_args(ac, av, env);
	if (args.here_doc)
		here_doc(&args);
	while (args.cmd < args.n_cmd - 1)
	{
		if (pipe(pipefd) == -1)
			(free(args.save_pid), fail_exit("pipe", args, pipefd, 0));
		args.save_pid[args.cmd] = fork();
		if (args.save_pid[args.cmd] == -1)
			(free(args.save_pid), fail_exit("fork", args, pipefd, 1));
		if (args.save_pid[args.cmd] == 0)
			child_process(args, pipefd);
		parent_process(&args, pipefd);
	}
	close(args.last_fd);
	while (++i < ac - 3 - args.here_doc)
		waitpid(args.save_pid[i], &(args.status), 0);
	free(args.save_pid);
	return (WEXITSTATUS(args.status));
}

void	parent_process(t_struct *args, int *pipefd)
{
	close(pipefd[1]);
	if (args->cmd != 0 || args->here_doc)
		close(args->last_fd);
	args->last_fd = pipefd[0];
	args->cmd += 1;
}

void	here_doc(t_struct *args)
{
	char	*s;
	int		here_doc[2];
	pid_t	pid;

	if (pipe(here_doc) == -1)
		(perror("pipe"), exit(FAILURE));
	args->last_fd = here_doc[0];
	pid = fork();
	if (pid == -1)
		fail_exit("fork", *args, here_doc, 1);
	if (pid == 0)
	{
		free(args->save_pid);
		close(here_doc[0]);
		s = get_next_line(0);
		while (ft_strncmp(s, args->limiter, max(s, args->limiter)))
		{
			write(here_doc[1], s, ft_strlen(s));
			(free(s), s = get_next_line(0));
		}
		(free(s), close(here_doc[1]), exit(EXIT_SUCCESS));
	}
	waitpid(pid, NULL, 0);
	close(here_doc[1]);
}

void	child_process(t_struct args, int *pipefd)
{
	free(args.save_pid);
	args.path_cmd = find_path(args, -1);
	if (!args.path_cmd)
		fail_exit("access", args, pipefd, 2);
	if (args.cmd == 0 && !args.here_doc)
	{
		args.in_fd = open(args.infile, O_RDONLY);
		if (args.in_fd < 0)
			(ft_freeall(&args.path_cmd), fail_exit("open", args, pipefd, 1));
	}
	else
		args.in_fd = args.last_fd;
	if (args.cmd == args.n_cmd - 2)
	{
		close(pipefd[1]);
		if (args.here_doc)
			args.out_fd = open(args.outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			args.out_fd = open(args.outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (args.out_fd < 0)
			(ft_freeall(&args.path_cmd), fail_exit("open", args, pipefd, 1));
	}
	else
		args.out_fd = pipefd[1];
	execute_cmd(args, pipefd);
}

void	execute_cmd(t_struct args, int *pipefd)
{
	char	**split;

	close(pipefd[0]);
	dup2(args.in_fd, STDIN_FILENO);
	dup2(args.out_fd, STDOUT_FILENO);
	close(args.in_fd);
	close(args.out_fd);
	split = ft_split(args.av[args.cmd], ' ');
	if (!split)
	{
		free(args.path_cmd);
		exit(127);
	}
	execve(args.path_cmd, split, args.env);
	perror("execve");
	free(args.path_cmd);
	ft_freeall(split);
	exit(FAILURE);
}
