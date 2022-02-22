
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

		typedef struct					s_location {

			std::string					name_location;
			std::string                 root_location;
			std::string					index_location;
			bool                        autoindex_location;
			std::map<int, std::string>  error_location;
			std::vector<std::string>    methods_location;
			std::string					upload_store_location;
			size_t						buffer_size_location;
		}								t_location;

		typedef struct                  s_server {

			std::string                 host_server;
			size_t                      port_server;
			std::string                 name_server;
			std::string                 root_server;
			std::string					index_server;
			bool                        autoindex_server;
			size_t						buffer_size_server;
			std::string					cgi_path_server;
			std::string					upload_store_server;
			std::vector<std::string>    methods_server;
			std::map<int, std::string>  error_server;

			std::vector<t_location>		location;
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

		// For server bloc
		bool      		               	ft_get_listen( size_t k, std::vector<std::string> tmp, size_t index_server );
		bool                        	ft_get_server_name( size_t k, std::vector<std::string> tmp, size_t index_server );
		bool                        	ft_get_root( size_t k, std::vector<std::string> tmp, size_t index_server );
		bool                        	ft_get_autoindex( size_t k, std::vector<std::string> tmp, size_t index_server );
		bool							ft_get_buffer_size( size_t k, std::vector<std::string> tmp, size_t index_server );
		bool							ft_get_cgi_path( size_t k, std::vector<std::string> tmp, size_t index_server );
		bool							ft_get_upload_store( size_t k, std::vector<std::string> tmp, size_t index_server );
		bool							ft_get_index( size_t k, std::vector<std::string> tmp, size_t index_server );
		size_t                      	ft_get_methods( size_t k, std::vector<std::string> tmp, size_t index_server );
		size_t                      	ft_get_error( size_t k, std::vector<std::string> tmp, size_t index_server );

		bool			ft_check_directive_server( std::vector<std::string> scope_server, size_t server_size );

		// For location bloc
		bool           					ft_get_root_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location );
		bool           					ft_get_autoindex_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location );
		bool							ft_get_upload_store_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location );
		bool							ft_get_buffer_size_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location );
		bool							ft_get_index_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location );
		size_t          				ft_get_error_location( size_t k, std::vector<std::string> tmp, size_t index_server, size_t index_location );
		size_t                      	ft_get_methods_location( size_t k, std::vector<std::string> tmp, size_t index_server,  size_t index_location);


		size_t							ft_get_location( size_t k, std::vector<std::string> tmp, size_t index_server );

		std::string                     ft_delete_comm( std::string &line );
		std::vector<std::string>        ft_get_scope( size_t index );



		void							display_all( void );

	private:
		Parsing( void );

		std::string                     _name_of_file;
		std::vector<std::string>        _data;
		size_t                          _nbr_servers;
		std::vector<t_server>           _servers;
	

};

#endif