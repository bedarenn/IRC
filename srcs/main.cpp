#include <iostream>

#include "Irc.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>

int	main(int ac, char **av){
	(void)av;
	(void)ac;

		struct sockaddr_in adr;

		int s = socket(AF_INET, SOCK_STREAM, 0); 		//creation d'un point d'acces
		adr.sin_family = AF_INET;						////////////////////////////////////////////////////////////
		adr.sin_addr.s_addr = INADDR_ANY;  				///// stockage des info(port, type) dans une struct ////////
		adr.sin_port = htons(8080);						////////////////////////////////////////////////////////////
		
		bind(s, (struct sockaddr*)&adr, sizeof(adr)); //adressage du point d'acces
		listen(s, 5); //mise en "ecoute" pour connection 2 = max queue length
		
		//select(); // connection...
/*	

	*	En général, pour les communications réseau, C++ offre des fonctions comme 
		-htonl() (host-to-network long) 
		-htons() (host-to-network short)
		pour convertir les valeurs de l'ordre local de l'hôte (qui peut être little-endian sur un PC classique) vers big-endian,

	*	fcntl() is used to set the O_NONBLOCK flag on the server socket file descriptor.
		This flag sets the socket to non-blocking mode

*/

	return (0);
}