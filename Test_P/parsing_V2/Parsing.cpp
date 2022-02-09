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

Parsing::Parsing( void ) : _name_of_file(NULL), _nbr_server(0) {

	std::cout << "Empty Parsing constructor = not used" << std::endl;
	return ;
}

Parsing::Parsing( std::string config_file ) : _name_of_file(config_file), _nbr_server(0) {

	

	std::cout << "Parsing constructor with argument" << std::endl;
	this->_name_of_file.push_back('\n');
	try {
		this->check_configuration_file();

	}
	catch (std::exception const &e)
	{
		std::cerr << e.what() << std::endl;
	}
	std::ifstream file(std::string(this->_name_of_file.begin(), this->_name_of_file.end() - 1));
	size_t 				pos = 0;;
	std::stringstream	buffer;
	std::string			line;
	try {
		if (file.is_open())
		{
			buffer << file.rdbuf();
			line = buffer.str();
			line = delete_comm(line);
			this->_server = this->ft_initialize(line);

			// On cree le nombre de bloc server
			std::cout << "\n nbr server = " << this->_nbr_server << std::endl;
			this->_server = this->init_serv_struct( line );
		}
	}
	catch (std::exception const &e)
	{
		file.close();
		std::cerr << e.what() << std::endl;
	}
	file.close();



	std::cout << "On initie notre premier vector de server avec tout a null" << std::endl;
	// this->_server = this->ft_initialize();
	//std::cout << "test = " << this->_server[0]._host_server << std::endl;

	// exit(1);

	// std::cout << "Start PARSING = getting information server..." << std::endl;
	// std::ifstream file(std::string(this->_name_of_file.begin(), this->_name_of_file.end() - 1));
	// size_t 				pos;
	// std::stringstream	buffer;
	// std::string			line;
	// try {
	// 	if (file.is_open())
	// 	{
	// 		buffer << file.rdbuf();
	// 		line = buffer.str();

	// 		line = delete_comm(line); 			// Delete commentaries
	// 		//std::cout << " delete = " << line << std::endl;

			
	// 		this->_nbr_server = this->_count_bloc_server(line);

	// 		std::cout << "Count block server = " << this->_nbr_server << std::endl;

	// 		this->_check_bloc_server(line);
	// 		std::cout << "Bloc server are correct" << std::endl;

	// 		// On cree le nombre de bloc server
	// 		// std::cout << "nbr server = " << this->_nbr_server << std::endl;
	// 		// this->_server = this->init_serv_struct( line );

	// 		//std::cout << "BLOC lcoation = " << this->_count_bloc_location( line ) << std::endl;
	// 		// this->_nbr_location = this->_count_bloc_location( line );
	// 		// std::cout << "Count all bloc location = " << this->_nbr_location << std::endl;

	// 		// On cree le nombre de bloc server
	// 		// std::cout << "nbr server = " << this->_nbr_server << std::endl;
	// 		this->_server = this->init_serv_struct( line );
	// 		// if (this->check_code_error_file())
	// 		// 	throw Error(45, "Error, error_page is wrong!", 1);
	// 		// int i = 0;
	// 		// while (i < this->_nbr_server)
	// 		// {
	// 		// 	std::cout << "\n\n SERVER = " << this->_nbr_server << std::endl;
	// 		// 	std::cout << "\n";
	// 		// 	std::cout << "host = " << this->_server[i]._host_server << std::endl;
	// 		// 	std::cout << "port = " << this->_server[i]._port_server << std::endl;
	// 		// 	std::cout << "name serv = " << this->_server[i]._name_server << std::endl;
	// 		// 	std::cout << "root = " << this->_server[i]._root_server << std::endl;
	// 		// 	std::cout << "autoindex = " << this->_server[i]._autoindex_server << std::endl;
	// 		// 	for (std::map<int, std::string>::iterator beg = this->_server[i]._error_server.begin(); beg != this->_server[i]._error_server.end(); beg++ )
	// 		// 	{
	// 		// 		std::cout << "beg->first = " << beg->first << " et = " << beg->second << std::endl;
	// 		// 	}
			
				
	// 		// 	std::cout << "\n";
	// 		// 	i++;
	// 		// }
	// 		// std::map<int, std::string>::iterator beg = this->_server[0]._error_server.begin();

			


	// 	}
	// }
	// catch (std::exception const &e)
	// {
	// 	file.close();
	// 	std::cerr << e.what() << std::endl;
	// }
	// file.close();

}

std::vector<Parsing::t_server>		Parsing::ft_initialize( std::string &line )
{
	std::vector<Parsing::t_server> tmp;

	tmp.push_back(Parsing::t_server());
	tmp[0]._host_server = "NULL";
	tmp[0]._port_server = -1;
	tmp[0]._name_server = "NULL";
	tmp[0]._root_server = "NULL";
	tmp[0]._autoindex_server = false;
	// tmp[0]._error_server = "NULL";
	tmp[0]._client_size = "NULL";
	// tmp[0]._methods_server
	tmp[0]._index_server = "NULL";
	tmp[0]._cgi_path_server = "NULL";
	tmp[0]._upload_path_server = "NULL";
	this->_nbr_server = this->_count_bloc_server(line);
	this->_check_bloc_server(line);
	std::cout << "Bloc server are correct" << std::endl;

	this->_nbr_location = this->_count_bloc_location(line);
	// compter les blocs locatiobns
	return (tmp);
}

Parsing::Parsing( const Parsing &copy ) : _name_of_file(copy._name_of_file), _nbr_server(copy._nbr_server)
{
	// 		_server(copy._server
	std::cout << "Copy Parsing constructor = not used" << std::endl;
	return ;
}

Parsing::~Parsing( void ) {

	std::cout << "Parsing destructor " << std::endl;
	// delete this->_server->location;
	// delete this->_server;
	return ;
}

Parsing			&Parsing::operator=( const Parsing &element ) {

	if (this != &element)
	{
		this->_name_of_file = element._name_of_file;
		this->_nbr_server = element._nbr_server;
		//this->_server = element._server;
	}
	return (*this);
}

std::vector<Parsing::t_server>		Parsing::init_serv_struct( std::string &line )
{
	std::vector<Parsing::t_server> tmp;

	size_t pos = 0;

	int i = 0;

	int y = 0;
	size_t count_tmp = this->_nbr_server;
	std::cout << "nbr_serv = " << count_tmp << std::endl;

	std::string serv = "server";
	std::string bloc = "NULL";
	size_t size_bloc = 0;
	std::cout << "\n\n\n debut grosse boucle \n\n";
	std::cout << "TOTAL LINE = " << line.size() << std::endl;
	while (count_tmp > 0)
	{
		tmp.push_back(Parsing::t_server());
		std::cout << "la pos = " << pos << std::endl;
		bloc = this->get_scope_v2(line, serv, i);	// on recupere le scope server
		pos = 0;
		size_bloc = bloc.size();						// on prend la taille du scope
		std::cout << "size_bloc = " << size_bloc << std::endl;
		std::cout << "bloc = " << bloc << std::endl;

		if (bloc.find("listen", pos) == std::string::npos)
			pos = 0;
		pos = this->find_ip_port(bloc, tmp, y, pos);
		//std::cout << "host = " << tmp[i]._host_server << std::endl;


		if (bloc.find("server_name", pos) == std::string::npos)
			pos = 0;
		pos = find_name_server(bloc, tmp, i, pos);
		//std::cout << "name serv = " << tmp[i]._name_server << std::endl;


		// if (bloc.find("root", pos) == std::string::npos)
		// 	pos = 0;
		// pos = find_root(bloc, tmp, i, pos);
		// std::cout << "root = " << tmp[i]._root_server << std::endl;


		// if (bloc.find("autoindex", pos) == std::string::npos)
		// 	pos = 0;
		// pos = find_autoindex(bloc, tmp, i, pos);

		// if (bloc.find("error_page", pos) == std::string::npos)
		// 	pos = 0;
		// pos = find_error_page(bloc, tmp, i, pos);


		y++;
		
		i += size_bloc;
		std::cout << " i = " << i << std::endl;
		pos = 0;
		std::cout << "\tnbr_serv = " << count_tmp << std::endl;
		count_tmp--;
		std::cout << "\tnbr_serv = " << count_tmp << std::endl;

		// if (count_tmp == 1)
		// 	break;
	}
	std::cout << "nbr_serv = " << count_tmp << std::endl;

	
	// std::vector<Parsing::t_server> ite_b;
	i = 0;
	while (i < this->_nbr_server)
	{
		std::cout << "\n";
		std::cout << "host = " << tmp[i]._host_server << std::endl;
		std::cout << "port = " << tmp[i]._port_server << std::endl;
		std::cout << "name serv = " << tmp[i]._name_server << std::endl;
		std::cout << "root = " << tmp[i]._root_server << std::endl;
		std::cout << "autoindex = " << tmp[i]._autoindex_server << std::endl;
		std::cout << "\n";
		i++;
	}
	return (tmp);
}


/*
**	Parsing::check_code_error_file()
**		Check if the file for the error_code given in the configuration file
**		is correct and exist. It use std::map<int, std::string> _error_page
**		If there is a problem return 1 otherwise 0.
*/
bool					Parsing::check_code_error_file( void ) // Need to delete //
{
	int i = 0;
	while (i < this->_nbr_server)
	{
		size_t pos = 0;
		std::string get_code;
		for (std::map<int, std::string>::iterator beg = this->_server[i]._error_server.begin(); beg != this->_server[i]._error_server.end(); beg++ )
		{
			std::cout << "beg->first = " << beg->first << " et = " << beg->second << std::endl;
			//break;
			pos = beg->second.find(".html", pos);
			if (pos == std::string::npos)
			{
				std::cout << "kek " << beg->second << std::endl;
				//break
				get_code = std::to_string(beg->first);
				//std::cout << "get code = " << get_code << std::endl;
				get_code.append(".html");
				//std::cout << "get code = " << get_code << std::endl;
				beg->second.append(get_code);
				//std::cout << "du coup = " << it_b->second << std::endl;
				if (check_file_and_empty_exit(beg->second) == 1)
					return (1);
				
			}
			else	// l'addresse du fichier possede .html
			{
				//std::cout << "good" << std::endl;
				size_t pos = 0;
				pos = beg->second.find(std::to_string(beg->first), pos);
				if (pos == std::string::npos)
				{
					std::cout << "CACA" << std::endl;
					size_t pos_back = beg->second.find_last_of("/", pos);
					size_t pos_dot = beg->second.find_last_of(".", pos);
					pos = 0;
					std::cout << "pos_b " << pos_back << " et " << pos_dot << std::endl;
					if (pos_dot - pos_back == 4)
					{
						std::cout << "YES" << std::endl;
						beg->second.replace(pos_back + 1, 3, std::to_string(beg->first));
					}
					std::cout << "it_first = " << beg->first << " second = " << beg->second << std::endl;
				}
				std::cout << "pos = " << pos << " et it_fsit = " << beg->first << std::endl;
				//if (std::to_string(it_b->first).)
				if (check_file_and_empty_exit(beg->second) == 1)
					return (1);
	
				
			}
			pos = 0;
		}
		i++;
	}

	//on parcourt toutes les error
	// std::map<int, std::string>::iterator it_b = this->_server->_error
	// std::map<int, std::string>::iterator it_e = this->_error_page.end();
	// size_t pos = 0;
	// std::string get_code;
	// for (it_b = this->_error_page.begin(); it_b != it_e; it_b++)
	// {
	// 	//std::cout << it_b->first << " => " << it_b->second << std::endl;
	// 	pos = it_b->second.find(".html", pos);
	// 	if (pos == std::string::npos)
	// 	{
	// 		std::cout << "kek " << it_b->second << std::endl;
	// 		//break
	// 		get_code = std::to_string(it_b->first);
	// 		//std::cout << "get code = " << get_code << std::endl;
	// 		get_code.append(".html");
	// 		//std::cout << "get code = " << get_code << std::endl;
	// 		it_b->second.append(get_code);
	// 		//std::cout << "du coup = " << it_b->second << std::endl;
	// 		if (check_file_and_empty_exit(it_b->second) == 1)
	// 			return (1);
	// 		// std::fstream			fs;
	// 		// fs.open(it_b->second);
	// 		// if (fs.is_open() == 0)
	// 		// {
	// 		// 	fs.close();
	// 		// 	std::cout << "file dosent exist : " << it_b->second << std::endl;
	// 		// 	//exit(EXIT_FAILURE);
	// 		// }
	// 		// else
	// 		// {
	// 		// 	std::cout << "\t= ) file exist " << it_b->second << std::endl;
	// 		// 	if (fs.peek() == std::fstream::traits_type::eof())
	// 		// 	{
	// 		// 		std::cout << "Error, file is empty : " << it_b->second << std::endl;
	// 		// 	}
	// 		// 	fs.close();
	// 		// }
	// 	}
	// 	else	// l'addresse du fichier possede .html
	// 	{
	// 		//std::cout << "good" << std::endl;
	// 		size_t pos = 0;
	// 		pos = it_b->second.find(std::to_string(it_b->first), pos);
	// 		if (pos == std::string::npos)
	// 		{
	// 			std::cout << "CACA" << std::endl;
	// 			size_t pos_back = it_b->second.find_last_of("/", pos);
	// 			size_t pos_dot = it_b->second.find_last_of(".", pos);
	// 			pos = 0;
	// 			std::cout << "pos_b " << pos_back << " et " << pos_dot << std::endl;
	// 			if (pos_dot - pos_back == 4)
	// 			{
	// 				std::cout << "YES" << std::endl;
	// 				it_b->second.replace(pos_back + 1, 3, std::to_string(it_b->first));
	// 			}
	// 			std::cout << "it_first = " << it_b->first << " second = " << it_b->second << std::endl;
	// 		}
	// 		std::cout << "pos = " << pos << " et it_fsit = " << it_b->first << std::endl;
	// 		//if (std::to_string(it_b->first).)
	// 		if (check_file_and_empty_exit(it_b->second) == 1)
	// 			return (1);
	// 		// std::fstream			fs;
	// 		// fs.open(it_b->second);
	// 		// if (fs.is_open() == 0)
	// 		// {
	// 		// 	fs.close();
	// 		// 	std::cout << "file dosent exist : " << it_b->second << std::endl;
	// 		// 	//exit(EXIT_FAILURE);
	// 		// }
	// 		// else
	// 		// {
	// 		// 	std::cout << "\t= ) file exist " << it_b->second << std::endl;
	// 		// 	if (fs.peek() == std::fstream::traits_type::eof())
	// 		// 	{
	// 		// 		std::cout << "Error, file is empty : " << it_b->second << std::endl;
	// 		// 	}
	// 		// 	fs.close();
	// 		// }
			
	// 	}
	// 	pos = 0;

	// }	
	
	return (false);
}

/*
**	find_error_page()
**		Get all error_page of the server from a std::string &line
**		Return a map container
**		
*/
size_t		Parsing::find_error_page( std::string &line, std::vector<Parsing::t_server> &myvect, int index, size_t new_pos )
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
	{
		return new_pos;
		//throw Error(7, "Error, no error_page", 1);
	}
	while ( count > 0)
	{
		pos = line.find("error_page", pos); // on trouve erreur_page
		pos = pos + 10;
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
	{
		myvect[index]._error_server = tmp;

		return (pos);
	}
	else
		throw Error(10, "Error, code_error no conform", 1);
	// return (tmp);
}

