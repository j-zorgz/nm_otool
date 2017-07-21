#include <nm.h>

void			add_elem_start(t_data *data, t_symbole **list, uint64_t offset,
	size_t poids)
{
	t_symbole	*elem;

	elem = create_elem(data, offset, poids);
	*list = elem;
}

t_symbole		*add_elem_end(t_data *data, t_symbole *list, uint64_t offset,
	size_t poids)
{
	t_symbole	*elem;
	t_symbole	*fl;

	if (!(elem = create_elem(data, offset, poids)))
		return (NULL);
	if (!list)
		return (elem);
	fl = list;
	while (list->next != NULL)
		list = list->next;
	list->next = elem;
	return (fl);
}

void			delete_list(t_symbole *list)
{
	t_symbole	*prev;

	while (list)
	{
		if (list->str)
			free(list->str);
		prev = list;
		list = list->next;
		free(prev);
	}
}

static void		print_u(t_symbole *list)
{
	if (list->magic == MH_MAGIC_64)
		ft_printf("%016llx ", list->value);
	else if (list->magic == MH_MAGIC)
		ft_printf("%08llx ", list->value);
}

void			print_list(t_data *data, t_symbole *list)
{
	while (list)
	{
		if (!list->is_debug)
		{
			if (list->sym != 'U' && list->sym != 'u')
				print_u(list);
			else
			{
				if (list->magic == MH_MAGIC_64)
					ft_printf("%16c ", ' ');
				else if (list->magic == MH_MAGIC)
					ft_printf("%8c ", ' ');
			}
			ft_printf("%c ", list->sym);
			if (list->str)
				ft_printf("%s\n", list->str);
			else
				ft_printf("\n");
		}
		list = list->next;
	}
	data = NULL;
}
