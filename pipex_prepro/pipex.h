/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbordeau <cbordeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:59:30 by cbordeau          #+#    #+#             */
/*   Updated: 2025/01/22 11:32:15 by cbordeau         ###   LAUSANNE.ch       */
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

typedef struct s_struct
{
	int		ac;
	int		i;
	int		fd[2];
	int		here_doc;
	int		status;
	char	**av;
	char	**env;
	char	*path_cmd;
	pid_t	pid;
	pid_t	*save_pid;
	pid_t	last_fd;
}t_struct;

#endif
