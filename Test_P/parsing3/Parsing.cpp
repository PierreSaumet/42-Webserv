#include "Headers/Parsing.hpp"
#include <iostream>

/*
**	Parsing empty constructor.
*/
Parsing::Parsing( void ) : _name_of_file(NULL), _nbr_servers(0) {

	return ;
}

/*
**	Parsing constructor.
**		It takes one argument corresponding to the name of the configuration file.
**		It checks of this file is correct and starts the parsing with the function 'ft_check_data'.
*/
Parsing::Parsing( std::string &configfile ) : _name_of_file(configfile), _nbr_servers(0) {

	try
	{
		if (!this->ft_check_conf_file())
		{
			std::cout << "On continue" << std::endl;
			this->_data = this->ft_get_data_container();
			this->_nbr_servers = 0;
		}
		if (!this->ft_check_data())
		{
			std::cout << "On continue 2" << std::endl;
		}
	
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	this->display_all();
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
**	Parsing overload operator '='	
*/
Parsing                         &Parsing::operator=( const Parsing &element ) {

	if (this != &element)
	{
		this->_name_of_file = element._name_of_file;
		this->_data = element._data;
		this->_nbr_servers = element._nbr_servers;
		this->_servers = element._servers;
	}
	return (*this);
}

/*
**	ft_check_data():
**		This function is the main function for the parsing.
**		It will check that everything is in order.
**
**	==>	Returns 1 if an error occurs, otherwise returns 0.
*/
bool                            Parsing::ft_check_data( void )
{
	if (this->_data.empty())
	{
		std::cout << "ERROR, data empty" << std::endl;
		return (true);
	}
	if (this->_data[0] != "server" || this->_data[1] != "{")
	{
		std::cout << "ERROR, should start with 'server' and '{'" << std::endl;
		return (true);
	}
	if (this->_data[this->_data.size() - 2] != "}" && this->_data[this->_data.size() - 1] == "")
	{
		std::cout << "Error, should end by '}'" << std::endl;
		return (true);
	}
	if (this->ft_check_number_of_bracket())
	{
		std::cout << "ERROR, problem with bracket" << std::endl;
		return (true);
	}
	if (this->ft_check_semicolon())
	{
	    std::cout << "ERROR, no semi colon" << std::endl;
	    return (true);
	}
	if (this->ft_check_server())
	{
		std::cout << "ERROR, problem bloc server" << std::endl;
		return (true);
	}
	std::cout << "lol ok " << std::endl;
	return (false);
}


/*
**	ft_check_server():
**		This function will retrieve all the information contained in a server block.
**		It will count how many server blocks there are in the configuration file.
**		Then, it will check that the mandatory directives are present.
**		If so, it retrieves the informations from the configuration file.
**
**	==>	Returns 1 if an error occurs, otherwise returns 0.
*/
bool                            Parsing::ft_check_server( void )
{
	std::vector<std::string>::size_type size_data = this->_data.size();
	std::map<std::string, bool>         serv_dir;
	size_t                              i = 0;
	int									server_size = 0;

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
	i = 0;
	while (i < this->_nbr_servers)
	{
		std::vector<std::string>    scope_server = this->ft_get_scope(server_size);
		server_size = scope_server.size();
		if (ft_check_directive_server(scope_server, server_size))
		{
			std::cout << "ERROR LA :ERDEEEEEEEEE" << std::endl;
			return (true);
		}
		else
			std::cout << "GOOD" << std::endl;
		if (ft_find_directive_server(1, scope_server, i))
		{
			std::cout << "ERREUR CATA" << std::endl;
			return (true);
		}
		else
			std::cout << "cool ca marche " << std::endl;
		std::cout << "FIN DU PREMIER SERVER  i = " << i << " DEBUT DU DEUXIEME" << std::endl;
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
bool                            Parsing::ft_check_semicolon( void )
{
	std::vector<std::string>            directives;
	std::vector<std::string>::iterator  it_direc;
	std::vector<std::string>::iterator  it_b;
	directives.push_back("listen");
	directives.push_back("server_name");
	directives.push_back("index");
	directives.push_back("autoindex");
	directives.push_back("root");
	directives.push_back("client_body_buffer_size");
	directives.push_back("cgi_path");
	directives.push_back("upload_store");
	directives.push_back("index");

	for (it_b = this->_data.begin(); it_b != this->_data.end(); it_b++)
	{
		for (it_direc = directives.begin(); it_direc != directives.end(); it_direc++)
		{
			if (*it_b == *it_direc)
			{
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
		find_open = (*it_b).find("{");
		find_close = (*it_b).find("}");
		if (find_open != std::string::npos)
		{
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

	std::string                 tmp_name(this->_name_of_file.begin(), this->_name_of_file.end());
	std::vector<std::string>    tmp;
	std::ifstream               fs;

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
	
	fs.open(tmp_name.c_str());
	if (fs.is_open() == 0)
	{
		fs.close();
		throw Error(1, "Error, the configuration file doesn't exist.", 0);
	}
	else
	{
		if (fs.peek() == std::string::traits_type::eof())
		{
			fs.close();
			throw Error(2, "Error, the configuration file is empty.", 0);;
		}
		else
		{
			std::string         to_find = ".conf";
			size_t              pos_to_find = tmp_name.find(to_find, 0);
			if (pos_to_find == std::string::npos)
			{
				fs.close();
				throw Error(3, "Error, the configuration file must terminate with '.conf'.", 0);
			}
			else
			{
				tmp_name.push_back('\n');
				if (tmp_name.compare(pos_to_find, 6, ".conf\n") != 0)
				{
					fs.close();
					throw Error(4, "Error, the configuration file must have a different name and end exclusively with '.conf'.", 0);
				}
				fs.close();
			}
		}
	}
	return (false);
}