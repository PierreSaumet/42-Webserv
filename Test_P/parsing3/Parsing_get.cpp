#include "Parsing.hpp"
# include <iostream>

size_t          Parsing::ft_get_error( size_t k, std::vector<std::string> tmp, size_t index )
{
    // on incremente k on passe errr_page
    k += 1;

    while (tmp[k][tmp[k].size() - 1] != ';')
    {
        //std::cout << "\tLA tmp[k] = " << tmp[k] << std::endl;
        int y = 0;
        while (tmp[k][y])
        {
            if (isdigit(tmp[k][y]))
            {
                y++;
                //std::cout << "NO problem " << std::endl;
            }
            else
            {
                std::cout << "Error: error_page directive should only have numbers then a directory!" << std::endl;
                exit(EXIT_FAILURE);
                //break;
            }
        }
        int error_code = std::strtol(tmp[k].c_str(), NULL, 10);
        if (this->ft_check_code_error(error_code) == 1)
        {
            std::cout << "EUh il faut quitter? " << std::endl;
            std::cout << "car = " << this->ft_check_code_error(error_code) << std::endl;
            exit(EXIT_FAILURE);
        }
        //std::cout << "ERROR = " << error_code << std::endl;
        this->_servers[index].error_server.insert(std::pair<int, std::string>(error_code, "NULL"));
        k++;
    }
    // on verifie le premier si c'est un point et le deuxieme un /
    if (tmp[k][0] != '.' || tmp[k][1] != '/')
    {
        std::cout << "Error, error_page directive should end with a directory or file" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    std::string address = tmp[k].substr(0, tmp[k].size() - 1);
    struct stat buffer;
    if (stat(address.c_str(), &buffer) != 0)
    {
        std::cout << "Error, error_page directive, directory doesn't exist!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // on ajute l'addresse a toutes les erreurs
    std::map<int, std::string>::iterator it = this->_servers[index].error_server.begin();
    for (it = this->_servers[index].error_server.begin(); it != this->_servers[index].error_server.end(); it++)
    {
        if (it->second == "NULL")
            it->second = address;
        struct stat buff;
        std::string check_c = std::to_string(it->first);
        check_c.append(".html");
        it->second.append("/");
        it->second.append(check_c);
        std::cout << "it->second = " << it->second << std::endl;
        if (stat(it->second.c_str(), &buff) != 0)
        {
            std::cout << "Error, error_page directive, cannot find the error file" << std::endl;
            exit(EXIT_FAILURE);
        }
        // (void)buff;
    }
    /*
    **  ne verifie pas si ;es fichiers sont vides ...
    */
    
    k++;
    return (k);
}


size_t          Parsing::ft_get_methods( size_t k, std::vector<std::string> tmp, size_t index )
{
    std::vector<std::string> methods;

    methods.push_back("DELETE");
    methods.push_back("GET");
    methods.push_back("POST");

    // on incremente k 
    k += 1;
    std::cout << "tmp[k] = " << tmp[k] << std::endl;

    while (tmp[k][tmp[k].size() - 1] != ';')
    {
        if (tmp[k].compare("DELETE") == 0)
            this->_servers[index].methods_server.push_back("DELETE");
        else if (tmp[k].compare("POST") == 0)
            this->_servers[index].methods_server.push_back("POST");
        else if (tmp[k].compare("GET") == 0)
            this->_servers[index].methods_server.push_back("GET");
        else
        {
            std::cout << "Error, dav_methods can only have DELETE POST and GET methods!" << std::endl;
            exit(EXIT_FAILURE);
        }
        std::cout << "tmp[k] TEST = " << tmp[k] << std::endl;
        k++;
    }
    // Donc on peut en avoir que deux max pour l'instant
    if (this->_servers[index].methods_server.size() > 2)
    {
        std::cout << "Error, dav_methods can only have 3 methods!" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // on ajoute e dernier
    this->_servers[index].methods_server.push_back(tmp[k].substr(0, tmp[k].size() - 1));
    k++;
    // on verifie les doublons
    size_t int_del = 0;
    size_t int_get = 0;
    size_t int_post = 0;
    
    std::vector<std::string>::iterator it;
    for (it = this->_servers[index].methods_server.begin(); it != this->_servers[index].methods_server.end(); it++)
    {
        std::cout << "it = " << *it << std::endl;
        if (*it == "DELETE")
            int_del++;
        else if (*it == "POST")
            int_post++;
        else if (*it == "GET")
            int_get++;
        else
        {
            std::cout << "Error, this method is unknow!" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (int_del > 1 || int_post > 1 || int_get > 1)
        {
            std::cout << "Error, dav_methods should not have doublons" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return (k);
}


void            Parsing::ft_get_autoindex( size_t k, std::vector<std::string> tmp, size_t index )
{
    k += 1;
    std::cout << "Dans autoindex =" << tmp[k] << std::endl;
    size_t  len = tmp[k].size();
    if (tmp[k][len] != '\0')
    {
        std::cout << "Error, autoindex directive should end with '\0'" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (tmp[k][len - 1] != ';')
    {
        std::cout << "Error, autoindex directive should end with ';'" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (tmp[k].compare("on;") != 0 && tmp[k].compare("off;") != 0)
    {
        std::cout << "Error, autoindex should be 'on' or 'off' " << std::endl;
        exit(EXIT_FAILURE);
    }
    if (tmp[k].compare("on;") == 0)
        this->_servers[index].autoindex_server = true;
    else
        this->_servers[index].autoindex_server = false;
    
    return ;
}


void            Parsing::ft_get_root(size_t k, std::vector<std::string> tmp, size_t index)
{
    k += 1;
    size_t  len = tmp[k].size();
    if (tmp[k][len] != '\0')
    {
        std::cout << "Error, root directive should end with '\0'" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (tmp[k][len - 1] != ';')
    {
        std::cout << "Error, root directive should end with ';'" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (tmp[k][0] != '.' || tmp[k][1] != '/')
    {
        std::cout << "Error, root directive should start with './'" << std::endl;
        exit(EXIT_FAILURE);
    }
    this->_servers[index].root_server = tmp[k].substr(0, len - 1);
    struct stat buffer;
    if (stat(this->_servers[index].root_server.c_str(), &buffer) != 0)
    {
        std::cout << "Error, root directive doesn't exist!" << std::endl;
        exit(EXIT_FAILURE);
    }
    return ;
}

void         Parsing::ft_get_server_name(size_t k, std::vector<std::string> tmp, size_t index)
{
    k += 1;
    size_t  len = tmp[k].size();
    if (tmp[k][len] != '\0')
    {
        std::cout << "Error, server_name directive should end with '\0'" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (tmp[k][len - 1] != ';')
    {
        std::cout << "Error, server_name directive should end with ';'" << std::endl;
        exit(EXIT_FAILURE);
    }
    this->_servers[index].name_server = tmp[k].substr(0, len - 1);
    return ;
}

void         Parsing::ft_get_listen( size_t k, std::vector<std::string> tmp, size_t index) 
{
    k += 1;
    size_t      len = tmp[k].size();
    if (tmp[k][len] != '\0')
    {
        std::cout << "Error, listen directive should end with \0" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (tmp[k][len - 1] != ';')
    {
        std::cout << "Error, listen directive should end with ';'" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (tmp[k].find(":", 0) == std::string::npos)
    {
        std::cout << "Error, listen directive should have a ':' between host and port" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (len > 15)
    {
        std::cout << "Error, listen directive have bad host or port" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (tmp[k].compare(0, 10, "127.0.0.1:") != 0)
    {
        std::cout << "Error, port should be 127.0.0.1" << std::endl;
        exit(EXIT_FAILURE);
    }
    this->_servers[index].host_server = tmp[k].substr(0, 9);
    this->_servers[index].port_server = std::strtol(tmp[k].substr(10, 4).c_str(), NULL, 10);
	if (this->_servers[index].port_server < 0 || this->_servers[index].port_server > 65535)
    {
        std::cout << "Error, port should be between 0 and 65535" << std::endl;
        exit(EXIT_FAILURE);
    }
    return ;
}




bool					Parsing::ft_check_code_error( int code ) const
{
	// if (server_code_error(code) == 1 || client_code_error(code) == 1)
	// 	return (1);
	// else
	// 	return (0);
	if (code > 399 && code < 500)
		return (this->ft_check_code_client(code));
	else if (code > 499 && code < 600)
		return (this->ft_check_code_serv(code));
	else
	{
        std::cout << "ERROR, code pas bon MERDE" << std::endl;
        exit(EXIT_FAILURE);
		//throw Error(8, "Error, code error not correct", 1);
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
bool					Parsing::ft_check_code_serv( int code ) const
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
bool					Parsing::ft_check_code_client( int code ) const
{
	if ((code >= 400 && code <= 417) || (code >= 421 && code <= 426) 
			|| code == 428 || code == 429 || code == 431 || code == 451 )		// || code == 499 pour NGINX
		return (false);
	else
		return (true);
}
