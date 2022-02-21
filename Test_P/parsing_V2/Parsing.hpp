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
# include <vector>
# include <sys/stat.h>

# include "Error.hpp"


class Parsing
{
	public:
		// struct s_server;

		typedef struct s_location
		{
			std::string		_root_location;
			std::vector<std::string>	_methods;
			s_location		*next;			
		}				t_location;

		typedef struct s_server {

			std::string		_host_server;	// listen bloc server
			int				_port_server;	// listen bloc server
			
			// Bloc server ou Bloc location
			// si rien setup a NULL;
			std::string		_name_server;
			std::string		_root_server;
			bool			_autoindex_server;
			std::map<int, std::string>	_error_server;
			std::string		_client_size;
			std::vector<std::string>	_methods_server;
			std::string		_index_server;

			// perso
			std::string		_cgi_path_server;
			std::string		_upload_store_server;

			size_t			_number_of_location;

			s_location		*location;
			s_server		*next;
		}				t_server;
		
		void	ft_insert_serv( void );
	
		Parsing( std::string config_file );
		Parsing( const Parsing &copy );
		~Parsing( void );
		Parsing		&operator=( const Parsing &element );

		/*
		**	Getters.
		**		Return the associated private member.
		*/
		std::string		get_name_of_file( void ) const;
		size_t			get_nbr_server( void ) const;


		/*
		**	This method checks if the configuration is valid
		*/
		bool			check_configuration_file( void ) const;
		bool		check_file_and_empty_exit( std::string &line ) const;
		/*
		**	This method delete all commentaries line from the configuration file
		*/
		std::string			delete_comm( std::string &line );
	
		/*
		**	These methods counts how many there are block server / location / error_page
		*/
		//size_t				_count_bloc_server( std::string &line ) const ;
	
		std::vector<t_server>		init_serv_struct( std::string &line );

		std::vector<t_server>		ft_initialize( std::string &line );

		size_t		find_ip_port( std::string &line, std::vector<t_server> &myvect, int index, size_t new_pos = 0);
		size_t	find_name_server( std::string &line, std::vector<t_server> &myvect, int index, size_t new_pos = 0);
		size_t	find_root( std::string &line, std::vector<Parsing::t_server> &myvect, int index, size_t new_pos );
		size_t	find_autoindex( std::string &line, std::vector<Parsing::t_server> &myvect, int index, size_t new_pos );
		size_t	find_error_page( std::string &line, std::vector<Parsing::t_server> &myvect, int index, size_t new_pos );
	
		bool					check_code_error_file( void );

		bool					client_code_error( int code ) const;
		bool					server_code_error( int code ) const;
		bool					check_code_error( int code ) const;
	private:
		Parsing( void );

		std::string		_name_of_file;
		size_t			_nbr_server;
		std::vector<t_server>		_server;
		std::map<int, int>	_nbr_loc_per_server;
		size_t			_nbr_location;

		/*
		**	This method pass all block possible
		*/
		size_t			_ft_is_blank( std::string &line, size_t to_start ) const;

		bool			_count_brackets( std::string &line ) const;

		/*
		**	These methods counts how many there are block server / location / error_page
		*/
		size_t				_count_bloc_server( std::string &line ) const ;
		bool				_check_bloc_server( std::string &line ) const;

		size_t _count_bloc_location (std::string &line ) const;
		bool				_check_bloc_location( std::string &line ) const;

		std::string			get_scope_v2( std::string &line, std::string &name , size_t i) const;

};



#endif