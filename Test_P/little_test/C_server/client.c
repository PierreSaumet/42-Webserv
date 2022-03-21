# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>

void				error(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);	// devrait etre egal a 0
}

int					main(int argc, char **argv)
{
	int				sockfd;
	int				portno;
	int				n;

	struct sockaddr_in	serv_addr;
	struct hostent	*server; // = malloc(sizeof(struct hostent *));

	char			buffer[256];

	if (argc < 3)
	{
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(1);
	}
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sockfd < 0)
		error("Error opening socket\n");
	
	//printf("argv[1] = %s\n", argv[2]);
	server = gethostbyname(argv[2]);
	if (server == NULL)
	{
		fprintf(stderr, "Error, don't find the host\n");
		exit(EXIT_FAILURE);
	}
	
	// erreur segfautl test avec memset et memcpy 
	// le segfault ne venaitn pas de la lol
	//bzero((char *)&serv_addr, sizeof(serv_addr));
	memset((char *)&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	//printf("KEK\n");
	//bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	memmove((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	//printf("KEK\n");
	serv_addr.sin_port = htons(portno);

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		error("Error, cannot be connected\n");
	
	printf("Please enter the message: ");
	bzero(buffer, 256);
	fgets(buffer, 255, stdin);

	n = write(sockfd, buffer, strlen(buffer));

	if (n < 0)
		error("Error, cannot write in the socket\n");
	
	bzero(buffer, 256);

	n = read(sockfd, buffer, 255);

	if (n < 0)
		error("Error, reading from socket\n");
	printf("%s\n", buffer);

	close(sockfd);
	//free(server);
	exit(EXIT_SUCCESS);
}