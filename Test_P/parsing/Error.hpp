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

// # include <iosteam>
# include <exception>
# include <string>

class Error: public std::exception
{
    public:
        //     :m_numero(numero),m_phrase(phrase),m_niveau(niveau)
        Error(int number = 0, std::string const &line="", int level=0) throw();
    
        virtual const char* what() const throw();
        // {
        //     return m_phrase.c_str();
        // }git sta
        
        int get_level() const throw();
        // {
        //     return m_niveau;
        // }
        
        virtual ~Error() throw();
        // {}
 
    private:
        int _number_error;               //Numéro de l'erreur
        std::string _line_error;            //Description de l'erreur
        int _level_error;               //Niveau de l'erreur

};

#endif