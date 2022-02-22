#include "Headers/Parsing.hpp"
#include <iostream>

Parsing::Parsing( void ) : _name_of_file(NULL), _nbr_servers(0) {

	return ;
}

Parsing::Parsing( std::string &configfile ) : _name_of_file(configfile), _nbr_servers(0) {

	// On verifie que le fichier de configuration est correct
	if (!this->ft_check_conf_file())
	{
		std::cout << "On continue" << std::endl;
		this->_data = this->ft_get_data_container();
		this->_nbr_servers = 0;
	
		if (this->ft_check_data() == true)
			std::cout << "ERROR dans les data" << std::endl;
		else
		{
			std::cout << "Parsing OK =) " << std::endl;
			this->display_all();
		}
	}
	else
		std::cout << "On arrete" << std::endl;

	
	
	// std::vector<std::string>::iterator it_b;
	// for (it_b = this->_data.begin(); it_b != this->_data.end(); it_b++)
	// {
	//     std::cout << " ok = " << *it_b << std::endl;
	// }

	return ;
}

/*
**	Parsing Destructor
*/
Parsing::~Parsing( void ) {

	return ;
}

/*
**	Parsing copy Constructor
*/
Parsing::Parsing( const Parsing &copy ) : _name_of_file(copy._name_of_file) {

	return ;
}

/*
**	Parsing overload operator =	
*/
Parsing                         &Parsing::operator=( const Parsing &element ) {

	if (this != &element)
	{
		this->_name_of_file = element._name_of_file;
	}
	return (*this);
}

/*
**	ft_check_data():
**		This function is the main function for the parsing.
**		It will check that everything is in order.
*/
bool                            Parsing::ft_check_data( void ) {

   // std::vector<std::string>::iterator it_b;
   // for (it_b = this->_data.begin(); it_b != this->_data.end(); it_b++)
	// {
	//     std::cout << " ok = " << *it_b << std::endl;
	// }

	if (this->_data.empty())									// veriffie que les donnees ne sont pas vides
	{
		std::cout << "ERROR, data empty" << std::endl;
		return (true);
	}
	if (this->_data[0] != "server" || this->_data[1] != "{")		// verifie que les donnees debutent par server et {
	{
		std::cout << "ERROR, should start with 'server' and '{'" << std::endl;
		return (true);
	}
	if (this->_data[this->_data.size() - 2] != "}" && this->_data[this->_data.size() - 1] == "")	// verifie que les donnees termine par }
	{
		std::cout << "Error, should end by '}'" << std::endl;
		return (true);
	}
	if (this->ft_check_number_of_bracket())							// verifie les brackets
	{
		std::cout << "ERROR, problem with bracket" << std::endl;
		return (true);
	}
	if (this->ft_check_semicolon())									// verifie les ;
	{
	    std::cout << "ERROR, no semi colon" << std::endl;
	    return (true);
	}
	// if (ft_check_location())					// a faire
	// {
	//     std::cout << "ERROR, pro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blem location" << std::endl;
	//     return (true);
	// }
	// std::string serv = "server";
	// std::string loc = "location";
	// this->ft_get_scope(0);
	if (this->ft_check_server())
	{
		std::cout << "ERROR, problem bloc server" << std::endl;
		return (true);
	}
	
	std::cout << "lol ok " << std::endl;
	return (false);
}


/*
**	ft_get_scope( size_t index ):
**		This function will return the scope starting and the given 'index' argument.
**
**	==> It will return a std::vector<std::string> which contains the scope.
*/
std::vector<std::string>        Parsing::ft_get_scope( size_t index )
{
	std::cout << "Debut ft_get scope   index = " << index << std::endl;
	size_t                  i = index;
	size_t                  count = 0;
	std::vector<std::string>::size_type sz = this->_data.size();
	std::vector<std::string>::iterator      it_b;
	std::vector<std::string>::iterator      it_e;
	std::cout << "size = " << this->_data.size() << std::endl;
	while (i < sz)
	{
		if (this->_data[i] == "{")
		{
			if (count == 0)
			{
				it_b = this->_data.begin() + i;
				//std::cout << "la i = " << i << std::endl;
			}
			count++;
		}
		if (this->_data[i] == "}" && count != 0)
		{
			count--;
			if (count == 0)
			{
				it_e = this->_data.begin() + i + 1;
				//std::cout << "ici i = " << i  << std::endl;
				break;
			}
		}
		i++;
	}
	std::vector<std::string>    tmp(it_b, it_e);
	for (std::vector<std::string>::iterator test = tmp.begin(); test != tmp.end(); test++)
	{
		std::cout << "test = " << *test<< std::endl;
	}
	return (tmp);
}

bool                            Parsing::ft_check_server( void )
{
	std::vector<std::string>::size_type size_data = this->_data.size();
	size_t                              i = 0;
	// size_t                              count = 0;
	std::map<std::string, bool>            serv_dir;
	
	

	std::cout << "\n\n" << std::endl;
	// on compte le nombre de server
	while (i < size_data)
	{
		if (this->_data[i] == "server")
		{
			if (this->_data[i + 1] == "{")
				this->_nbr_servers++;
			else
			{
				std::cout << "Error, bloc server." << std::endl;
				return (true);
			}
		}
		i++;
	}
	std::cout << "nbr serv = " << this->_nbr_servers << std::endl;
	// test fonction
	i = 0;
	int server_size = 0;
	while (i < this->_nbr_servers)
	{
		std::cout << "AVANT GET SCOPE server _ize = " << server_size << std::endl;
		std::vector<std::string>    scope_server = this->ft_get_scope(server_size);
		std::cout << "APRES GET SCOPTE server _ize = " << server_size << std::endl;
		std::cout << "La taille de scope_server = " << scope_server.size() << std::endl;
		server_size = scope_server.size();

		if (ft_check_directive_server(scope_server, server_size))
		{
			std::cout << "ERROR LA :ERDEEEEEEEEE" << std::endl;
			return (true);
		}
		else
			std::cout << "GOOD" << std::endl;
	
		size_t k = 0;
		k = 1;
		if (ft_find_directive_server(k, scope_server, i))
		{
			std::cout << "ERREUR CATA" << std::endl;
			return (true);
		}
		else
			std::cout << "cool ca marche " << std::endl;
		// while (k < scope_server.size())
		// {
		// 	if (scope_server[k] == "listen")
		// 	{
		// 		std::cout << "go listen" << std::endl;
		// 		if (this->ft_get_listen(k, scope_server, i))
		// 			return (true);
		// 		std::cout << "\thost = " << this->_servers[i].host_server << " et port = " << this->_servers[i].port_server << std::endl;
		// 		k += 2;
		// 	}
		// 	else if (scope_server[k] == "server_name")
		// 	{
		// 		std::cout << "go server_name " << std::endl;
		// 		if (this->ft_get_server_name(k, scope_server, i))
		// 			return (true);
		// 		std::cout << "\tserver_name = " << this->_servers[i].name_server << std::endl;
		// 		k += 2;
		// 	}
		// 	else if (scope_server[k] == "autoindex")
		// 	{
		// 		std::cout << "go autoindex " << std::endl;
		// 		if (this->ft_get_autoindex(k, scope_server, i))
		// 			return (true);
		// 		std::cout << "\tautoindex = " << this->_servers[i].autoindex_server << std::endl;
		// 		k += 2;
		// 	}
		// 	else if (scope_server[k] == "root")
		// 	{
		// 		std::cout << "go root" << std::endl;
		// 		if (this->ft_get_root(k ,scope_server, i))
		// 			return (true);
		// 		std::cout << "\troot = " << this->_servers[i].root_server << std::endl;
		// 		k += 2;
		// 	}
		// 	else if (scope_server[k] == "error_page")
		// 	{
		// 		std::cout << "go error_page " << std::endl;
		// 		k = this->ft_get_error(k, scope_server, i);
		// 		if (k == 0)
		// 			return (true);
		// 		//k += 3;
		// 	}
		// 	else if (scope_server[k] == "dav_methods")
		// 	{
		// 		std::cout << "go dav_methods " << std::endl;
		// 		k = this->ft_get_methods(k, scope_server, i);
		// 		if (k == 0)
		// 		{
		// 			std::cout << "ERROR DANS DAV METHODS" << std::endl;
		// 			return (true);
		// 		}
		// 		std::cout << "\tmedhods 1 = " << this->_servers[i].methods_server[0] << std::endl;
		// 		//k += 2;
		// 	}
		// 	else if (scope_server[k] == "client_body_buffer_size")
		// 	{
		// 		std::cout << "go client_body_buffer_size" << std::endl;
		// 		if (this->ft_get_buffer_size(k, scope_server, i))
		// 			return (true);
		// 		k += 2;
		// 	}
		// 	else if (scope_server[k] == "cgi_path")
		// 	{
		// 		std::cout << "go cgi path" << std::endl;
		// 		if (this->ft_get_cgi_path(k, scope_server, i))
		// 			return (true);
		// 		k += 2;
		// 	}
		// 	else if (scope_server[k] == "upload_store")
		// 	{
		// 		std::cout << "go ft_get_upload_store " << std::endl;
		// 		if (this->ft_get_upload_store(k, scope_server, i))
		// 			return (true);
		// 		k += 2;
		// 	}
		// 	else if (scope_server[k] == "server" && scope_server[k + 1] == "{")
		// 	{
		// 		std::cout << "Error, a bloc server cannot have another bloc server inside." << std::endl;
		// 		return (true);
		// 	}
		// 	else if (scope_server[k] == "location")
		// 	{
		// 		std::cout << "go location " << std::endl;
		// 		std::cout << "k = " << k << std::endl;
		// 		k = ft_get_location( k, scope_server, i);
		// 		std::cout << "fin de location : k = " << k << std::endl;
		// 		if (k == 0)
		// 			return (true);
		// 		// return (true);
		// 		// break;
		// 		// k += 2;
		// 	}
		// 	else if (scope_server[k] == "index")
		// 	{
		// 		std::cout << "go index " << std::endl;
		// 		if (this->ft_get_index(k, scope_server, i))
		// 		{
		// 			return (true);
		// 		}
		// 		k += 2;
		// 	}
		// 	else
		// 	{
		// 		std::cout << "\n\nDANS ELSE, tout est parse, il manque location et k == ." << scope_server[k] << std::endl;
		// 		if (scope_server[k] == "}")
		// 			break;
		// 		else
		// 		{
		// 			std::cout << "INSTRUCTION NON RECONNU" << std::endl;
		// 			return (true);
		// 		}
		// 		//std::cout << " EUH ERROR " << scope_server[k] << " et k = " << k << std::endl;
		// 		//if ()
		// 		//return (true);
		// 		//exit(EXIT_FAILURE);
		// 		//break;
		// 	}
		// }

		std::cout << "FIN DU PREMIER SERVER  i = " << i << " DEBUT DU DEUXIEME" << std::endl;
		i++;
		
	}
	
	return (false);
}

bool                            Parsing::ft_check_location( void )      // to do
{
	std::vector<std::string>::size_type sz = this->_data.size();
	size_t i = 0;
	while (i < sz)
	{
		std::cout << "val = " << this->_data[i] << std::endl;
		if (this->_data[i] == "location")
		{
			if (this->_data[i + 1][0] != '/')
			{
				std::cout << "ERROR, after location should start with /" << std::endl;
				return (true);
			}
			if (this->_data[i + 2] != "{")
			{
				std::cout << "ERROR, after location / should start with {" << std::endl;
				return (true);
			}
		}
		i++;
	}
	return (false);
}

/*
**  ft_check_semicolon():
**      This function will check if after a directive, there is a semi colon at the end.
**      It will only checks the following directives:
**          listen / server_name / index / autoindex / root / client_body_buffer_size / cgi_path / upload_store
**
**  ==> Returns 0 if there is no error, otherwise returns 1.
*/
bool                            Parsing::ft_check_semicolon( void )     // to do
{
	std::vector<std::string>            directives;
	std::vector<std::string>::iterator  it_direc;
	directives.push_back("listen");
	directives.push_back("server_name");
	directives.push_back("index");
	directives.push_back("autoindex");
	directives.push_back("root");
	directives.push_back("client_body_buffer_size");
	directives.push_back("cgi_path");
	directives.push_back("upload_store");
	directives.push_back("index");

	std::vector<std::string>::iterator  it_b;
	for (it_b = this->_data.begin(); it_b != this->_data.end(); it_b++)
	{
		for (it_direc = directives.begin(); it_direc != directives.end(); it_direc++)
		{
			if (*it_b == *it_direc)
			{
				//std::cout << "it_b = " << *it_b << " et it_direc = " << *it_direc <<  std::endl;
				std::string tmp = *(++it_b);
				if (tmp[tmp.size() - 1] != ';')
					return (true);
			}
		}
	}
	return (false);
}

/*
**	ft_check_number_of_bracket():
**		This function will count the bracket '{' and '}'.
**		And checks if the brackets are alone.
**	
**	==> Returns 0 if there is no difference between the number of 'open bracket = {' and 'close bracket }'.
**		Otherwise, return 1.
*/
bool                            Parsing::ft_check_number_of_bracket( void )
{
	std::vector<std::string>::iterator  it_b;
	std::size_t							find_open;
	std::size_t							find_close;
	int                                 count = 0;
	
	for (it_b = this->_data.begin(); it_b != this->_data.end(); it_b++)
	{
		std::cout << "*it_b = " << *it_b << std::endl;
		find_open = (*it_b).find("{");
		find_close = (*it_b).find("}");
		if (find_open != std::string::npos)
		{
			std::cout << "*it_b.size() = " << (*it_b).length() << std::endl; 
			if ((*it_b).compare("{") == 0)
				count++;
			else
			{
				if ((*it_b).length() == 1)
					return (true);
			}
		}
		if (find_close != std::string::npos)
		{
			if ((*it_b).compare("}") == 0)
				count--;
			else
			{
				std::cout << "ICI *it_b = " << *it_b << std::endl;
				std::cout << "*it_b.size() = " << (*it_b).length() << std::endl; 
				// if ((*it_b).length() )
				if ((*it_b).length() == 1)
					return (true);
			}
		}
	}
	if (count == 0)
		return (false);
	else
		return (true);
}

/*
**	ft_get_data_container():
**		This function will get all the data from the configuration file and
**		put it into a std::vector<std::string> container.	
**
**	==> Returns the container.
*/
std::vector<std::string>        Parsing::ft_get_data_container( void ) {

	std::vector<std::string>    tmp;
	std::string                 tmp_name(this->_name_of_file.begin(), this->_name_of_file.end());
	std::ifstream               fs;

	//fs.open(tmp_name);
	fs.open(tmp_name.c_str());
	if (fs.is_open() == 0)
	{
		fs.close();
		std::cout << "ERROR, configuration file doesn't exist" << std::endl;
		return (tmp);
	}
	else
	{
		char                    *data;
		std::stringstream       buffer;
		std::string             line;
		buffer << fs.rdbuf();						// recupere tous le fichier dans un buffer
		line = buffer.str();						//	mets le buffer dans une string pour la traiter
		line = this->ft_delete_comm(line);			//	supprime tous les commentaires
		data = std::strtok(&line[0], " \t\n\v\r\f");	// on divise notre string en morceaux tous les types d'espaces
		if (data == NULL)
		{
			fs.close();
			std::cout << "ERROR, configuration file is empty after deleting commentaries.\n";
		}
		else
		{
			while (data)								// on met toutes les valeurs dans un container vector
			{
				tmp.push_back(data);
				data = strtok(NULL, " \t\n\v\r\f");		
			}
			fs.close();
		}
	}
   	tmp.push_back("");
	return (tmp);
}


/*
**	ft_check_conf_file():
**		This function will check if the configuration file is correct.
**		The file should exist, not be empty, ending with a '.conf' and use '.conf' only for file format
**
**	==>	Returns 1 if an error occurs, 0 otherwise.
*/
bool                            Parsing::ft_check_conf_file( void ) {

	std::string     tmp_name(this->_name_of_file.begin(), this->_name_of_file.end());
	std::ifstream   fs;
	
	// fs.open(tmp_name);
	fs.open(tmp_name.c_str());
	if (fs.is_open() == 0)
	{
		fs.close();
		std::cout << "ERROR, configuration file doesn't exist" << std::endl;
		return (true);
	}
	else
	{
		if (fs.peek() == std::string::traits_type::eof())
		{
			fs.close();
			std::cout << "ERROR, configuration file is empty" << std::endl;
			return (true);
		}
		else
		{
			std::string         to_find = ".conf";
			size_t              pos_to_find = tmp_name.find(to_find, 0);

			if (pos_to_find == std::string::npos)
			{
				fs.close();
				std::cout << "ERROR, configuration file must terminate with '.conf'" << std::endl;
				return (true);
			}
			else
			{
				tmp_name.push_back('\n');
				if (tmp_name.compare(pos_to_find, 6, ".conf\n") != 0)
				{
					fs.close();
					std::cout << "ERROR, configuration file must have a different name and end exclusively with '.conf'" << std::endl;
					return (true);
				}
			}
			fs.close();
		}
	}
	return (false);
}