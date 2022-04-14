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

#include "../Headers/CGI_exec.hpp"

Cgi_exec::Cgi_exec( void ) : _env_cgi()
{
	return ;
}

Cgi_exec::Cgi_exec( const Cgi_exec &copy ) : _env_cgi(copy._env_cgi)
{
	return ;
}

Cgi_exec::~Cgi_exec( void )
{
	if (this->_env_cgi.empty() == 0)
	{
		this->_env_cgi.clear();
	}
	return ;
}

Cgi_exec		&Cgi_exec::operator=( const Cgi_exec &element )
{
	if (this != &element)
	{
		this->_env_cgi = element._env_cgi;
	}
	return (*this);
}

size_t			Cgi_exec::ft_setup_env_cgi( void )
{
	// setup all data to null ... 
	//https://fr.wikipedia.org/wiki/Variables_d%27environnement_CGI
	this->_env_cgi.insert(std::pair<std::string, std::string>("SERVER_SOFTWARE", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("SERVER_NAME", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("GATEWAY_INTERFACE", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("SERVER_PROTOCOL", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("SERVER_PORT", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("REQUEST_METHOD", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("PATH_INFO", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("PATH_TRANSLATED", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("SCRIPT_NAME", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("QUERY_STRING", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("REMOTE_ADDR", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("AUTH_TYPE", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("REMOTE_USER", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("REMOTE_IDENT", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("CONTENT_TYPES", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("CONTENT_LENGTH", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("REQUEST_URI", "NULL"));




	// PAS OBLIGE JE CROIS... DOIT METTRE VIDE CAD LE SUPPRIMER
	this->_env_cgi.insert(std::pair<std::string, std::string>("REMOTE_HOST", "NULL"));
	

	// PAS NECESSAIRE JE CROIS
	this->_env_cgi.insert(std::pair<std::string, std::string>("HTTP_ACCEPT", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("HTTP_ACCEPT_LANGUAGE", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("HTTP_USER_AGENT", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("HTTP_COOKIE", "NULL"));
	this->_env_cgi.insert(std::pair<std::string, std::string>("HTTP_REFERER", "NULL"));

	//	A rajouter un status code ? je ne sais pas si ca existe mais Jerome m'a dit que oui
	//this->_env_cgi.insert(std::pair<std::string, std::string>("REDIRECT_STATUS", "NULL"));

	std::cout << "this->_env_cgi contains : " << std::endl;
	std::map<std::string, std::string>::const_iterator		it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator		it_e = this->_env_cgi.end();
	for ( ; it_b != it_e; it_b++)
	{
		std::cout << "it_b->first = " << it_b->first << " and it_b->second = " << it_b->second << std::endl;
	}
	return (0);
}

/*
**	pour une requete GET il nous faut
	CONTENT_LENGTH = vide ?
	CONTENT_TYPE = vide ?
	GATEWAY_INTERFACE = CGI/1.1
	HTTP_accept = valeur du header:
		exemple = text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*//*;q=0.8
	PATH_INFO = le chemin du fichier php:
		exemple = /home/pierre/dossier/webser/test/parsing3/root/Hello/fichier.php
	PATH_TRANSLATED = pareil que path_info
		exemple = /home/pierre/dossier/webser/test/parsing3/root/Hello/fichier.php
	QUERY_STRING = les valeurs qui proviennet de l'url car get
		exemple = name=SAUMET&prenom=PIERRE
	REDIRECT_STATUS = 200  ==> verifier avant que les fichiers sont bons et les donnees le sont egalement
	REQUEST_METHOD = GET
	REQUEST_URI = l'addresse du fichier + les info
		exemple = /Hello/query_get_test.php?name=SAUMET
	SCRIPT_FILENAME = nom du fichier
		exemple = query_test_get.php
	SCRIPT_NAME = pareil que script_filemane
		exemple = query_test_get.php
	SERVER_NAME = le nom du server
	SERVER_PORT = le port du server ou on ecoute
	SERVER_PROTOCOl = HTTP/1.1
	SERVER_SOFTWARE = webserv/1.0 enfin notre server
	STATUS_CODE = 200 comme redirect status

	et le write on lui donne une string vide. je pense
*/

/*
**	pour une requete POST il nous faut
	CONTENT_LENGTH = la valeur du body recu enfin la taille 
		exemple = 6 pour pierre
	CONTENT_TYPE = valeur qu'on trovue dans le header de la requete
		exemple = application/x-www-form-urlencoded
	GATEWAY_INTERFACE = CGI/1.1
	HTTP_ACCEPT = valeur qu'on trouve dans le header de la requete
		exemple = text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*//*;q=0.8
	PATH_INFO = pareil que pour GET
	PATH_TRANSLATED = pareil que pour GEt
	REDIRECT_STTUS = 200
	REQUEST_METHOD = POST
	REQUEST_URO = nom du fichier suelement
		exemple = /fichier.php
	SCRIPT_NAME = nom du fichier
		exemple = /fichier.php
	SERVER_NAME	= le nom du server
	SERVEr_PORt = le ort du server
	SERVER_PROTOCOL = HTTP/1.1
	SERVER_SOFTWARE = bah server webserv
	STATTUS _CODE = 200

	et pour le write, il faut lui transmettre les donnees qu'on a recupere
	genre = name=pierre?prenom=paindemie
*/

//	test a supprimer
void		Cgi_exec::ft_test( void )
{
	// this->setServerPort("HTTP/1.1");
	// this->setServerName("127.0.0.1");
	// this->setServerPort("8080");
	// this->setServerSoftware("Webserv/1.0");
	// this->setGatewayInterface("CGI/1.1");
	// this->setRequestUri("/Hello/query_get_test.php?name=Pierre&prenom=Saumet&message=PARFAIT");
	// this->setHttpAccept("test/html");


	char **sysCline = NULL;
	char **sysENV = NULL;
	// std::string		sData ="var_1=val1&var_=val2";

	std::vector<std::string> aArgs;
	aArgs.push_back("/usr/bin/php-cgi");
	aArgs.push_back("./root/Hello/query_get_test.php");

	sysCline = new char*[aArgs.size()+1];
	for (unsigned long i = 0; i < aArgs.size(); i++)
	{
		sysCline[i] = new char[aArgs[i].size() + 1];
		strncpy(sysCline[i], aArgs[i].c_str(), aArgs[i].size() + 1);
		std::cout << "syscline[" << i << "] = " << sysCline[i] << std::endl;
	}
	sysCline[aArgs.size()] = NULL;


	// DONC pour _GET dans le fichier query_get_test
	// il faut que QUERY_STRING possede les valeurs
	// ex = QUERY_STRING=name=pierre
	//	et dans le write, bah on peut ecrier n'importe quoi... bizarre

	//	DONC pour _POST dans le fichier query_get_test
	//	il faut que QUERY_STRINg possede test=querystring
	std::vector<std::string> aEnv; 
	aEnv.push_back("GATEWAY_INTERFACE=CGI/1.1");
	aEnv.push_back("SERVER_PROTOCOL=HTTP/1.1");
	// pour tester avec get
	// aEnv.push_back("QUERY_STRING=name=pierre");
	// aEnv.push_back("REQUEST_URI=./root/Hello/query_get_test.php?name=pierre");
	
	
	// pour tester avec post
	// aEnv.push_back("QUERY_STRING=test=querystring");
	 aEnv.push_back("REQUEST_URI=./root/Hello/query_get_test.php");
	
	aEnv.push_back("REDIRECT_STATUS=200");
	
	
	aEnv.push_back("REQUEST_METHOD=POST");	// avec post ca ne marche pas ... bizarre
	
	aEnv.push_back("CONTENT_TYPE=application/x-www-form-urlencoded;charset=utf-8");

	// aEnv.push_back("SCRIPT_FILENAME=./test1.php");
	aEnv.push_back("SCRIPT_FILENAME=./root/Hello/query_get_test.php");
	// aEnv.push_back("CONTENT_LENGTH=500");				//+ std::to_string(sData.length()) );
	
	aEnv.push_back("CONTENT_LENGTH=11"); //+ std::to_string(sData.length()) );
	sysENV = new char*[aEnv.size() + 1];
	for (unsigned long i = 0; i < aEnv.size(); i++)
	{
		sysENV[i] = new char[aEnv[i].size() + 1];
		strncpy(sysENV[i], aEnv[i].c_str(), aEnv[i].size() + 1);
		std::cout << "sysENV[" << i << "] = " << sysENV[i] << std::endl;

	}
	sysENV[aEnv.size()] = NULL;

	pid_t pid;
	int stdin_tmp = dup(STDIN_FILENO);
	int stdout_tmp = dup(STDOUT_FILENO);

	FILE *file_in = tmpfile();		// cree un fichier temporaire dans le cas de grosse donnees je pense
	if (file_in == NULL)
	{
		std::cout << "ERREUR ne peut pas creer un fichier temporaire" << std::endl;
		std::cout << "doit nretourner une erreur de type server genre 500" << std::endl;
	}
	FILE *file_out = tmpfile();
	if (file_out == NULL)
	{
		std::cout << "ERREUR ne peut pas creer un fichier temporaire" << std::endl;
		std::cout << "doit nretourner une erreur de type server genre 500" << std::endl;
	}
	long fd_in = fileno(file_in);
	if (fd_in == -1)
	{
		std::cout << "errreur ne permet pas d'identifier le file descriptor" << std::endl;
		std::cout << "doit nretourner une erreur de type server genre 500" << std::endl;
	}
	long fd_out = fileno(file_out);
	if (fd_out == -1)
	{
		std::cout << "errreur ne permet pas d'identifier le file descriptor" << std::endl;
		std::cout << "doit nretourner une erreur de type server genre 500" << std::endl;
	}


	//	on ecrit dans fd_in car c'est dedans qu'on va passer les valeurs recuperer via le parsing
	//	et les transmettre au CGI.
	// euh quand c'est un post il faut mettre les donnes ici 
	write(fd_in, "name=pierre", 11);
	//	On replace le curseur de lecture de fd_in au debut
	lseek(fd_in, 0, SEEK_SET);

	pid = fork();
	if (pid == -1)
	{
		std::cerr << "For kcrash" << std::endl;
		std::cout << "doit nretourner une erreur de type server genre 500" << std::endl;
		exit(1);
	}
	else if (pid == 0)		// enfant lol
	{
		// On duplique les files descriptors d'entree et de sortie.
		if (dup2(fd_in, 0) == -1) // STDIN_FILENO
		{
			std::cout << "erreur dup2 fd_in " << std::endl;
			std::cout << "doit retourner une erruer" << std::endl;
			exit(1);
		}
		if (dup2(fd_out, 1) == -1) // STDOUT_FILENO
		{
			std::cout << "erreur dup2 fd_in " << std::endl;
			std::cout << "doit retourner une erruer" << std::endl;
			exit(1);
		}
		// on execute le cgi avec execve
		if (execve(sysCline[0], sysCline, sysENV) == -1)
		{
			std::cerr << "Error cgi" << std::endl;
			std::cout << "doit retourner une erruer genre 500" << std::endl;
		}
	}
	else // parenmt
	{
		// on setup un buffer pour recuperer les donnees
		//	a voir pour la taille, j'ai mis 2000 aleatoirement
		unsigned char buffer[2000] = {0};

		// on attend la fin du processus 
		waitpid(-1, NULL, 0);
		//	on place la tete de lecture de fd_out au debut
		lseek(fd_out, 0, SEEK_SET);
		std::cout << "dans parent apres waitpid" << std::endl;

		int ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, 2000);
			// on lit fd_out et on le place dans le buffer
			ret = read(fd_out, buffer, 2000 - 1);
			std::cout << "dans parents et buffer = " << buffer << std::endl;
			// on termine la chaine de charactere par 0
			buffer[ret] = 0;
			if (ret > 0)
			{
				std::cout << "\n\n bingo buffer = " << buffer << std::endl;
				// du coup faudra rajouter la chaine de charactere a une string a chaque lecture
			}
		}

	}
	dup2(stdin_tmp, 0);
	dup2(stdout_tmp, 1);
	fclose(file_in);
	fclose(file_out);
	close(fd_in);
	close(fd_out);
	close(stdin_tmp);
	close(stdout_tmp);

	// faut nettoyer pour les leaks
	for (unsigned long i = 0; i < aEnv.size(); i++)
	{
		delete [] sysENV[i];
	}
	delete [] sysENV;

	for (unsigned long i = 0; i < aArgs.size(); i++)
	{
		delete [] sysCline[i];
	}
	delete [] sysCline;


	return ;
}

std::string	Cgi_exec::ft_execute_cgi( void )
{
	std::cout << "Dans ft_execute_cgi " << std::endl;

	/*
			PSEUDO CODE A FAIRE	


			1 ) mettre a jour les variables environnement de cgi
				exemples :
							==>	SERVER_SOFTWARE	=	"webser/1"
							==>	SERVER_PROTOCOL	=	"HTTP/1.1"
					c'est les infos qu'on a dans la requete.

				il faut aussi pour execve mettre les variables dans un char** pour le troisieme argument

			2 ) init les variables locales a utiliser
				pid_t pid;
				int		stdin?
				int		stdout?

				stdin = dup(0)
				stdout = dup(1);
				pid = fork()
			3 ) les conditions du fork()
				if (pid < 0)
					error
				else if (pid == 0) child
				{
					dup2
					dup2
					execute ?
						si execute < 0 
							error
				}
				else	parent
				{
					waitpid()
					recuperer la string du "retour de execute"

				}
			4 ) retourner la string contenant les infos du CGI
				pour l'afficher apres dans le read.
	*/


	return ("");
}


/*
**	SETTERS
*/
void	Cgi_exec::setRequestUri( std::string const request_uri )
{
	this->_env_cgi["REQUEST_URI"] = request_uri;
	return ;
}
void	Cgi_exec::setServerSoftware( std::string const software )
{
	this->_env_cgi["SERVER_SOFTWARE"] = software;
	return ;
}

void	Cgi_exec::setServerName( std::string const name )
{
	this->_env_cgi["SERVER_NAME"] = name;
	return ;
}

void	Cgi_exec::setGatewayInterface( std::string const gateway )
{
	this->_env_cgi["GATEWAY_INTERFACE"] = gateway;
	return ;
}

void	Cgi_exec::setServerProtocol( std::string const protocol )
{
	this->_env_cgi["SERVER_PROTOCOL"] = protocol;
	return ;
}

void	Cgi_exec::setServerPort( std::string const port )
{
	this->_env_cgi["SERVER_PORT"] = port;
	return ;
}

void	Cgi_exec::setRequestMethod( std::string const method )
{
	this->_env_cgi["REQUEST_METHOD"] = method;
	return ;
}

void	Cgi_exec::setPathInfo( std::string const path_info )
{
	this->_env_cgi["PATH_INFO"] = path_info;
	return ;
}

void	Cgi_exec::setPathTranslated( std::string const path_trans )
{
	this->_env_cgi["PATH_TRANSLATED"] = path_trans;
	return ;
}

void	Cgi_exec::setScriptName( std::string const script_name )
{
	this->_env_cgi["SCRIPT_NAME"] = script_name;
	return ;
}

void	Cgi_exec::setQueryString( std::string const query_string )
{
	this->_env_cgi["QUERY_STRING"] = query_string;
	return ;
}

void	Cgi_exec::setRemoteHost( std::string const remote_host )
{
	this->_env_cgi["REMOTE_HOST"] = remote_host;
	return ;
}
void	Cgi_exec::setRemoteADDR( std::string const remote_addr )
{
	this->_env_cgi["REMOTE_ADDR"] = remote_addr;
	return ;
}
void	Cgi_exec::setAuthType( std::string const auth_type )
{
	this->_env_cgi["AUTH_TYPE"] = auth_type;
	return ;
}
void	Cgi_exec::setRemoteUser( std::string const remote_user )
{
	this->_env_cgi["REMOTE_USER"] = remote_user;
	return ;
}
void	Cgi_exec::setRemoteIdent( std::string const remote_ident )
{
	this->_env_cgi["REMOTE_IDENT"] = remote_ident;
	return ;
}
void	Cgi_exec::setContentTypes( std::string const content_type )
{
	this->_env_cgi["CONTENT_TYPES"] = content_type;
	return ;
}
void	Cgi_exec::setContentLength( std::string const content_length )
{
	this->_env_cgi["CONTENT_LENGTH"] = content_length;
	return ;
}
void	Cgi_exec::setHttpAccept( std::string const http_accept )
{
	this->_env_cgi["HTTP_ACCEPT"] = http_accept;
	return ;
}
void	Cgi_exec::setHttpAcceptLanguage( std::string const http_languange )
{
	this->_env_cgi["HTTP_ACCEPT_LANGUAGE"] = http_languange;
	return ;
}
void	Cgi_exec::setHttpUserAgent( std::string const http_user )
{
	this->_env_cgi["HTTP_USER_AGENT"] = http_user;
	return ;
}
void	Cgi_exec::setHttpCookie( std::string const http_cookie )
{
	this->_env_cgi["HTTP_COOKIE"] = http_cookie;
	return ;
}
void	Cgi_exec::setHttpReferer( std::string const http_referer )
{
	this->_env_cgi["HTTP_REFERER"] = http_referer;
	return ;
}

/*
**	GETTERS
*/
std::string Cgi_exec::getRequestUri( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "REQUEST_URI")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getServerSoftware( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "SERVER_SOFTWARE")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getServerName( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "SERVER_NAME")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getGatewayInterface( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "GATEWAY_INTERFACE")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getServerProtocol( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "SERVER_PROTOCOL")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getServerPort( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "SERVER_PORT")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getRequestMethod( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "REQUEST_METHOD")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getPathInfo( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "PATH_INFO")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getPathTranslated( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "PATH_TRANSLATED")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getScriptName( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "SCRIPT_NAME")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getQueryString( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "QUERY_STRING")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getRemoteHost( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "REMOTE_HOST")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getRemoteAddr( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "REMOTE_ADDR")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getAuthType( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "AUTH_TYPE")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getRemoteUser( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "REMOTE_USER")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getRemoteIdent( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "REMOTE_IDENT")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getContentTypes( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "CONTENT_TYPES")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getContentLength( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "CONTENT_LENGTH")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getHttpAccept( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "HTTP_ACCEPT")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getHttpAcceptLanguage( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "HTTP_ACCEPT_LANGUAGE")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getHttpUserAgent( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "HTTP_USER_AGENT")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getCookie( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "HTTP_COOKIE")
			return (it_b->second);
	}
	return ("");
}

std::string	Cgi_exec::getReferer( void ) const
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		if (it_b->first == "HTTP_REFERER")
			return (it_b->second);
	}
	return ("");
}





void	Cgi_exec::ft_display_all_variable_env( void )
{
	std::map<std::string, std::string>::const_iterator it_b = this->_env_cgi.begin();
	std::map<std::string, std::string>::const_iterator it_e = this->_env_cgi.end();
	
	for (; it_b != it_e; it_b++)
	{
		std::cout << it_b->first << " = " << it_b->second << std::endl;
	}
	return ;
}