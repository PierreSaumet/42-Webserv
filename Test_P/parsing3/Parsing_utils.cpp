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



/*
**	ft_get_scope( size_t index ):
**		This function will return the scope starting and the given 'index' argument.
**
**	==> It will return a std::vector<std::string> which contains the scope.
*/
std::vector<std::string>        Parsing::ft_get_scope( size_t index )
{
	std::vector<std::string>::iterator      it_b;
	std::vector<std::string>::iterator      it_e;
	std::vector<std::string>::size_type 	size_data = this->_data.size();
	size_t                  				i = index;
	size_t                  				count = 0;

	while (i++ < size_data)
	{
		if (this->_data[i] == "{")
		{
			if (count == 0)
				it_b = this->_data.begin() + i;
			count++;
		}
		if (this->_data[i] == "}" && count != 0)
		{
			count--;
			if (count == 0)
			{
				it_e = this->_data.begin() + i + 1;
				break;
			}
		}
	}
	std::vector<std::string>    tmp(it_b, it_e);
	return (tmp);
}