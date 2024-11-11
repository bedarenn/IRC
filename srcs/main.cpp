#include <utility>

#include <cstdlib>

#include "Server.hpp"

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

int	main(int ac, char **av){
	if (ac != 3) {
		std::cerr << "error: numbers of arguments" << std::endl;
		return (1);
	}


	if(!parse(av))
		return(1);
	Server	server(atoi(av[1]), av[2]);
	server.init_server();
	server.run();
	return (0);
}