/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServerSetResponse.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/HttpServer.hpp"
#include "../Headers/CGI_exec.hpp"


std::string		HttpServer::ft_setup_response_to_send( void )				
{
	std::cout << GREEN << "\n\n ft_setup_response_to_send " << CLEAR << std::endl;
	std::string file_contents;
	struct stat buff;
	std::string res;
	FILE *input_file = NULL;


	std::string header = ft_setup_header();
    std::cout << "le fichier demande  est = -" << this->_header_requete[0].path << "-" << std::endl;
	std::cout << "le header recu est = " << header << std::endl;
		
	if (this->_header_requete[0].error == true || this->_header_requete[0].cgi == true || this->_header_requete[0].return_used == true)
		return (header);
	input_file = fopen(this->_header_requete[0].path.c_str(), "r");
	if (stat(this->_header_requete[0].path.c_str(), &buff) < 0)
	{
		std::cout << "Error dans ft_setup_response_to_send, cannot open the file, put error" << std::endl;
		this->_header_requete[0].error = true;
		this->_header_requete[0].num_error = 500;
		this->ft_setup_response_to_send();

	}
	res.resize(buff.st_size + 100);											
	fread(const_cast<char*>(res.data()), buff.st_size, 1, input_file);
	fclose(input_file);
	file_contents = res;
	file_contents.insert(0, header.c_str());
	return (file_contents);
}


std::string		HttpServer::ft_setup_header( void )
{

	std::string filename(this->_servers[this->_num_serv].index_server.c_str());
	FILE *input_file = NULL;
	std::string res;
	std::string file_contents;
	std::string the_header;
	struct stat buff;
	//							 on verifie le path
	std::cout << GREEN <<  "\nFonction ft_setup_header" << CLEAR << std::endl;

	if (this->_header_requete[0].error == true)
	{
		std::cout << "On a une erreur : " << this->_header_requete[0].num_error << std::endl;
		if (this->_header_requete[0].body_error.empty() == false)
		{
			the_header = ft_find_error_html( );
			std::cout << "return de ft_setup_header : " << the_header << std::endl;
			return (the_header);
		}
		return (ft_create_error());
	}
	if (this->_header_requete[0].cgi == true)
	{
		std::cout << "IL Y A DU CGI" << std::endl;
		std::cout << "\n\n display = " << this->_header_requete[0].body_error << std::endl;
		
		std::string tmp = this->_header_requete[0].body_error;
		tmp.insert(0, this->ft_get_content_length(buff, tmp.size(), 0));
		tmp.insert(0, this->ft_get_server_name());
		tmp.insert(0, this->ft_get_date());
		tmp.insert(0, this->ft_get_status(true));

		return (tmp);
	}
	if (this->_header_requete[0].return_used == true)
	{
		std::cout << GREEN << "redirection 301" << CLEAR << std::endl;
		std::cout << "path de la requete = " << this->_header_requete[0].path << std::endl;
		
		the_header.insert(0, this->ft_get_end_header());
		the_header.insert(0, "Content-Length: 0\r\n");
		the_header.insert(0, this->ft_get_server_name());
		the_header.insert(0, this->ft_get_date());
		the_header.insert(0, this->ft_get_return_location());
		the_header.insert(0, this->ft_get_status(true));
		std::cout << "\nTHE HEADER FOR THE REDIRECTION = \n\n " << the_header << std::endl;
		// exit(1);
		return (the_header);
	}


	// A CHANGER
	if (this->_header_requete[0].path == "/")
	{
		std::cout << "path = " << this->_header_requete[0].path << std::endl;   
		if (this->_header_requete[0].return_used == false)
		{
			std::cout << "Dans le cas ou il y a pas de redirection :" << std::endl;

			if (this->_header_requete[0].path_file.empty() == true)
			{
				this->_header_requete[0].path.append(this->_servers[this->_num_serv].index_server);
				// this->_header_requete[0].path.erase(0, 1);								// on supprime le /
				this->_header_requete[0].path.insert(0, this->_servers[this->_num_serv].root_server);
				std::cout << "du coup path = " << this->_header_requete[0].path << std::endl;
				// exit(1);
			}
			else
			{
				this->_header_requete[0].path = this->_header_requete[0].path_file;
			}
		}
		else
		{	// Redirection 301 dans un bloc server

			std::cout << "ICI ERREUR " << std::endl;
			exit(1);
			return (the_header);

		}
		std::cout << "ICI path = " << this->_header_requete[0].path << std::endl;
	}
	else
	{
		std::cout << "ici " << std::endl;
		if (this->_header_requete[0].path.compare(0, 13, "--AUTOINDEX--") == 0)
		{
			std::cout << "bingo autoindex exit" << std::endl;
			the_header = this->ft_create_autoindex();

			std::cout << "header autorindex = \n" << the_header <<  std::endl;
			the_header.insert(0, this->ft_get_end_header());
			the_header.insert(0, this->ft_get_content_length(buff, the_header.size(), 0));
			the_header.insert(0, this->ft_get_server_name());
			the_header.insert(0, this->ft_get_date());
			the_header.insert(0, this->ft_get_status(true));
			// on utilise la variable error pour retourner directement tout le header avecle body
			this->_header_requete[0].error = true;
			return (the_header);
			exit(1);
		}
	}


	std::cout << "on doit avoir le fichier : " << this->_header_requete[0].path << std::endl;

	if (stat(this->_header_requete[0].path.c_str(), &buff) < 0)	// le fichier existe pas on return 404
	{
		// CONDITION A CHANGER 
		// on setup une erreur 404
		std::cout << RED << "on doit setup 404" << CLEAR << std::endl;
		this->_header_requete[0].error = true;
		this->_header_requete[0].num_error = 404;

		// on verifie si l'erreur existe
		std::cout << "PB ici" << std::endl;
		ft_setup_error_header();
		exit(1);
		sleep(3);
		std::cout << "\n\n fin du test " << std::endl;
		size_t pos;
		int ret = 0;
		if (ret == 0)
		{
			std::cout << "alors erreur dans le root server " << std::endl;
			pos = this->_header_requete[0].body_error.find(this->_servers[this->_num_serv].folder_error);
		}
		else if (ret > 0 || ret == -1)
		{
			std::cout << "alors erreur dans le location server " << std::endl;
			// A TERMINER LA POSITION 2
			if (ret == -1)
				ret = 0;
			std::cout << "kek " << this->_servers[this->_num_serv].location[ret].folder_error << std::endl;
			std::cout << "bur " << this->_header_requete[0].body_error << std::endl;
			pos = this->_header_requete[0].body_error.find(this->_servers[this->_num_serv].location[ret].folder_error);
		}
		else
		{
			std::cout << "tu fais de la merde pierre avril " << std::endl;
			exit(1);
		}
		if (pos == std::string::npos)
		{
			// c'est que le dossier contentn les erreurs n'a pas ete trouve
			// doncfaut creer une erreur
			std::cout << "merde" << std::endl;
			sleep(2);
			return (ft_create_error());
		}
		else
		{
			// faut renvoyer l'erreur
			std::cout << "merde 2 " << std::endl;
			sleep(2);
			struct stat buff2;
			if (stat(this->_header_requete[0].body_error.c_str(), &buff2) < 0)
				return (ft_create_error());
			std::string tmp = ft_get_file(this->_header_requete[0].body_error);
			tmp.insert(0, this->ft_get_end_header());
			tmp.insert(0, this->ft_get_content_length(buff2, 0, 0));
			tmp.insert(0, this->ft_get_server_name());
			tmp.insert(0, this->ft_get_date());
			tmp.insert(0, this->ft_get_charset());
			tmp.insert(0, this->ft_get_content_type());
			tmp.insert(0, this->ft_get_status(true));


			// size_t len_header = tmp.size();
			// size_t find_end = tmp.find("\r\n\r\n");
			// tmp.insert(find_end, this->ft_get_content_length(buff2, 0, len_header));
			return (tmp);
		}
	}

	input_file = fopen(this->_header_requete[0].path.c_str(), "r");
	if (input_file == NULL)
	{
		std::cout << "Error, pour ouvrir la page demande avec fopen." << std::endl;
		return (NULL);
	}
	fclose(input_file);
	std::cout << "OK donc tout est bon ici la page demandee existe on va mettre rendre le header\n" << std::endl;
	the_header.insert(0, this->ft_get_end_header());
	the_header.insert(0, this->ft_get_content_length(buff, 0, 0));
	the_header.insert(0, this->ft_get_server_name());
	the_header.insert(0, this->ft_get_date());
	the_header.insert(0, this->ft_get_charset());
	the_header.insert(0, this->ft_get_content_type());
	the_header.insert(0, this->ft_get_status(true));

	return (the_header);

}