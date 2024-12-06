#include <iostream>
#include <cstdlib>
#include <signal.h>
#include "Server.hpp"

int g_loop;

bool	parse(char **av);
void	set_sig(void);

int	main(int ac, char **av){
	if (ac != 3) {
		std::cerr << "error: numbers of arguments" << std::endl;
		return (1);
	}

	if(!parse(av))
		return(1);

	Server	server(av[0], atoi(av[1]), av[2]);
	set_sig();
	try {
		server.init_server();
		server.run();
	} catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}