#include "philo.h"

bool	parsing(int argc, char **argv)
{
	int	i;
	int	value;

	i = 1;
	while (i < argc)
	{
		if (!is_digit(argv[i]))
			return (false);
		value = ft_atoi(argv[i]);
		if (i == 1)
		{
			if (value <= 0)
				return (false);
		}
		else
		{
			if (value < 0)
				return (false);
		}
		i++;
	}
	return (true);
}

int	main(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		printf("Error : invalid number of arguments.\n");
		return (1);
	}
	if (!parsing(argc, argv))
	{
		printf("Error : invalid arguments.\n");
		return (1);
	}
	init_table(argc, argv);
	return (0);
}
