/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HPP
# define ERROR_HPP

# include <iostream>
# include <ios>
# include <sstream>
# include <exception>
# include <string>
# include <string.h>

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define BLUE "\033[1;34m"
# define CLEAR "\033[0m"

class Error: public std::exception
{
    public:
        Error(int number = 0, std::string const &line="", int level=0) throw();
    
	    virtual ~Error() throw();

        virtual const char* what() const throw();
		int get_level() const throw();
 
    protected:
        int _number_error;               	//Numéro de l'erreur
        std::string _line_error;            //Description de l'erreur
        int _level_error;               	//Niveau de l'erreur

};

/*
**	Classification
**		Level_error	=	0	==>	Configuration file
**			number_error	=	1	==>	file doesn't exist.
**							=	2	==>	file is empty.
**							=	3	==> file must terminate with '.conf'.
							=	4	==> file have a wrong name or a bad ending.

		Level_error	=	1	==>	Parsing of the configuration file
			number_error	=	1	==>	data is emtpy.
							=	2	==> should start with 'server' and '{'.
							=	3	==>	should end with '}'.
							=	4	==> problem with bracket.
							=	5	==> missing a semi colon.
							=	6	==>	block server should start with 'server' and '{'.
							=	7	==> has a doublon.
							=	8	==> miss 'listen' directive.
							=	9	==>	miss 'server_name' directive.
							=	10	==> miss 'root' directive.
							=	11	==>	miss 'error_page' directive.
							=	12	==>	miss 'dav_methdods' directive.
							=	13	==> miss 'index' directive.

							=	14	==> directive unrecognized 

							=	15	==> in 'listen directive' it should end with \0
							=	16	==>	in 'listen directive' it should end with ';'
							=	17	==> in 'listen directive' it should have a ':' between host and port.
							=	18	==>	in 'listen directive' it has bad host or port.
							=	19	==>	in 'listen directive' host should be 127.0.0.1 or 'localhost'.
							=	20	==>  in 'listen directive' port should only have digit and a semicolon at the end.
							=	21	==>	in 'listen directive'  port should be between 0 and 65535.
							=	22	==>	in 'listen directive', bloc servers have the same port.

							=	23	==>	in 'server_name' directive, it should end with '\0'.
							=	24	==> in 'server_name' directive, it should only have alphanumeric characters.
							=	25	==>	 in 'server_name' directive, it should end with ';'.

							=	26	==>	in 'index' directive, it should end with '\0'.
							=	27	==>	in 'index' directive, it should end with ';'.
							=	28	==>	in 'index' directive, it should have a proper name and end with '.html'.
							=	29	==>	in 'index' directive, it should end with '.html'.
							=	30	==>	in 'index' directive, file doesn't exist.
							=	31	==>	in 'index' directive, file is empty.

							=	32	==>	 in 'root' directive, it should end with '\0'.
							=	33	==>	in 'root' directive, it should end with ';'.
							=	34	==> in 'root' directive, it should start with './'.
							=	35	==>	in 'root' directive doesn't exist!.

							=	36	==>	in 'autoindex' directive it should end with '\0'.
							=	37	==>	in 'autoindex' directive it should end with ';'.
							=	38	==> in 'autoindex' directive it should be 'on' or 'off'.
							
							=	39	==>	in 'dav_methods' directive, it can only have DELETE POST and GET methods!
							=	40	==>	in 'dav_methods' directive, it can only have 3 methods maximum!
							=	41	==>	in 'dav_methods' directive, methods are not correct.
							=	42	==> in 'dav_methods' directive, this method is unkonwn.
							=	43	==> in 'dav_methods' directive, it should not have doublons.

							=	44	==>	in 'error_page' directive, it should only have numbers then a directory!
							=	45	==>	in 'error_page' directive should end with a directory or file
							=	46	==>	in 'error_page' directive, the directory doesn't exist!
							=	47	==>	in 'error_page' directive, it cannot find the error file.
							=	48	==>	in 'error_page' directive, the file is empty.
							
							=	49	==>	in 'upload_store' directive, it should end with '\0'.
							=	50	==>  in 'upload_store' directive, it should end with ';'
							=	51	==> in 'upload_store' directive, it should start with './'
							=	52	==> in  'upload_store' directive , the folder doesn't exist!

							=	53	==>	in 'cgi_path' directive, it should end with '\0'.
							=	54	==>in 'cgi_path' directive, it should end with ';'.
							=	55	==> in 'cgi_path' directive, it should start with './'.
							=	56	==>	in 'cgi_path' directive, the folder doesn't exist!

							=	57	==>	in 'client_body_buffer_size' directive, it should only be digits.
							=	58	==>	in 'client_body_buffer_size' directive, buffer size must be between 8k and 16k.
							=	59	==>	in 'client_body_buffer_size' directive, buffer size must be between 8000 and 16000.
							=	60	==>	in 'client_body_buffer_size' directive, informations are corrupted.

							=	61	==>	a bloc server cannot have another bloc server inside.

							=	62	==>	 in 'location' directive, it needs 'location' + '/' + '{'.




							=	99	==>	'wrong argument given'.
							=	100	==>	wrong code client.
							=	101	==> wrong code server.


		Level_error	=	2	==>	HttpServer creation
			number_error	=	1	==>	cannot create a socket.
							=	2	==>	cannot set up the socket options.
							=	3	==>	cannot bind socket.
							=	4	==>	cannot listen.		
throw Error(1, "Error, the configuration file doesn't exist.", 0);






*/		

#endif	