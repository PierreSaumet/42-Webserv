# include <iostream>
# include <sys/socket.h>	// For socket functions
# include <netinet/in.h>	// For sockaddr_in
# include <cstdlib>			// For exit() and EXIT_FAILURE/SUCESS
# include <unistd.h>		// For read

int				main( void ) 
{
	// First create a socket
	int			mysocket_fd = socket(AF_INET, SOCK_STREAM, 0);		// de type tcp/ip
	if (mysocket_fd == -1)											// si erreur
	{
		std::cout << "Failed to create socket. errno = " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	// Listen port 9999 and config the stucture ofr the connection
	sockaddr_in	my_sockaddr;
	my_sockaddr.sin_family = AF_INET;
	my_sockaddr.sin_addr.s_addr = INADDR_ANY;						// on ne veut pas bind notre socket a une adresse ip specifique
	my_sockaddr.sin_port = htons(9999);

	if (bind(mysocket_fd, (struct sockaddr*)&my_sockaddr, sizeof(my_sockaddr)) < 0)
	{
		std::cout << "Failed to bind to port 9999, ernno = " << errno << std::endl;
		exit(EXIT_FAILURE); 
	}

	//	Start listening, max 10 in the queue
	if (listen(mysocket_fd, 10) < 0)
	{
		std::cout << "Failed to listen, errno = " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	// Grab a connection , accepte it
	auto		addrlen = sizeof(my_sockaddr);
	int			connection = accept(mysocket_fd, (struct sockaddr*)&my_sockaddr, (socklen_t *)&my_sockaddr);
	if (connection < 0)
	{
		std::cout << "Failed to accept a connection, errno = " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	//	Read the message
	char		buffer[100];
	auto		bytesread = read(connection, buffer, 100);
	std::cout << "The message was: " << buffer << std::endl;

	//	Send message
	std::string	response = "Good talking to you \n";
	send(connection, response.c_str(), response.size(), 0);

	//	Close connections
	close(connection);
	close(mysocket_fd);


	exit(EXIT_SUCCESS);
}

/*
**	Tous fonctionne bien, on compile et on lance le programme
		c++ -o server server.cpp
		./server
	autre terminal on lance :
		telnet localhost 9999
	et on tape un message, genre salut.
	le server va recevoir le message l'afficher et fermer le programme.
	par contre quand on relance ca pete.
		Failed to bind to port 9999, ernno = 98
	
	En fait, dans une connection TCP, le socket ne sera pas tout de suite delivre.
	il faut attendre un peu et ca c'est embetant. 

	 == > Pour cela il faut utiliser setsockopt()
*/