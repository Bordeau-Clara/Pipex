/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <cbordeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:59:30 by cbordeau          #+#    #+#             */
/*   Updated: 2025/02/07 09:26:51 by cbordeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
# include "Libft/libft.h"
# include "get_next_line.h"

# define FAILURE 1

typedef struct s_struct
{
	int		in_fd;
	int		out_fd;
	int		n_cmd;
	int		cmd;
	int		here_doc;
	int		status;
	char	**av;
	char	**env;
	char	*path_cmd;
	char	*infile;
	char	*outfile;
	char	*limiter;
	pid_t	*save_pid;
	pid_t	last_fd;
}t_struct;

void		parent_process(t_struct *args, int *pipefd);
void		here_doc(t_struct *args);
void		child_process(t_struct args, int *pipefd);
void		execute_cmd(t_struct args);

t_struct	init_args(int ac, char **av, char **env);
char		*find_path(t_struct args);
int			max(char *s, char *limiter);
void		fail_exit(char *message, t_struct args, int *pipefd, int mode);

#endif
