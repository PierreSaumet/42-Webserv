Error(int number = 0, std::string const &line="", int level=0) throw();

level = 0
c'est toutes erreurs qui provient du fichier de .conf
	number = 0 = extension pas bon
	number = 1 = nom du fichier pas bon
	number = 2 = fichier existe pas
	number = 3 = fichier vide

level = 1
C'est toutes les erreurs de configuration
	number = -1 = general erreur
	number = 0 = pas de listen
	number = 1 = pas de host ou mauvais host
	number = 2 = pas de port ou mauvais port
	number = 3 = pas de server_name
	number = 4 = pas de root
	number = 5 = pas autoindex
	number = 6 = autoindex error
	number = 7 = pas d'erreur_page
	number = 8 = code error worng
	number = 9 = pas d'index


	number = 10 = error_code non conforme
	number = 11 = index should end by .htnml


	number = 20 = cannot open directory
	number = 99 = pas de ;