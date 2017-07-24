/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcamhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 15:35:52 by jcamhi            #+#    #+#             */
/*   Updated: 2017/07/24 15:35:53 by jcamhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm.h>

void	*convert_chunk(void *binary, size_t size)
{
	char		*ret;
	char		*bc;
	char		*tmp;
	size_t		i;

	ret = (char*)malloc(size);
	bc = (char*)binary;
	tmp = ret;
	i = 0;
	while (i < size)
	{
		*(tmp + 0) = *(bc + 3);
		*(tmp + 1) = *(bc + 2);
		*(tmp + 2) = *(bc + 1);
		*(tmp + 3) = *(bc + 0);
		tmp += 4;
		bc += 4;
		i += 4;
	}
	return (ret);
}

void	handle_fat_arch(t_data *data, struct fat_arch *arch, size_t poids,
	t_opt opt)
{
	data->magic = *(uint32_t*)(data->binary + arch->offset);
	if (arch->cputype == CPU_TYPE_I386)
	{
		if (ft_strnequ((char*)data->binary + arch->offset, ARMAG, SARMAG))
		{
			data->end = arch->offset + arch->size;
			handle_static_lib(data, arch->offset, opt);
		}
		else
			handle_32(data, arch->offset, poids);
	}
	else if (arch->cputype == CPU_TYPE_X86_64)
	{
		if (ft_strnequ((char*)data->binary + arch->offset, ARMAG, SARMAG))
		{
			data->end = arch->offset + arch->size;
			handle_static_lib(data, arch->offset, opt);
		}
		else
			handle_64(data, arch->offset, poids);
	}
}

void	handle_fat_cigam(t_data *data, t_opt opt)
{
	void				*data_cigam;
	struct fat_header	*header;
	struct fat_arch		*arch;
	size_t				i;
	size_t				nbr;

	data_cigam = convert_chunk(data->binary, sizeof(struct fat_header));
	header = (struct fat_header*)data_cigam;
	i = 0;
	nbr = header->nfat_arch;
	free(data_cigam);
	data_cigam = convert_chunk(data->binary + sizeof(struct fat_header),
			nbr * sizeof(struct fat_arch));
	while (i < nbr)
	{
		arch = (struct fat_arch*)data_cigam + i;
		if (arch->cputype == CPUTYPE)
		{
			handle_fat_arch(data, arch, i, opt);
			free(data_cigam);
			return ;
		}
		i++;
	}
}
