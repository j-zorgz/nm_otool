#include <otool.h>

void	do_otool_on_archive(const char *file, t_data *data)
{
	ft_printf("Archive : %s\n", file);
	handle_static_lib(data, 0);
}

void	do_otool(const char *file)
{
	t_data	data;

	data.binary = map_binary(file);
	if (!data.binary)
		return ;
	data.av = file;
	data.magic = *(uint32_t*)(data.binary);
	if (data.magic == MH_MAGIC_64)
	{
		ft_printf("%s:\n", file);
		handle_64(&data, 0);
	}
	else if (data.magic == MH_MAGIC)
	{
		ft_printf("%s:\n", file);
		handle_32(&data, 0);
	}
	else if (data.magic == FAT_CIGAM)
	{
		handle_fat(&data);
	}
	else if (ft_strnequ((char*)data.binary, ARMAG, SARMAG))
		do_otool_on_archive(file, &data);
}

int		main(int ac, char **av)
{
	int		i;

	if (ac == 1)
		return (0);
	else
	{
		i = 1;
		while (i < ac)
		{
			if (ac > 2)
				ft_printf("\n%s:\n", av[i]);
			do_otool(av[i]);
			i++;
		}
	}
	return (0);
}
