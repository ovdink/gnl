/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgendry <sgendry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/06 16:20:07 by fmacgyve          #+#    #+#             */
/*   Updated: 2018/12/14 19:17:20 by sgendry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "get_next_line.h"
#include <stdio.h>

static int		ft_cut(t_file *file, char **line)
{
	char		*start;
	char		*tail;
	int 		i;

	i = 0;
	start = file->tail; // сохр указатель на первый символ
	if (*(file->tail) == '\n')
	{
		file->tail++; // увеличивакем указательн на файлтемп
		tail = ft_strdup(file->tail); // копируем то что после слеш Н
		file->tail = tail;
		ft_strdel(&start); // фришим что до этого
		*line = ft_strnew(0);
		return (1);
	}
	while (*(file->tail + i) != '\n' && *(file->tail + i)) // двигаемся по строке пока не найдем \n
		i++;
	file->tail = file->tail + i + 1; //записываем указатель после слеш Н
	tail = ft_strdup(file->tail); //копируем то что после слеш Н во временную переменную тейл
	file->tail = tail; //создали новый указатель в тейле
	tail = ft_strsub(start, 0, i); //выделили память отрезали до /n (индекса i)
	ft_strdel(&start); //от утечек
	*line = tail; //записываем тейл в лайн
	return (1);
}

static t_file	*create_file(int fd)
{
	t_file 		*new_lst;
	new_lst = (t_file *)malloc(sizeof(t_file));
	new_lst->fd = fd;
	new_lst->read_res = 0;
	new_lst->tail = ft_strnew(BUFF_SIZE);
	return (new_lst);
}	

static t_file	*find_fd(t_file **lst, int fd)
{
	t_file		*iter;
	t_file		*file;

	iter = *lst;
	if (!(lst) || !(*lst))
	{
		file = create_file(fd);
		*lst = file;
	}
	else
		{
			if (iter->fd == fd)
				return (iter);
			while (iter->next)
			{
				if (iter->fd == fd)
					return (iter);
				iter = (iter)->next;
			}
			//if (iter->fd == fd)
				//return (iter);
			file = create_file(fd);
			iter->next = file;
		}
		return (file);		
}


static void		read_more(t_file *file)
{
	char		*more_temp;
	char		*new_str;

	more_temp = ft_strnew(BUFF_SIZE); // Cоздаем пустую строку
	file->read_res = read(file->fd, more_temp, BUFF_SIZE); // Читаем в эту строку
	new_str = ft_strjoin(file->tail, more_temp); // Соединяем прошлый tail и только что прочитанную строку в новую строку
	ft_strdel(&more_temp); //Чистим вилкой временную переменную
	ft_strdel(&(file->tail)); //Чистим вилкой прошлый tail
	file->tail = new_str; // Приравниваем tail'у нашу новую строку
	if (!ft_strchr(file->tail, '\n') && file->read_res == BUFF_SIZE)
		read_more(file); // Рекурсивно читаем пока не наткнемся на /n или конец файла
}

int		get_next_line(const int fd, char **line)
{
	static t_file	*list = NULL;
	t_file				*file; //сохранять то что возьмем из листа
	if (fd < 0 || !line)
		return (-1);
	file = find_fd(&list, fd);
	if (file->read_res == 0 && !(file->tail))
		return (0);
	if (!ft_strchr(file->tail, '\n'))
		read_more(file);
	if (file->read_res == -1) //если пытаемся прочитать из закрытого файла-дескриптора
		return (-1);
	if (file->read_res == 0 && file->tail)
	{
		*line = ft_strdup(file->tail);
		ft_strdel(&(file->tail));
			return (1);
	}
	if (file->read_res == 0)
		return (0);
	return (ft_cut(file, line));
}

int			main(int argc, char **argv)
{
	char	*buf;
	int		fd;

	(void)argc;
	fd = open(argv[1], O_RDONLY);
	get_next_line(fd, &buf);
	ft_putendl(buf);
	while (get_next_line(fd, &buf))
	{
		ft_putstr(buf);
		printf("\n");
		free(buf);
	}
	//printf("%s", buf);
}