#include "Headers/Parsing.hpp"
#include <iostream>

/*
**	ft_delete_comm( std::string &line ):
**		This function will delete all commentaries (starting with #) from the configuration file.
**
**	==> Returns the new string "tmp" which will contain all the configuration file.
*/
std::string                     Parsing::ft_delete_comm( std::string &line )
{
	std::string                 tmp = line;
	size_t                      pos = 0;
	size_t                      pos_end = 0;
	while (1)
	{
		pos = tmp.find('#', pos);
		if (pos == std::string::npos)
			break;
		pos_end = tmp.find('\n', pos);
		tmp.erase(pos, pos_end - pos);
	}
	return (tmp);
}
