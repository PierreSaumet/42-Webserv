/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Parsing.hpp"

Parsing::Parsing( void ) : _name_of_file(NULL) {

	std::cout << "Empty Parsing constructor = not used" << std::endl;
	return ;
}

Parsing::Parsing( std::string configuration_file ) : _name_of_file(configuration_file) {

	std::cout << "Parsing constructor with argument" << std::endl;
	this->_name_of_file.push_back('\n');
	std::cout << this->get_name_of_file() << std::endl;

	std::cout << "\n START CHECKING FILES : " << std::endl;
	std::cout << "checking extension... : ";
	try
	{
		this->check_extension();
		std::cout << "OK" << std::endl;
		this->check_exist();
		std::cout << "OK" << std::endl;
		this->check_empty();
		std::cout << "OK" << std::endl;
	}
	catch (std::exception const &e)
	{
		//file.close();
		std::cerr << e.what() << std::endl;
		
	}

	std::cout << "Start PARSING = getting information server..." << std::endl;
	std::ifstream file(std::string(this->_name_of_file.begin(), this->_name_of_file.end() - 1));
	size_t 				pos;
	std::stringstream	buffer;
	std::string			line;
	try 
	{
		
		if (file.is_open())
		{
			
			buffer << file.rdbuf();
			line = buffer.str();
			std::cout << "AFFICHAGE LINE = " << line << std::endl;

			line = delete_comm(line);
			std::cout << " delete = " << line << std::endl;

			this->_ip_server = find_ip_port(line);
			std::cout << "IP_SERVER = " << this->_ip_server << std::endl;
			std::cout << "PORT = " << this->_port << std::endl;
			this->_name_server = find_name_server(line);
			std::cout << "NAME SERVER = " << this->_name_server << std::endl;
			this->_root = find_root(line);
			std::cout << "ROOT = " << this->_root << std::endl;
			this->_autoindex = find_autoindex(line);
			std::cout << "AUTOINDEX = " << this->_autoindex << std::endl;
			
			std::cout << std::endl;
			std::cout << std::endl;
			this->_error_page = find_error_page(line);
			std::cout << std::endl;
			

			std::cout << std::endl;
			if (check_code_error_file(line))
			{
				std::cout << "ERROR dans fichier erreur_parge"<< std::endl;
				exit(EXIT_FAILURE);
			}

			// this->_index = find_index(line);
			// std::cout << "index = " << this->_index << std::endl;

		}
		file.close();
		
	}
	catch (std::exception const &e)
	{
		file.close();
		//delete buffer;
		//free(buffer);
		std::cerr << e.what() << std::endl;
		
	}
	
	

	std::cout << "Start PARSING = getting information server..." << std::endl;
	//std::string tmp(this->_name_of_file.begin(), this->_name_of_file.end() - 1); 
	// std::ifstream file(std::string(this->_name_of_file.begin(), this->_name_of_file.end() - 1));
	// if (file.is_open())
	// {
		



		
		// // std::cout << "nbr locatiopn = " <<  this->count_location(line) << std::endl;
		// // std::cout << "nbr server = " << this->count_server(line) << std::endl;
		// // std::cout << "nbr error_page = " << this->count_error_page(line) << std::endl;
	
		// // test count sccope
		// std::cout << "scope = " << count_scope(line) << std::endl;
		// // test find scope
		// std::cout << "test scope server = " << get_scope(line) << std::endl;
		// std::string server = "server";
		// std::cout << "test scope server 2= " << get_scope_v2(line, server) << std::endl;
		// std::string location = "location";
		// std::cout << "test scope location 2= " << get_scope_v2(line, location) << std::endl;

	// }
	// else
	// {
	// 	std::cout << "CANNOT OPEN FILE" << std::endl;
	// 	exit(EXIT_FAILURE);
	// }

	
	//this->_ip_server = get_ip
	return ;
}

std::string		Parsing::find_client_max_size( std::string &line )
{
	size_t		pos = 0;
	size_t		pos_end = 0;
	size_t		pos_n = 0;
	std::string tmp;

	return tmp;
}

///////// ATTENTION ERRUER SI .htmlcca
std::string		Parsing::find_index( std::string &line )
{
	size_t		pos = 0;
	size_t		pos_end = 0;
	size_t		pos_n;
	std::string	tmp;

	pos = line.find("index", pos);
	if (pos == std::string::npos)
		throw Error(9, "Error, index not found", 1);
	std::cout << "pos = " << line[pos - 1] << std::endl;
	if (isalpha(line[pos - 1]))
	{
		std::cout << "oui" << std::endl;
		pos_end = line.find(";", pos);
		pos = pos_end;
		pos = line.find("index", pos);
		std::cout << "pos = " << line[pos] << std::endl;
		pos_end = 0;
	}
	
	pos = pos + 6;
	pos = line.find_first_not_of(" \t\n\v\r\f", pos);
	std::cout << "pos = " << line[pos] << std::endl;
	pos_end = line.find(";", pos);
	pos_n = line.find("\n", pos);
	
	if (pos_n <= pos_end)
		throw Error(99, "Error, need an ';' in configuration file", 1);
	
	tmp = line.substr(pos, pos_end - pos).c_str();
	std::cout << "tmp = " << tmp << std::endl;
	if (tmp.find(".html") == std::string::npos)
		throw Error(11, "Error, index should end by .html", 1);
	
	return (tmp);


	if (tmp.compare("127.0.0.1") != 0)
		throw Error(1, "Error, host should be 127.0.0.1", 1);

	//return (line.substr(pos, pos_port - pos).c_str());

}


std::string		Parsing::get_scope_v2( std::string &line, std::string &name ) const
{
	size_t		i = 0;
	int		open_scope = 0;
	size_t		pos_open = 0;
	size_t		pos_close = 0;
	size_t		diff = 0;

	size_t		pos = 0;
	pos = line.find(name, pos);
	if (pos == std::string::npos)
	{
		std::cout << "Erreur get scope v2 pour le nom" << std::endl;
		exit(EXIT_FAILURE);
	}
	i = pos;
	while (line[i])
	{
		if (line[i] == '{')
		{
			if (open_scope == 0)
				pos_open = i;
			open_scope++;
			
		}
		if (line[i] == '}')
		{
			open_scope--;
			if (open_scope == 0)
				pos_close = i;
		}
		i++;
	}
	std::cout << "open_scope = " << open_scope << " et pos_open = " << pos_open << " et pos_close = " << pos_close << std::endl;
	return (std::string(line, pos_open, pos_close - pos_open + 1));
}


/*
**	get the first scope
*/
std::string		Parsing::get_scope( std::string &line ) const
{
	size_t		i = 0;
	size_t		open_scope = 0;
	size_t		pos_open = 0;
	size_t		pos_close = 0;
	size_t		diff = 0;

	while (line[i])
	{
		if (line[i] == '{')
		{
			if (open_scope == 0)
				pos_open = i;
			open_scope++;
			
		}
		if (line[i] == '}')
		{
			open_scope--;
			if (open_scope == 0)
				pos_close = i;
		}
		i++;
	}
	std::cout << "open_scope = " << open_scope << " et pos_open = " << pos_open << " et pos_close = " << pos_close << std::endl;
	return (std::string(line, pos_open, pos_close - pos_open + 1));
}


/*
**	Parsing::count_scope():
**		Checks if there is no open scope or close scope
**		Return 0 if no problem and 1 if there is a problem
*/
bool			Parsing::count_scope( std::string &line ) const
{
	size_t			i = 0;
	size_t			open_scope = 0;
	size_t			close_scope = 0;
	while (line[i])
	{
		if (line[i] == '{')
			open_scope++;
		else if (line[i] == '}')
			close_scope++;
		i++;
	}
	if (open_scope == close_scope)
		return (false);
	else
		return (true);
}



Parsing::Parsing( const Parsing &copy ) : _name_of_file(copy._name_of_file) {
	std::cout << "Copy Parsing constructor = not used" << std::endl;
	return ;
}

Parsing::~Parsing( void ) {

	std::cout << "Parsing destructor " << std::endl;
	return ;
}

Parsing			&Parsing::operator=( const Parsing &element ) {

	if (this != &element)
	{
		this->_name_of_file = element._name_of_file;
	}
	return (*this);
}




/*
**	Other functions
*/

/*
**	find_ip_port()
**		Get the ip and the port from a std::string &line
**		The ip is a std::string and port an int.
**		Throw an error if "listen" is not find.
**		Otherwise return std::string ip_server;
*/
std::string			Parsing::find_ip_port( std::string &line )
{
	size_t 			pos = 0;
	size_t			pos_host = 0;
	size_t			pos_port = 0;
	size_t			pos_end = 0;
	size_t			pos_n = 0;
	std::string		tmp;

	pos = line.find("listen");
	if (pos == std::string::npos)
		throw Error(0, "Error, no listen in the configuration file", 1);
	if (line[pos - 1] != '\t')
		throw Error(1, "Error, bad listen in the configuration file", 1);
	pos = pos + 6;
	std::cout << "line[pos] = " << line[pos] << " et pos = " << pos << std::endl;
	// while (line[pos] == '\t' || line[pos] == ' ')
	// 	pos++;

	pos = ft_is_blank(line, pos);
	
	std::cout << "XXD line[pos] = " << line[pos] << " et pos = " << pos << std::endl;
	if (isdigit(line[pos]) == 0)
		throw Error(1, "Error, no host or bad host in the configuration file", 1);
	std::cout << "XXD line[pos] = " << line[pos] << " et pos = " << pos << std::endl;
	
	
	pos = line.find_first_not_of(" \t\n\v\r\f", pos);
	pos_host = line.find_first_of("0123456789", pos);;
	if (pos_host == std::string::npos)
		throw Error(1, "Error, no host in the configuration file", 1);
	pos_port = line.find_first_of(":", pos);
	if (pos_port == std::string::npos)
		throw Error(1, "Error, no host in the configuration file", 1);
	if (pos_host >= pos_port)
		throw Error(-1, "Error,  in the configuration file", 1);

	pos_end = line.find(";", pos);
	pos_n = line.find("\n", pos);
	
	if (pos_n <= pos_end)
		throw Error(99, "Error, need an ';' in configuration file", 1);
	this->_port = std::strtol(line.substr(pos_port + 1, pos_end - pos_port).c_str(), NULL, 10);
	if (this->_port < 0 || this->_port > 65535)
		throw Error(2, "Error, bad port. Should be between 0 and 65535", 1);
	
	tmp = line.substr(pos, pos_port - pos).c_str();
	if (tmp.compare("127.0.0.1") != 0)
		throw Error(1, "Error, host should be 127.0.0.1", 1);

	return (line.substr(pos, pos_port - pos).c_str());
}

size_t				Parsing::ft_is_blank( std::string &line, size_t to_start)
{
	size_t			pos = to_start;

	while (line[pos] == ' ' || line[pos] == '\t' || line[pos] == '\n'
			|| line[pos] == '\v' || line[pos] == '\f' || line[pos] == '\r')
		pos++;
	return (pos);
}

/*
**	find_name_server()
**		Get the name of the server from a std::string &line
**		The name is a std::string
**		Throw an error if "server_name" is not find.
**		Otherwise return std::string name_server;
*/
std::string			Parsing::find_name_server( std::string &line ) 
{
	size_t 				pos = 0;
	size_t				pos_end = 0;
	size_t				pos_n = 0;

	pos = line.find("server_name");
	if (pos == std::string::npos)
		throw Error(3, "Errer, no server_name", 1);
	// add 12 for -server_name -
	pos = pos + 12;
	pos = line.find_first_not_of(" \t\n\v\r\f", pos);
	pos_end = line.find(";", pos);
	pos_n = line.find("\n", pos);
	if (pos_n <= pos_end)
		throw Error(99, "Error, need an ';' in configuration file", 1);
	return (line.substr(pos, pos_end - pos).c_str());
}

/*
**	find_root()
**		Get the root of the server from a std::string &line
**		The name is a std::string
**		Throw an error if "server_name" is not find.
**		Otherwise return std::string name_server;
*/
std::string			Parsing::find_root( std::string &line )
{
	size_t 				pos = 0;
	size_t				pos_end = 0;
	size_t				pos_n = 0;
	std::string			tmp;
	struct stat 		buffer;

	pos = line.find("root");
	if (pos == std::string::npos)
		throw Error(4, "Error, no root name", 1);
	// add 5 for -root -
	pos = pos + 5;
	pos = line.find_first_not_of(" \t\n\v\r\f", pos);
	pos_end = line.find(";", pos);
	pos_n = line.find("\n", pos);
	if( pos_n <= pos_end)
		throw Error(99, "Error, need an ';' in configuration file", 1);

	tmp = line.substr(pos, pos_end - pos).c_str();
	tmp.insert(0, ".");
	if (stat(tmp.c_str(), &buffer) != 0)
		throw Error(20, "Error, cannot open directory", 1);
	return (line.substr(pos, pos_end - pos).c_str());
}


/*
**	find_autoindex()
**		Get the autoindex of the server from a std::string &line
**		The autoindex is a int
**		0 or 1;
*/
int				Parsing::find_autoindex( std::string &line )
{
	size_t 				pos = 0;
	size_t				pos_end = 0;
	size_t				pos_n = 0;
	pos = line.find("autoindex");
	if (pos == std::string::npos)
		throw Error(5, "Error, no autoindex", 1);
	// add 10 for -root -
	pos = pos + 10;
	pos = line.find_first_not_of(" \t\n\v\r\f", pos);
	pos_end = line.find(";", pos);
	pos_n = line.find("\n", pos);
	if( pos_n <= pos_end)
		throw Error(99, "Error, need an ';' in configuration file", 1);
	if (line.compare(pos, 3, "on;") == 0)	// oui
		return (1);
	else if (line.compare(pos, 4, "off;") == 0)
		return (0);
	else
		throw Error(9, "Error, autoindex not valid", 1);
}


/*
**	find_error_page()
**		Get all error_page of the server from a std::string &line
**		Return a map container
**		
*/
std::map<int, std::string>		Parsing::find_error_page( std::string &line )
{
	std::map<int, std::string>		tmp;

	size_t 				pos = 0;
	size_t				count = 0;
	size_t				nbr_length = -1;
	int					error_code = -1;
	size_t 				pos_end = 0;
	size_t				pos_n = 0;
	int 				i = 0;

	while (line[i])
	{
		if (line[i] == 'e')
		{
			if (line.compare(i, 10, "error_page") == 0)
			{
				count++;
				i = i + 10;
			}
		}
		i++;
	}
	std::cout << "count = " << count << std::endl;   // nombre de lignes avec error_page
	if (count == 0)
		throw Error(7, "Error, no error_page", 1);
	while ( count > 0)
	{
		pos = line.find("error_page", pos); // on trouve erreur_page
		pos = pos + 11;
		pos = line.find_first_not_of(" \t\n\v\r\f", pos);
		std::cout << "line[pos] = " << line[pos] << " et pos " << pos << std::endl;

		if (isdigit(line[pos]) != 0)
		{
			std::cout << " on trouve un chiffre" << std::endl;

			while (1)
			{
				error_code = std::strtol(&line[pos], NULL, 10);
				this->check_code_error(error_code);
				std::cout << "Error_code1 = " << error_code << std::endl;
				tmp.insert(std::pair<int, std::string>(error_code, "NULL"));
				pos = pos + 3;
				pos = line.find_first_not_of(" \t\n\v\r\f", pos);
				if (line[pos] == '.' || line[pos] == '/')
					break;
				std::cout << "line[pos] = " << line[pos] << " et pos " << pos << std::endl;
			}
		}
		if (line[pos] == '/' || line[pos] == '.')
		{
			if (error_code == -1)
				throw Error(8, "Error, error code wrong", 1);
			pos_end = line.find(";", pos);
			pos_n = line.find("\n", pos);
			if( pos_n <= pos_end)
				throw Error(99, "Error, need an ';' in configuration file", 1);
			std::string truc = line.substr(pos, pos_end - pos);
			std::cout << "truc = " << truc << std::endl;
			std::cout << "Error_code1 = " << error_code << std::endl;
			std::map<int, std::string>::iterator it_b;
			for (it_b = tmp.begin(); it_b != tmp.end(); it_b++)
			{
				if (it_b->second == "NULL")
					it_b->second = truc;
			}
			count--;
		}	
		std::cout << "PD =" << count << std::endl;
	}

	std::cout << "Affiche les erreurs :" << std::endl;
	std::map<int, std::string>::iterator it_b = tmp.begin();
	std::map<int, std::string>::iterator it_e = tmp.end();
	for (it_b = tmp.begin(); it_b != it_e; it_b++)
	{
		std::cout << "1 = " << it_b->first << " et " << it_b->second << std::endl;
	}


	if (count == 0)
		return (tmp);
	else
		throw Error(10, "Error, code_error no conform", 1);
	// return (tmp);
}



size_t		Parsing::count_server( std::string &line ) const
{
	size_t	count = 0;
	int		i = 0;

	size_t count_open_scope = 0;
	size_t count_end_scope = 0;
	while (line[i])
	{
		if (line[i] == 's')
		{
			if (line.compare(i, 6, "server") == 0)
			{
				int y = i;
				while (line[y])
				{
					size_t pos = 0;
					if ((pos = line.find("{", y)) != std::string::npos)
						count_open_scope++;
					//size_t pos = line.find("{", y);
					
					if (pos == std::string::npos)
						break;
					//else
					//	count_open_scope++;
					size_t pos2 = line.find("}", y);
					if (pos2 == std::string::npos)
						break;
					else
						count_end_scope++;
					//std::cout << "pos = " << pos << std::endl;
					y++;
				}
				count++;
				i = i + 6;
			}
		}
		i++;
	}
	std::cout << "scope_open = " << count_open_scope << std::endl;
	std::cout << "scope_end = " << count_end_scope << std::endl;
	return (count);
}

size_t		Parsing::count_error_page( std::string &line ) const
{
	size_t	count = 0;
	int		i = 0;
	while (line[i])
	{
		if (line[i] == 'e')
		{
			if (line.compare(i, 10, "error_page") == 0)
			{
				count++;
				i = i + 10;
			}
		}
		i++;
	}
	return (count);
}

size_t		Parsing::count_location( std::string &line ) const
{
	size_t	count = 0;
	int		i = 0;
	while (line[i])
	{
		if (line[i] == 'l')
		{
			if (line.compare(i, 8, "location") == 0)
			{
				count++;
				i = i + 8;
			}
		}
		i++;
	}
	return (count);
}