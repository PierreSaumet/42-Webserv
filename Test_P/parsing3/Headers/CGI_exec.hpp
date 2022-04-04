/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI_exec.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaumet <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 11:07:34 by psaumet           #+#    #+#             */
/*   Updated: 2021/06/01 11:07:42 by psaumet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

#include <iostream>
#include <string>
#include <map>

class Cgi_exec
{
	public:
		Cgi_exec( void );
		Cgi_exec( const Cgi_exec &copy );
		~Cgi_exec( void );

		Cgi_exec		&operator=( const Cgi_exec &element );

		size_t			ft_setup_env_cgi( void );

		void			setSoftware( std::string const software );
		std::string		getSoftware( void );

		void			ft_display_all_variable_env( void );

	private:
		std::map<std::string, std::string>		_env_cgi;

};

#endif