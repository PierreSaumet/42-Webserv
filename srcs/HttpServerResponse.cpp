/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServerSetResponse.cpp                           :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/HttpServer.hpp"
#include "../Headers/CGI_exec.hpp"


std::string		HttpServer::ft_setup_response_to_send( t_header_request requete )				
{
	std::cout << GREEN << "\n\n ft_setup_response_to_send " << CLEAR << std::endl;
	std::string file_contents;
	struct stat buff;
	std::string res;
	FILE *input_file = NULL;

	// std::cout << "test on affiche la requete " << std::endl;
	// std::cout << "requete path = " << requete.path << std::endl;

	// exit(1);

	if (still_to_send > 0)
	{
		std::cout << "je dois encore envouye des donnees " << std::endl;
		unsigned long truc = total_send - still_to_send;
		std::cout << "truc = " << truc << std::endl;
		_response_to_send.erase(0, truc);
		return (_response_to_send);
		exit(1);

	}

	std::string header = ft_setup_header(requete);
    // std::cout << "le fichier demande  est = -" << requete.path << "-" << std::endl;
	// std::cout << "le header recu est = " << header << std::endl;
	// std::cout << "taille du header = " << header.size() << std::endl;
	if (requete.error == true || requete.cgi == true || requete.return_used == true)
		return (header);
	input_file = fopen(requete.path.c_str(), "rb");
	if (stat(requete.path.c_str(), &buff) < 0)
	{
		std::cout << "Error dans ft_setup_response_to_send, cannot open the file, put error" << std::endl;
		requete.error = true;
		requete.num_error = 500;
		this->ft_setup_response_to_send(requete);
		exit(1); //return;

	}
	res.resize(buff.st_size + 100);											
	fread(const_cast<char*>(res.data()), buff.st_size, 1, input_file);
	fclose(input_file);
	file_contents = res;
	file_contents.insert(0, header.c_str());
	return (file_contents);
}

int				ft_open_binary( std::string const path )
{
	std::vector<std::string>  extension;
	extension.push_back(".jpeg");	//image
	extension.push_back(".jpg");
	extension.push_back(".png");
	extension.push_back(".gif");
	extension.push_back(".bmp");
	extension.push_back(".webp");
	extension.push_back(".ico");  
	extension.push_back(".midi");	//audio
	extension.push_back(".mpeg");
	// extension.push_back(".webm");
	// extension.push_back(".ogg");
	extension.push_back(".mp3");
	extension.push_back(".mp4");	// video
	extension.push_back(".avi");
	extension.push_back(".mov");
	extension.push_back(".mpeg4");
	extension.push_back(".webm");

	extension.push_back(".pdf");

	for (std::vector<std::string>::iterator it = extension.begin(); it != extension.end(); ++it)
	{
		size_t find = path.find(*it);
		if (find != std::string::npos)
		{
			std::cout << "path = " << path << " et extension = " << *it << " et position " << find << std::endl;
			std::cout << "path[find = " << path[find] << std::endl;
			std::cout << "size path = " << path.size() << std::endl;
			if (path.size() == find + it->size())
			{
				std::cout << "BINGO " << std::endl;
				return (1);
			}
		}
	}
	return (0);
}

std::string		HttpServer::ft_setup_header( t_header_request requete )
{
	(void)requete;
	std::string filename(this->_servers[this->_num_serv].index_server.c_str());
	FILE *input_file = NULL;
	std::string res;
	std::string file_contents;
	std::string the_header;
	struct stat buff;
	//							 on verifie le path
	std::cout << GREEN <<  "\nFonction ft_setup_header" << CLEAR << std::endl;

	if (requete.error == true)
	{
		std::cout << "On a une erreur : " << requete.num_error << std::endl;
		if (requete.body_error.empty() == false)
		{
			the_header = ft_find_error_html(requete);
			std::cout << "return de ft_setup_header : " << the_header << std::endl;
			return (the_header);
		}
		return (ft_create_error(requete));
	}
	if (requete.cgi == true)
	{
		std::cout << "IL Y A DU CGI" << std::endl;
		std::cout << "\n\n display = " << requete.body_error << std::endl;
		
		std::string tmp = requete.body_error;
		std::cout << "size requete .body error = " << requete.body_error.size() << std::endl;
		size_t pos = tmp.find("\r\n\r\n");
		if (pos == std::string::npos)
		{
			requete.error = true;
			requete.num_error = 500;

			ft_setup_error_header_response(requete);
			if (requete.body_error.empty() == false)
			{
				the_header = ft_find_error_html(requete);
				std::cout << "ls return de ft_setup_header : " << the_header << std::endl;
				// exit(1);
				return (the_header);
			}
		}
		// Attention erreur
		std::cout << " tmp.sie() " << tmp.size() << std::endl;
		std::cout << " tmp.sie() -pos = " << tmp.size() - pos<< std::endl;
		tmp.insert(0, this->ft_get_content_length(buff, tmp.size(), 0));
		
		// exit(1);
		
		
		tmp.insert(0, this->ft_get_server_name());
		tmp.insert(0, this->ft_get_date());

		if (tmp.find("Status: 201", pos) != std::string::npos)
			requete.num_error = 201;
		else if (tmp.find("Status: 200", pos) != std::string::npos)
			requete.num_error = 200;
		tmp.insert(0, this->ft_get_status(requete, true));


		return (tmp);
	}

	if (requete.return_used == true)
	{
		std::cout << GREEN << "redirection 301" << CLEAR << std::endl;
		std::cout << "path de la requete = " << requete.path << std::endl;
		
		the_header.insert(0, this->ft_get_end_header());
		the_header.insert(0, "Content-Length: 0\r\n");
		the_header.insert(0, this->ft_get_server_name());
		the_header.insert(0, this->ft_get_date());
		the_header.insert(0, "Location: " + requete.path + "\r\n");
		// the_header.insert(0, this->ft_get_return_location());
		the_header.insert(0, this->ft_get_status(requete, true));
		std::cout << "\nTHE HEADER FOR THE REDIRECTION = \n\n " << the_header << std::endl;
		// exit(1);
		return (the_header);
	}


	// A CHANGER
	if (requete.path == "/")
	{
		std::cout << "path = " << requete.path << std::endl;   
		if (requete.return_used == false)
		{
			std::cout << "Dans le cas ou il y a pas de redirection :" << std::endl;

			if (requete.path_file.empty() == true)
			{
				requete.path.append(this->_servers[this->_num_serv].index_server);
				// requete.path.erase(0, 1);								// on supprime le /
				requete.path.insert(0, this->_servers[this->_num_serv].root_server);
				std::cout << "du coup path = " << requete.path << std::endl;
				// exit(1);
			}
			else
			{
				requete.path = requete.path_file;
			}
		}
		else
		{	// Redirection 301 dans un bloc server

			std::cout << "ICI ERREUR " << std::endl;
			exit(1);
			return (the_header);

		}
		std::cout << "ICI path = " << requete.path << std::endl;
	}
	else
	{
		std::cout << "ici " << std::endl;
		if (requete.path.compare(0, 13, "--AUTOINDEX--") == 0)
		{
			std::cout << "bingo autoindex exit" << std::endl;
			the_header = this->ft_create_autoindex(requete);

			std::cout << "header autorindex = \n" << the_header <<  std::endl;
			the_header.insert(0, this->ft_get_end_header());
			the_header.insert(0, this->ft_get_content_length(buff, the_header.size(), 0));
			the_header.insert(0, this->ft_get_server_name());
			the_header.insert(0, this->ft_get_date());
			the_header.insert(0, this->ft_get_status(requete, true));
			// on utilise la variable error pour retourner directement tout le header avecle body
			requete.error = true;
			return (the_header);
			exit(1);
		}
	}


	std::cout << "on doit avoir le fichier : " << requete.path << std::endl;

	if (stat(requete.path.c_str(), &buff) < 0)	// le fichier existe pas on return 404
	{
		std::cout << RED << "on doit setup 404" << CLEAR << std::endl;
		requete.error = true;
		requete.num_error = 404;

		ft_setup_error_header();
		if (requete.body_error.empty() == false)
		{
			the_header = ft_find_error_html(requete);
			std::cout << "ls return de ft_setup_header : " << the_header << std::endl;
			// exit(1);
			return (the_header);
		}
		return (ft_create_error(requete));

	}


	// doit ouvrir en binaire ? test afficher une image
	size_t binary = 0;
	if (ft_open_binary(requete.path) == 0)
	{
		std::cout << "classic" << std::endl;
		input_file = fopen(requete.path.c_str(), "r");
		
	}
	else
	{
		std::cout << "binary" << std::endl;
		input_file = fopen(requete.path.c_str(), "rb");
		binary = 1;
	}
	
	if (input_file == NULL)
	{
		requete.error = true;
		requete.num_error = 503; // a changer

		ft_setup_error_header();
		if (requete.body_error.empty() == false)
		{
			the_header = ft_find_error_html(requete);
			std::cout << "ici return de ft_setup_header : " << the_header << std::endl;
			exit(1);
			return (the_header);
		}
		return (ft_create_error(requete));
	}
	fclose(input_file);
	std::cout << "OK donc tout est bon ici la page demandee existe on va mettre rendre le header\n" << std::endl;
	the_header.insert(0, this->ft_get_end_header());
	the_header.insert(0, this->ft_get_content_length(buff, 0, 0));
	the_header.insert(0, this->ft_get_server_name());
	the_header.insert(0, this->ft_get_date());
	the_header.insert(0, this->ft_get_charset());
	the_header.insert(0, this->ft_get_content_type(requete, binary));
	the_header.insert(0, this->ft_get_status(requete, true));

	return (the_header);

}