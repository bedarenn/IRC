#include <utility>
#include <iostream>

#include <cstdlib>

#include <signal.h>

#include "Server.hpp"

int g_loop;

bool	test_str(const std::string& str) {
	if (str.empty())
		return (false);
	for (std::string::const_iterator it = str.begin(); it != str.end(); it++) {
		if (std::isspace(*it))
			return (false);
	}
	return(true);
}

bool	parse(char **av){
	w_port		port = 0;

	if (!test_str(av[1]) || !test_str(av[2])) {
		std::cerr << "error: argument empty" << std::endl;
		return(false);
	}
	port = atoi(av[1]);
	if(port < 0 || port > 65535){
		std::cerr << "error: port not allowed" << std::endl;
		return (0);
	}
	return(1);
}

void	sig_quit(int code) {
	g_loop = 0;
	if (code == SIGINT)
		std::cout << std::endl;
}

int	main(int ac, char **av){
	if (ac != 3) {
		std::cerr << "error: numbers of arguments" << std::endl;
		return (1);
	}

	if(!parse(av))
		return(1);
	Server	server(atoi(av[1]), av[2]);
	signal(SIGINT, sig_quit);
	try {
		server.init_server();
		server.run();
	} catch(const std::exception& e) { std::cerr << e.what() << std::endl; }
	return (0);
}