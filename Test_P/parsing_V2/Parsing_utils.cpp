/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing_utils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Parsing.hpp"

/*
**	Parsing::delete_comm():
**		Delete all sentences starting with '#' from the std::string &line given.
**		Return line without the #'s lines.
*/
std::string				Parsing::delete_comm( std::string &line )		// Need to delete //
{
	std::string tmp(line);
	size_t			pos = 0;
	size_t			pos_end = 0;
	while (1)
	{
		pos = tmp.find("#", pos);
		if (pos == std::string::npos)
			break;
		pos_end = tmp.find("\n", pos);
		tmp.erase(pos , pos_end - pos);
	}
	return (tmp);
}





size_t			Parsing::_count_bloc_location( std::string &line ) const
{
	size_t count = 0;
	size_t pos = 0;

	size_t pos2 = 0;

	size_t size_serv = 0;
	size_t size_loc = 0;

	size_t count_loc = 0;

	std::string serv = "server";

	std::string bloc_server = "NULL";
	std::cout << "DANS COUNT BLOC LOCATION " << std::endl;
	while (1)
	{
		// on recupere un bloc server
		std::cout << "Pos = " << pos << std::endl;
		bloc_server = this->get_scope_v2(line, serv, pos);
		if (bloc_server == "")
			break;
		size_serv = bloc_server.size();
		//pos = 0;
		std::cout << "bloc_size = " << size_serv << " et bloc = " << bloc_server << "\n\n";
		pos2 = 0;
		while (1)  // on va compter les bloc locations
		{
			
			pos2 = bloc_server.find("location", pos2);
			if (pos2 == std::string::npos)
				break;
			// on a trouve location
			
			// on aumente
			pos2 = pos2 + 8;
			if (this->_ft_is_blank(bloc_server, pos2) == pos2)
			{
				std::cout << "pas de blanc";
				std::cout << "ERREUR addresse location 2" << std::endl;
				exit(EXIT_FAILURE);
			}
			else
			{
				std::cout << "il y a des blanc" << std::endl;
				pos2 = this->_ft_is_blank(bloc_server, pos2);
				if (bloc_server[pos2] != '/')
				{
					// '' l'addresse du bloc location ne commence pas par /
					std::cout << "ERREUR addresse location" << std::endl;
					exit(EXIT_FAILURE);
				}
				while (bloc_server[pos2] == '/' || isalpha((int)bloc_server[pos2]) || isdigit((int)bloc_server[pos2]))
					pos2++;
				// on vient de passer l'addresse du bloc location

				// on passe les espace et on trouve {
				if (this->_ft_is_blank(bloc_server, pos2) == pos2)
				{
					std::cout << " erreur addresse location 3pas de blanc 2";
					exit(EXIT_FAILURE);
				}
				else
				{
					std::cout << "il y a des blanc 2" << std::endl;
					pos2 = this->_ft_is_blank(bloc_server, pos2);
					if (bloc_server[pos2] != '{')
					{
						std::cout << "erreur doit debuter par {" << std::endl;
						exit(EXIT_FAILURE);
					}
					else
					{
						std::cout << "ON A UN BLOC SERVER" << std::endl;
						pos2 = bloc_server.find("}", pos2);
						count++;
					}
				}


			}
			std::cout << "pos2 = " << pos2 << " et bloc[pos2] = " << bloc_server[pos2] << std::endl;
			// break;
		}
		std::cout << "ICI count = " << count << std::endl;
		//break;
		if (pos2 == std::string::npos)
		{
			pos += bloc_server.size();
			std::cout << "la pos = " << pos << std::endl;
		}
		//break;
		
		
		// count++;
		// if (count > 4)
		// 	break;
	}
	std::cout << "FIN count = " << count << std::endl;
	std::cout << "BLOC SERVER = " << count << std::endl;
	return (count);
}



std::string		Parsing::get_scope_v2( std::string &line, std::string &name , size_t i) const
{
	size_t		y = i;
	int		open_scope = 0;
	size_t		pos_open = 0;
	size_t		pos_close = 0;
	size_t		diff = 0;

	size_t		pos = 0;
	//std::cout << " i = " << i << std::endl;
	pos = line.find(name, i);
	if (pos == std::string::npos)
	{
		std::cout << "Erreur get scope v2 pour le nom = " << name <<  " et i = " << i << std::endl;
		// changement attention
		return ("");
		exit(EXIT_FAILURE);
	}
	//std::cout << "DANS GET SCOPE pos = " << pos << std::endl; 
	y = pos;
	while (line[y])
	{
		if (line[y] == '{')
		{
			if (open_scope == 0)
				pos_open = y;
			open_scope++;
			
		}
		if (line[y] == '}')
		{
			open_scope--;
			if (open_scope == 0)
			{
				pos_close = y;
				break;
			}
		}
		y++;
	}
	//std::cout << "open_scope = " << open_scope << " et pos_open = " << pos_open << " et pos_close = " << pos_close << std::endl;
	return (std::string(line, pos_open, pos_close - pos_open + 1));
}

/*
**	Parsing::_count_bloc_server():

*/
size_t		Parsing::_count_bloc_server( std::string &line ) const
{
	size_t	count = 0;
	size_t	pos = 0;
	size_t	pos_n = 0;

	std::cout << std::endl;
	pos = line.find("server", pos);
	if (pos == std::string::npos)			// No server 
		throw Error(0, "Error, no server bloc in the configuration file!", 1);
	if (pos != 0)							// Server bloc don't start at the beginning of the file confifguration
		throw Error(1, "Error, configuration file should start by a bloc server!", 1);
	pos_n = line.find("\n", pos);
	if (line.find('{', pos) != pos_n - 1)
	{
		std::cout << "ERREUR dans count bloc server" << std::endl;
		exit(EXIT_FAILURE);
	}
	count++;
	if (this->_count_brackets(line) == 0)
	{
		pos = 0;
		pos += 6;		// on passe le premier
		while (1)
		{
			pos = line.find("server", pos);
			if (pos == std::string::npos)
				break;
			
			//std::cout << " 0 = line[pos] = " << line[pos] << " et pos = " << pos << std::endl;
			
			//pos += 6;
			if (line[pos - 1] == '/' && line[pos + 6] == '/') // cas des dossier erreur pages
			{
				//std::cout << "addresse = " << pos << std::endl;
				pos += 6;
				// pos++;
				// pos = line.find("server", pos);
			}
			else if (line.compare(pos, 11, "server_name") == 0)
			{
				//std::cout << " yes " << std::endl;
				pos += 11;
				//continue ;
			}
			else
			{
				
				pos += 6;
				pos_n = line.find("\n", pos);
				//std::cout << " 1 = line[pos] = " << line[pos] << " et pos = " << pos << std::endl;
				if (this->_ft_is_blank(line, pos) == pos)
				{
					//std::cout << "cas pas de blanc..." << std::endl;
					pos += 1; // 
				}
				else
				{

					//std::cout << "cas il y ades blancs" << std::endl;
					pos = this->_ft_is_blank(line, pos);	// on passe les blancs
					if (line[pos] == '{')
						count++;
					else
					{

						std::cout << "ERREUR truc entre server et { " << std::endl;
						exit(EXIT_FAILURE);
					}
				}
			}
			


			//std::cout << " 1 = line[pos] = " << line[pos] << " et pos = " << pos << std::endl;
			// if (this->_ft_is_blank(line, pos) == pos)
			// {
			// 	pos = pos_n;
			// 	//std::cout << "\t pos = " << pos << std::endl;
			// 	pos = line.find("server", pos);
			// 	if (pos == std::string::npos)
			// 		break;
			// 	pos_n = line.find("\n", pos);
			// 	pos += 6;
			// }
			// pos = this->_ft_is_blank(line, pos);
			// //std::cout << "2 = line[pos] = " << line[pos] << " et pos = " << pos << std::endl;
			// if (line[pos] != '{')
			// {
			// 	//std::cout << "3 = line[pos] = " << line[pos] << " et pos = " << pos << std::endl;
			// 	if (line[pos - 1 - 6] != '/')
			// 		throw Error(2, "Error, after 'server', should have '{'!", 1);
			// }
			// else
			// 	count++;
			// if (pos_n < pos)
			// 	throw Error(3, "Error, after 'server', should have '{' at the same line!", 1);
		}
	}

	std::cout << "\n\n FIN DE COUNT BLOC SERVER = " << count << std::endl;

	
	return (count);
}


/*
**	Parsing::ft_is_blank()
**		This method pass all blank character possible
**		It start at to_start and return pos.
*/
size_t				Parsing::_ft_is_blank( std::string &line, size_t to_start = 0) const
{
	size_t			pos = to_start;

	while (line[pos] == ' ' || line[pos] == '\t' || line[pos] == '\n'
			|| line[pos] == '\v' || line[pos] == '\f' || line[pos] == '\r')
		pos++;
	return (pos);
}

/*
**	Parsing::count_brackets():
**		Countn the open and close brackets.
**		If the number differs return 1 error. Otherwise return 0.
*/
bool				Parsing::_count_brackets( std::string &line) const
{
	size_t 			open_bracket = 0;
	size_t			close_brackets = 0;
	size_t			i = 0;

	while (line[i])
	{
		if (line[i] == '{')
			open_bracket++;
		if (line[i] == '}')
			close_brackets++;
		i++;
	}
	if (open_bracket == close_brackets)
		return (0);
	else
		throw Error(-1, "Error, general error in the configuration file!", 1);

}