/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_lib.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcamhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 15:36:47 by jcamhi            #+#    #+#             */
/*   Updated: 2017/07/24 15:36:49 by jcamhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <otool.h>

static void		handle_obj_sl(t_data *data, uint32_t offset, uint32_t max)
{
	struct ar_hdr	*hdr;
	void			*obj;

	hdr = data->binary + offset;
	obj = (void*)(hdr + 1);
	while (offset <= max)
	{
		ft_printf("%s(%s):\n", data->av, hdr + 1);
		obj = (void*)(hdr + 1);
		data->magic = *(uint32_t*)(obj);
		while (data->magic != MH_MAGIC_64 && data->magic != MH_MAGIC)
		{
			obj = (void*)obj + 1;
			data->magic = *(uint32_t*)(obj);
		}
		if (data->magic == MH_MAGIC_64)
			handle_64(data, obj - data->binary);
		else if (data->magic == MH_MAGIC)
			handle_32(data, obj - data->binary);
		offset += ft_atoi(hdr->ar_size) + sizeof(struct ar_hdr);
		hdr = data->binary + offset;
	}
}

uint32_t		find_first_obj(t_data *data, uint32_t offset)
{
	void	*obj;

	obj = data->binary + offset;
	while (!ft_strnequ(obj, "  `\n", 4))
	{
		obj++;
	}
	return (obj + 4 - data->binary - sizeof(struct ar_hdr));
}

static int		handle_weird_lib(t_data *data, struct ranlib *symtab)
{
	uint32_t		min;

	min = find_first_obj(data, (void*)symtab - data->binary);
	handle_obj_sl(data, min, data->end - 1);
	return (1);
}

void			handle_static_lib(t_data *data, uint32_t offset)
{
	struct ranlib	*symtab;
	struct ranlib	*start;
	uint32_t		*nbr;
	uint32_t		min;
	uint32_t		max;

	nbr = (uint32_t*)(data->binary + offset + SARMAG + sizeof(struct ar_hdr)
		+ sizeof(char[20]));
	symtab = data->binary + offset + SARMAG + sizeof(struct ar_hdr)
		+ sizeof(char[20]) + sizeof(uint32_t);
	start = symtab;
	min = 0;
	max = 0;
	if (*nbr == 0 && handle_weird_lib(data, symtab))
		return ;
	while ((void*)symtab < (void*)start + *nbr)
	{
		if (min == 0 || symtab->ran_off < min)
			min = symtab->ran_off;
		if (symtab->ran_off > max)
			max = symtab->ran_off;
		symtab++;
	}
	handle_obj_sl(data, min + offset, max + offset);
}
