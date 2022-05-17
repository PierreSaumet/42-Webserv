#include "../Headers/Parsing.hpp"
#include <iostream>

/*
**	void	Parsing::put_name_server( void )
**		This function put a random name if a server have no server_name.
*/
void		Parsing::put_name_server( void ) {

	char letter = 'a';
	for (std::vector<t_server>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
	{
		std::string name = "random_serv_";
		if (it->name_server.empty() == true)
		{
			name.push_back(letter);
			it->name_server = name;
			letter++;
		}		
	}
	return ;
}

/*
**	bool Parsing::ft_count_dot( std::string const host )
**		This function checks if there is 3 dots in the host given in the server's bloc
**	Returns true if there is no 3 dots, otherwise false.
*/
bool Parsing::ft_count_dot( std::string const host )
{
	size_t i = std::count(host.begin(), host.end(), '.');
	if (i != 3)
		return (true);
	return (false);
}

/*
**	bool Parsing::ft_check_host( std::string const host )
**		This function checks if the dots from the host are in a correct order.
**	Returns true if an error occurs, otherwise return false.
*/
bool Parsing::ft_check_host( std::string const host )
{
	size_t i = 0;

	if (host[0] == '.' || host[host.size()] == '.')
		return (true);

	while (i < host.size())
	{
		if (host[i] == '.' && host[i + 1] == '.')
			return (true);
		i++;
	}
	return (false);
}

/*
**	bool Parsing::all_digit(std::string string)
**		This functon checks if in the given string there is only digits.
**	Returns true if an error occurs, otherwise return false.
*/
bool Parsing::all_digit(std::string string)
{
	size_t i = 0;
	while (isdigit(string[i]) != 0)
		i++;
	if (i == string.size())
		return (false);
	return (true);

}