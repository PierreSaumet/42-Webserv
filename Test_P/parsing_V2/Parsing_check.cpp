/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing_check.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Parsing.hpp"


bool		Parsing::_check_bloc_server( std::string &line ) const
{
	size_t		nbr_serv = this->get_nbr_server();
	size_t		pos = 0;
	size_t		tmp_size = 0;
	std::string	tmp;
	std::string name = "server";
	while (nbr_serv > 0)
	{
		// pos = line.find("server", pos);
		pos = tmp_size;
		//std::cout << "ICI, pos = " << pos << std::endl;

		tmp = this->get_scope_v2(line, name, pos);
		tmp_size = tmp.size();
		//std::cout << "\ntmp = " << tmp << std::endl;
		pos = 0;
		while (tmp[pos])
		{
			
			pos = tmp.find("server", pos);
			if (pos == std::string::npos)
			{
				//std::cout << "Good pas de server " << std::endl;
				break;
			}
			else
			{
				//std::cout << "pos = " << pos << " et tmp[pos] = " << tmp[pos] << std::endl;
				pos += 6;
				if (this->_ft_is_blank(tmp, pos) != pos)
				{
					pos = this->_ft_is_blank(tmp, pos);
					if (tmp[pos] == '{')
						throw Error(4, "Error, a bloc server cannot have another bloc server inside!", 1);
				}
			}
		}
		nbr_serv--;
	}
	return (false);
}



/*
**	Parsing::check_configuration_file():
**		Checks if the configuration file is valid.
**		Return 0 if no error occurs
**		Throw an Error() if there is an error.
*/
bool			Parsing::check_configuration_file( void ) const {

	std::ifstream		fs;
	std::string			tmp_name(this->_name_of_file.begin(), this->_name_of_file.end() - 1);

	fs.open(tmp_name);
	if (fs.is_open() == 0)		// File doesn't exist
	{
		fs.close();
		throw Error(0, "Error, configuration file doesn't exist!", 0);
	}
	else						// File exist
	{
		std::cout << "ici" << std::endl;
		if (fs.peek() == std::ifstream::traits_type::eof())		// File is empty
		{
			fs.close();
			throw Error(1, "Error, configuration file is empty!", 0);
		}
		else					// File extension
		{
			std::string			to_find = ".conf";
			size_t				pos_to_find = this->_name_of_file.find(to_find, 0);
			if (pos_to_find == std::string::npos)				// Cannot find .conf
			{
				fs.close();
				throw Error(2, "Error, configuration should end by '.conf'!", 0);
			}
			else
			{
				if (this->_name_of_file.compare(pos_to_find, 6, ".conf\n") != 0)	// Should have only one '.conf'
				{
					fs.close();
					throw Error(2, "Error, configuration should end by '.conf'!", 0);
				}
			}
		}
	}
	fs.close();
	return (false);
}




bool					Parsing::check_code_error( int code ) const
{
	// if (server_code_error(code) == 1 || client_code_error(code) == 1)
	// 	return (1);
	// else
	// 	return (0);
	if (code > 399 && code < 500)
		return (client_code_error(code));
	else if (code > 499 && code < 600)
		return (server_code_error(code));
	else
	{
		throw Error(8, "Error, code error not correct", 1);
		// std::cout << "ERROR CODE PAS VALIDE" << std::endl;
		// exit(EXIT_FAILURE);
	}
	//return ( client_code_error(code));
}

/*
**	Parsing::server_code_error():
**		Check iff the given code is a server code.
**		if not, return 1.
*/
bool					Parsing::server_code_error( int code ) const
{
	if ((code >= 500 && code <= 508) || code == 510 || code == 511)
		return (false);
	else
		return (true);
}

/*
**	Parsing::client_code_error():
**		Check if the given code is a client code.
**		If not, return 1;
*/
bool					Parsing::client_code_error( int code ) const
{
	if ((code >= 400 && code <= 417) || (code >= 421 && code <= 426) 
			|| code == 428 || code == 429 || code == 431 || code == 451 )		// || code == 499 pour NGINX
		return (false);
	else
		return (true);
}




/*
**	Parsing::check_file_and_empty_exit()
**		Check if the given string (line), allows to open a file and checks if it is
**		empty.
**		If an error occurs, return 1, otherwise 0.
*/
bool					Parsing::check_file_and_empty_exit( std::string &line ) const
{
	std::fstream			fs;
	fs.open(line);
	if (fs.is_open() == 0)
	{
		fs.close();
		std::cout << "file dosent exist : " << line << std::endl;
		//exit(EXIT_FAILURE);
		return (true);
	}
	else
	{
		std::cout << "\t= ) file exist " << line << std::endl;
		if (fs.peek() == std::fstream::traits_type::eof())
		{
			std::cout << "Error, file is empty : " << line << std::endl;
			return (true);
		}
		fs.close();
	}
	return (false);
}
