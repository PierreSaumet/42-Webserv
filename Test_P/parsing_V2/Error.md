Error(int number = 0, std::string const &line="", int level=0) throw();

level = 0
c'est toutes erreurs qui provient du fichier de .conf
	number = 0 = configuration file doesn't exist
	number = 1 = configuration file is empty
	number = 2 = configuration should end by '.conf'

level = 1
C'est toutes les erreurs de configuration
	number = -1 = general erreur
	number = 0 = no server bloc in the configuration file!
	number = 1 = configuration file should start by a bloc server!
	number = 2 = after 'server', should have '{'!
	number = 3 = after 'server', should have '{' at the same line!
	number = 4 = a bloc server cannot have another bloc server inside!
	number = 5 = no location server in the configuration file!
	number = 6 = after 'location', should have '/'!
	number = 7 = a bloc location should begin with '{'!
	number = 8 = location should be like 'location 	/example {' !

	number = 15 = server_name is not correct!

	number = 25 = root is not correct!
	number = 26 = root should start by '/'!"
	number = 27 = cannot open directory from root


	number = 35 = no autoindex!
	number = 39 = autoindex not valid!


	number = 45 = error_page is wrong!
