#include <iostream>

#include "Irc.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int	main(int ac, char **av){
	if(ac == 3){
		struct sockaddr_in *adr;

		(void)av;
		int s = socket(AF_INET, SOCK_STREAM, 0); //creation d'un point d'acces

		adr.sin_family = AF_INET;			/////////////////////////////////////////////////////////////////////
		adr.sin_addr.s_addr = INADDDR_ANY;  //stockage des info(port, type) dans une struct?? en cpp??///////////
		adr.sin_port = hton(8080);			/////////////////////////////////////////////////////////////////////
		
		bind(s, &adr, sizeof(adr)); //adressage du point d'acces
		
		listen(s, 2); //mise en "ecoute" pour connection
		
		select() // connection...

	}
	return (0);
}