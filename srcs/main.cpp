#include <iostream>
#include <fcntl.h>
#include <sys/socket.h>
//#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <cstdlib>
#include "Server.hpp"

int	spaced(std::string arg){
	int count = 0;
	for(int i = 0; i < static_cast<int>(arg.length()); i++){
		if(isspace(arg[i]))
			count++;
	}
	if(count == static_cast<int>(arg.length()))
		return(0);
	return(1);
}

int	parse(char **av, Server &server){
	std::string	arg;
	double port = 0;
	for(int i = 1; i < 2; i++){
		arg = av[i];
		if (arg.empty() || !spaced(arg)){
			std::cerr << "error: argument empty" << std::endl;
			return(0);
		}
		if(i == 1){
			port = strtod(arg.c_str(), NULL);
			if(port < 0 || port > 65535){
				std::cerr << "error: port not allowed" << std::endl;
				return (0);
			}
		}
	}
	server.setPort(port);
	server.setPass(arg);
	return(1);
}

int	main(int ac, char **av){
	if(ac == 3){
		Server server;
		if(!parse(av, server))
			return(0);
		struct sockaddr_in adr;
		int s = socket(AF_INET, SOCK_STREAM, 0); 		//creation d'un point d'acces
		fcntl(s, F_SETFL, O_NONBLOCK);
		adr.sin_family = AF_INET;						////////////////////////////////////////////////////////////
		adr.sin_addr.s_addr = INADDR_ANY;  				///// stockage des info(port, type) dans une struct ////////
		adr.sin_port = htons(server.getPort());			////////////////////////////////////////////////////////////
		bind(s, (struct sockaddr*)&adr, sizeof(adr)); 	//adressage du point d'acces
		listen(s, 1); 									//mise en "ecoute" pour connection; 2 = max queue length
		
		epoll_create(s); 										// connection...
	}
	else
		std::cerr << "error: numbers of arguments" << std::endl;
	return (0);
}