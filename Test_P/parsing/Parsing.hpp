/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
# define PARSING_HPP

# include <iostream>
# include <cstring>
# include <string>
# include <fstream>
# include <sstream>
# include <map>
# include <sys/stat.h>

# include "Error.hpp"


class Parsing
{
	public:
		Parsing( std::string configuration_file );
		Parsing( const Parsing &copy );
		~Parsing( void );
		Parsing		&operator=( const Parsing &element );
	
		/*
		**	Setters
		*/
		void				set_name_of_file( const std::string &new_name );

		/*
		**	Getters
		*/
		std::string			get_name_of_file( void ) const;
		std::string			get_ip_server( void ) const;
		int					get_port( void ) const;
		std::string			get_root( void ) const;
		int					get_autoindex( void ) const;
		std::string			get_index( void ) const;
		std::map<int, std::string>	get_error_page( void ) const;
	
		// Check the file configuration
		bool				check_extension( void ) const;
		bool				check_exist( void ) const;
		bool				check_empty( void ) const;
	
		// find the informations
		std::string			find_ip_port( std::string &line );
		std::string			find_name_server( std::string &line );
		std::string			find_root( std::string &line );
		int					find_autoindex( std::string &line );
		std::map<int, std::string>	find_error_page( std::string &line );
		std::string			find_index( std::string &line );
		std::string			find_client_max_size( std::string &line );

		size_t				count_location( std::string &line ) const ;
		size_t				count_server( std::string &line ) const ;
		size_t				count_error_page( std::string &line ) const;


	private:
		Parsing( void );
		std::string			delete_comm( std::string &line );		// delete commentaire

		bool				check_code_error( int code ) const;
		bool				server_code_error( int code ) const;
		bool				client_code_error( int code ) const;
		bool				check_code_error_file( std::string &line );
		
		bool				check_file_and_empty_exit( std::string &name ) const;
		
		// fonctions pour scope
		bool				count_scope( std::string &line ) const; 
		std::string			get_scope( std::string &line ) const;
		std::string			get_scope_v2( std::string &line, std::string &name ) const;
		


		size_t				ft_is_blank( std::string &line, size_t to_start);
		// F

		std::string			_name_of_file;

		// server informations
		std::string			_ip_server;
		std::string			_name_server;
		int					_port;
		std::string			_root;				// repertoire qui heberge le site wen
		int					_autoindex;
		std::map<int, std::string>		_error_page;
		std::string			_index;
		std::string			_client_max_size;
};

#endif