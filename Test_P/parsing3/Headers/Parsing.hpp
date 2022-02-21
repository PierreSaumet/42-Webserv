
#ifndef PARSING_HPP
# define PARSING_HPP

#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <sys/stat.h>
#include <stdlib.h>

class Parsing {

	public:

		typedef struct                  s_server {

			std::string                 host_server;
			size_t                      port_server;
			std::string                 name_server;
			std::string                 root_server;
			bool                        autoindex_server;
			size_t						buffer_size;
			std::string					cgi_path;
			std::string					upload_store;
			std::vector<std::string>    methods_server;
			std::map<int, std::string>  error_server;
		}                               t_server;

		Parsing( std::string &configfile );
		~Parsing( void );
		Parsing( const Parsing &copy );

		Parsing                         &operator=( const Parsing &element );

		bool                            ft_check_conf_file( void );
		
		std::vector<std::string>        ft_get_data_container( void );
		std::vector<t_server>           ft_get_server( void );

		bool                            ft_check_data( void );
		bool                            ft_check_number_of_bracket( void );
		bool                            ft_check_semicolon( void );
		bool                            ft_check_location( void );
		bool                            ft_check_server( void );
		bool                            ft_check_code_error( int code ) const;
		bool                            ft_check_code_serv( int code ) const ;
		bool                            ft_check_code_client( int code ) const;

		bool      		               	ft_get_listen( size_t k, std::vector<std::string> tmp, size_t index_server );
		bool                        	ft_get_server_name( size_t k, std::vector<std::string> tmp, size_t index_server );
		bool                        	ft_get_root( size_t k, std::vector<std::string> tmp, size_t index_server );
		bool                        	ft_get_autoindex( size_t k, std::vector<std::string> tmp, size_t index_server );
		bool							ft_get_buffer_size( size_t k, std::vector<std::string> tmp, size_t index_server );
		bool							ft_get_cgi_path( size_t k, std::vector<std::string> tmp, size_t index_server );
		bool							ft_get_upload_store( size_t k, std::vector<std::string> tmp, size_t index_server );
		size_t                      	ft_get_methods( size_t k, std::vector<std::string> tmp, size_t index_server );
		size_t                      	ft_get_error( size_t k, std::vector<std::string> tmp, size_t index_server );


		std::string                     ft_delete_comm( std::string &line );
		std::vector<std::string>        ft_get_scope( size_t index );

	private:
		Parsing( void );

		std::string                     _name_of_file;
		std::vector<std::string>        _data;
		size_t                          _nbr_servers;
		std::vector<t_server>           _servers;
	

};

#endif