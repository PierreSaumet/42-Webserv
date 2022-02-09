# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

void			error(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

int				main(int argc, char **argv)
{
	int			sockfd;
	int			newsockfd;
	int			portno;

	socklen_t	clilen;
	char		buffer[256];
	struct sockaddr_in	serv_add;
	struct sockaddr_in	cli_addr;

	int			n;

	// on passe le port en argument
	if (argc < 2)
	{
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}

	//	on cree un socket
	sockfd = socket( AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("Error cannot create socket\n");
	
	// on nettoie l'addresse structure
	bzero((char *)&serv_add, sizeof(serv_add));

	portno = atoi(argv[1]);

	//	on setup la structure du socket
	serv_add.sin_family = AF_INET;
	serv_add.sin_addr.s_addr = INADDR_ANY;
	serv_add.sin_port = htons(portno);

	// on clean le socket == utile ?
	int val = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0)
	{
		error("Error reset socket\n");
	}

	// on le bind = on ecoute
	if (bind(sockfd, (struct sockaddr *)&serv_add, sizeof(serv_add)) < 0)
	{
		error("Erro cannot bind socket\n");
	}

	// on ecoute
	if (listen(sockfd, 10) < 0)
	{
		error("Error cannot listen");
	}

	// on recupere une connection avec accept // differe de server2
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
	if (newsockfd < 0)
		error("Error cannot accept a connection\n");
	else
	{
		printf("server: got a connection from %s port %d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
	}

	// on envoie un message au client pour dire ok
	send(newsockfd, "Yo, ca marche\n", 14, 0);

	// on clean le buffer avant de le lire
	bzero(buffer, 256);

	n = read(newsockfd, buffer, 255);
	if (n < 0)
		error("Error, cannot read from the buffer\n");
	
	printf("Here is the message: %s\n", buffer);

	close(newsockfd);
	close(sockfd);

	exit(EXIT_SUCCESS);
}