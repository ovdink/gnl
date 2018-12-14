/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgendry <sgendry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/06 15:55:21 by sgendry           #+#    #+#             */
/*   Updated: 2018/12/14 16:53:00 by sgendry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE
# define GET_NEXT_LINE

#include "./libft/libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

# define BUFF_SIZE 	15

typedef struct		s_file
{
	int				fd;
	char			*tail; /* временная переменная */
	int				read_res; /* количество байт сколько прочитал в temp */
	struct s_file	*next;
}					t_file;

int					get_next_line(const int fd, char **line);

#endif