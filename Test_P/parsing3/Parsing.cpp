#include "Headers/Parsing.hpp"
#include <iostream>

Parsing::Parsing( void ) : _name_of_file(NULL) {

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
			//std::cout << "ERROR dans les data" << std::endl;

		std::cout << "OK " << std::endl;
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
		std::cout << "ERROR, should end by '}'" << std::endl;
		return (true);
	}
	if (this->ft_check_number_of_bracket())
	{
		std::cout << "ERROR, a problem occurs with brackets." << std::endl;
		return (true);
	}
	if (this->ft_check_semicolon())			// a faire
	{
		std::cout << "ERROR, a problem occurs with semicolon." << std::endl;
		return (true);
	}
	// if (ft_check_location())					// a faire
	// {
	//     std::cout << "ERROR, problem location" << std::endl;
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
	// std::cout << "lol ok " << std::endl;
	return (false);
}

// retourne un vector avec les donnees comprises dans le scope
std::vector<std::string>        Parsing::ft_get_scope(size_t index)
{
	std::cout << "Debut ft_get scope" << std::endl;
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
	std::vector<std::string>::size_type sz = this->_data.size();
	size_t                              i = 0;
	size_t                              count = 0;
	std::map<std::string, bool>            serv_dir;
	
	

	std::cout << "\n\n" << std::endl;
	// on compte le nombre de server
	while (i < sz)
	{
		if (this->_data[i] == "server" && this->_data[i + 1] == "{")
			this->_nbr_servers++;
		i++;
	}
	std::cout << "nbr serv = " << this->_nbr_servers << std::endl;

	// test fonction
	i = 0;
	int y = 0;
	while (i < this->_nbr_servers)
	{
	   
		std::vector<std::string>    scope_actual = this->ft_get_scope(y);

		std::cout << "La taille de scope_actual = " << scope_actual.size() << std::endl;
		y = scope_actual.size();

		serv_dir.insert(std::pair<std::string, bool>("listen", false));
		serv_dir.insert(std::pair<std::string, bool>("server_name", false));
		serv_dir.insert(std::pair<std::string, bool>("error_page", false));
		serv_dir.insert(std::pair<std::string, bool>("root", false));
		serv_dir.insert(std::pair<std::string, bool>("dav_methods", false));
		serv_dir.insert(std::pair<std::string, bool>("autoindex", false));
		serv_dir.insert(std::pair<std::string, bool>("client_body_buffer_size", false));
		serv_dir.insert(std::pair<std::string, bool>("cgi_path", false));
		serv_dir.insert(std::pair<std::string, bool>("upload_path", false));

		// on parcoourt le vector et on verifie que nos directives sont presentes
		size_t k = 0;
		count = 0;
		while (k < scope_actual.size())
		{
			for (std::map<std::string, bool>::iterator it_b = serv_dir.begin(); it_b != serv_dir.end(); it_b++)
			{
				if (it_b->first == scope_actual[k])
				{
					count++;
					if (it_b->second == false)
						it_b->second = true;
					else
					{
						std::cout << "ERROR, bloc server cannot have doublon BNJJJJJJJJ: " << it_b->first << std::endl;
						return (true);
					}
				}
			}
			if (scope_actual[k] == "location")
				break;
			k++;
		}
		std::cout << "COUT =  " << count << std::endl;

		// HARDCORE si il y a pas 9 directives = erreur ? a voir ...
		if (count != 9)
		{
			std::cout << "ERROR, missing : ";
			for (std::map<std::string, bool>::iterator it_b = serv_dir.begin(); it_b != serv_dir.end(); it_b++)
			{
				if (it_b->second == false)
					std::cout << "\"" << it_b->first << "\" ";
			}
			std::cout << "\t directive(s) in a bloc server before a location bloc in the configuration file" << std::endl;
			serv_dir.clear();
			return (true);
		}
		serv_dir.clear();

		this->_servers.push_back(Parsing::t_server());
		k = 1;
		while (k < scope_actual.size())
		{
			if (scope_actual[k] == "listen")
			{
				std::cout << "go listen" << std::endl;
				if (this->ft_get_listen(k, scope_actual, i))
					return (true);
				std::cout << "\ti = " << i << " host = " << this->_servers[i].host_server << " et port = " << this->_servers[i].port_server << std::endl;
				k += 2;
			}
			else if (scope_actual[k] == "autoindex")
			{
			    std::cout << "go autoindex " << std::endl;
			    if (this->ft_get_autoindex(k, scope_actual, i))
					return (true);
			    std::cout << "\ti = " << i << "autoindex = " << this->_servers[i].autoindex_server << std::endl;
			    k += 2;
			}
			// else if (truc[k] == "server_name")
			// {
			//     std::cout << "go server_name " << std::endl;
			//     this->ft_get_server_name(k, truc, i);
			//     std::cout << "\tserver_name = " << this->_servers[i].name_server << std::endl;
			//     k += 2;
			// }
			// else if (truc[k] == "root")
			// {
			//     std::cout << "go root" << std::endl;
			//     this->ft_get_root(k ,truc, i);
			//     std::cout << "\troot = " << this->_servers[i].root_server << std::endl;
			//     k += 2;
			// }

			// else if (truc[k] == "dav_methods")
			// {
			//     std::cout << "go dav_methods " << std::endl;
			//     k = this->ft_get_methods(k, truc, i);
			//     std::cout << "\tmedhods 1 = " << this->_servers[i].methods_server[0] << std::endl;
			//     //k += 2;
			// }
			// else if (truc[k] == "error_page")
			// {
			//     std::cout << "go error_page " << std::endl;
			//     k = this->ft_get_error(k, truc, i);
				
			//     //k += 3;
			// }
			// else if (truc[k] == "location")
			// {
			//     std::cout << "go location " << std::endl;
			//     break;
			//     k += 2;
			// }
			// else if (truc[k] == "{" || truc[k] == "}")
			// {
			//     std::cout << " on coninue " << std::endl;
			//     k++;
			// }
			else
			{
				std::cout << " EUH ERROR " << scope_actual[k] << " et k = " << k << std::endl;
				return (true);
				//exit(EXIT_FAILURE);
				//break;
			}
		}


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
**          listen / server_name / index / autoindex / root / client_body_buffer_size / cgi_path / upload_path
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
	directives.push_back("upload_path");

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
	int                                 count = 0;
	for (it_b = this->_data.begin(); it_b != this->_data.end(); it_b++)
	{
		std::size_t	find_open;
		std::size_t find_close;
		find_open = (*it_b).find("{");
		find_close = (*it_b).find("}");
		if (find_open != std::string::npos)
		{
			if ((*it_b).compare("{") == 0)
			{
				count++;
				//std::cout << "OUI open =" << *it_b << std::endl;
			}
			else
				return (true);
		}
		if (find_close != std::string::npos)
		{
			if ((*it_b).compare("}") == 0)
			{
				count--;
				//std::cout << "OUI close =" << *it_b << std::endl;
			}
			else
				return (true);
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