#include "libft.h"

int	ft_isnumeric(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	
	// Optionnel : autoriser un signe + ou - au début
	if (str[i] == '+' || str[i] == '-')
		i++;
	
	// Vérifier qu'il y a au moins un chiffre
	if (!str[i])
		return (0);
	
	while (str[i])
	{
		// MODIFICATION ICI : Autoriser aussi les espaces et retours à la ligne
		if (str[i] == '\n' || str[i] == '\r' || str[i] == ' ' || str[i] == '\t')
		{
			// Si c'est un espace/retour, vérifier qu'il n'y a plus de chiffres après
			i++;
			while (str[i])
			{
				if (ft_isdigit(str[i]))
					return (0); // Chiffre après espace = invalide
				i++;
			}
			return (1); // "100\n" est valide
		}
		
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}